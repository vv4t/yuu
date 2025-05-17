#include "scene.hpp"

enum input_axis {
  AXIS_CURSOR_X,
  AXIS_CURSOR_Y,
  AXIS_MOUSE
};

static std::vector<ubo_t::field_t> make_fields(scene_file_t& scene_file);
static target_t create_target(std::vector<texture_ref_t> output);
static ubo_t::type_t type_from_string(std::string type_string);

scene_t::scene_t(input_t& input, scene_file_t& scene_file)
  : m_ubo(0, make_fields(scene_file)),
    m_input(input),
    m_time(0.0),
    m_width(scene_file.get_width()), 
    m_height(scene_file.get_height()) {
  m_passes.reserve(16);
  m_input.bind_move(AXIS_CURSOR_X, AXIS_CURSOR_Y);
  m_input.bind_button(AXIS_MOUSE, 1);

  for (auto& data : scene_file.get_data()) {
    add_data(type_from_string(data.get_type()), data.get_name());
  }

  for (auto& script : scene_file.get_scripts()) {
    add_script(script);
  }
  
  for (auto& buffer : scene_file.get_buffers()) {
    add_buffer(buffer.get_name(), buffer.get_width(), buffer.get_height());
  }
  
  for (auto& image : scene_file.get_images()) {
    load_image(image.get_name(), image.get_src());
  }
  
  for (auto& cubemap : scene_file.get_cubemaps()) {
    load_cubemap(cubemap.get_name(), cubemap.get_src());
  }
  
  for (auto& shader : scene_file.get_shaders()) {
    add_shader(shader.get_name(), shader.get_src(), shader.get_channels());
  }
  
  for (auto& pass : scene_file.get_renderer()) {
    add_pass(pass.get_shader(), pass.get_input(), pass.get_output());
  }
}

void scene_t::render() {
  handle_input();

  m_logic.update();
  m_ubo.sub("_pad_", m_logic.get_data().data(), sizeof(float), m_logic.get_data().size() * sizeof(float));

  float mouse [] = {
    m_input.get_axis(AXIS_CURSOR_X),
    m_input.get_axis(AXIS_CURSOR_Y),
    m_cursor_x,
    m_cursor_y
  };

  m_ubo.sub("iTime", &m_time, 0, sizeof(m_time));
  m_ubo.sub("iMouse", mouse, 0, sizeof(mouse));

  for (auto& pass : m_passes) {
    pass.bind(m_ubo);
    m_mesh.draw();
  }
  
  m_time += 0.015;
}

void scene_t::handle_input() {
  bool mouse_down = m_cursor_x < 0.0;

  if (!mouse_down && m_input.get_axis(AXIS_MOUSE)) {
    m_cursor_x = m_input.get_axis(AXIS_CURSOR_X);
    m_cursor_y = m_input.get_axis(AXIS_CURSOR_Y);
  }
  
  float sgn = m_input.get_axis(AXIS_MOUSE) ? -1.0 : 1.0;
  m_cursor_x = sgn * std::abs(m_cursor_x);
  m_cursor_y = sgn * std::abs(m_cursor_y);
}

void scene_t::load_image(std::string name, std::string src) {
  m_textures.try_emplace(name, src.c_str());
}

void scene_t::load_cubemap(std::string name, std::string src) {
  m_cubemaps.try_emplace(name, src.c_str());
}

void scene_t::add_buffer(std::string name, int width, int height) {
  m_textures.try_emplace(name, width, height, GL_RGBA, GL_RGBA32F, GL_FLOAT);
}

void scene_t::add_script(std::string src) {
  m_logic.add_script(src);
}

void scene_t::add_data(ubo_t::type_t type, std::string name) {
  switch (type) {
  case ubo_t::type_t::FLOAT:
    m_logic.define_float(name);
    break;
  case ubo_t::type_t::VEC2:
    m_logic.define_vec2(name);
    break;
  case ubo_t::type_t::VEC3:
    m_logic.define_vec3(name);
    break;
  case ubo_t::type_t::VEC4:
    m_logic.define_vec4(name);
    break;
  }
}

void scene_t::add_shader(std::string name, std::string src, std::vector<std::string> channels) {
  std::stringstream src_vertex, src_fragment;
  src_vertex << "layout (location = 0) in vec2 v_pos;" << std::endl;
  src_vertex << "void main() { gl_Position = vec4(v_pos, 0.0, 1.0); }" << std::endl;
  src_fragment << m_ubo.get_definition() << std::endl;
  src_fragment << shader_read_source(src.c_str()).rdbuf() << std::endl;
  m_shaders.try_emplace(name, src_vertex, src_fragment);
  shader_t& shader = m_shaders.at(name);
  m_ubo.attach_shader(shader);
  
  for (long unsigned int i = 0; i < channels.size(); i++) {
    shader.uniform_int(channels[i].c_str(), i);
  }
}

void scene_t::add_pass(std::string shader, std::vector<std::string> input, std::vector<std::string> output) {
  std::vector<bindable_ref_t> bindings;
  for (auto& name : input) {
    if (m_textures.find(name) != m_textures.end())
      bindings.push_back(m_textures.at(name));
    else if (m_cubemaps.find(name) != m_cubemaps.end())
      bindings.push_back(m_cubemaps.at(name));
    else
      throw std::runtime_error("buffer '" + name + "' does not exist.");
  }
  
  int width = m_width, height = m_height;
  
  std::vector<texture_ref_t> target;
  for (auto& name : output) {
    texture_t& buffer = m_textures.at(name);
    target.push_back(buffer);
    width = std::min(buffer.get_width(), width);
    height = std::min(buffer.get_height(), height);
  }
  
  m_passes.emplace_back(m_shaders.at(shader), bindings, target, width, height);
}

scene_t::pass_t::pass_t(shader_t& shader, std::vector<bindable_ref_t> input, std::vector<texture_ref_t> output, int width, int height)
  : m_shader(shader),
    m_bindings(input),
    m_target(create_target(output)),
    m_width(width),
    m_height(height) {}

void scene_t::pass_t::bind(ubo_t& ubo) {
  glViewport(0, 0, m_width, m_height);
  float resolution[] = { (float) m_width, (float) m_height };
  ubo.sub("iResolution", resolution, 0, sizeof(resolution));
  
  m_shader.bind();
  for (unsigned long int i = 0; i < m_bindings.size(); i++)
    m_bindings[i].get().bind(i);
  m_target.bind();
}

target_t create_target(std::vector<texture_ref_t> output) {
  std::vector<target_t::binding_t> bindings;
  
  for (unsigned long int i = 0; i < output.size(); i++) {
    bindings.push_back(target_t::binding_t(GL_COLOR_ATTACHMENT0 + i, output[i]));
  }
  
  return target_t(bindings);
}

ubo_t::type_t type_from_string(std::string type_string) {
  if (type_string == "float") return ubo_t::type_t::FLOAT;
  if (type_string == "vec2") return ubo_t::type_t::VEC2;
  if (type_string == "vec3") return ubo_t::type_t::VEC3;
  if (type_string == "vec4") return ubo_t::type_t::VEC4;
  throw std::runtime_error("unknown type: " + type_string);
}

std::vector<ubo_t::field_t> make_fields(scene_file_t& scene_file) {
  std::vector<ubo_t::field_t> fields = {
    ubo_t::field_t(ubo_t::VEC4, "iMouse"),
    ubo_t::field_t(ubo_t::VEC2, "iResolution"),
    ubo_t::field_t(ubo_t::FLOAT, "iTime"),
    ubo_t::field_t(ubo_t::FLOAT, "_pad_"),
  };
  
  for (auto& data : scene_file.get_data()) {
    fields.push_back(ubo_t::field_t(type_from_string(data.get_type()), data.get_name()));
  }

  return fields;
}
