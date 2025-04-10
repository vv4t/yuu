#include "scene.hpp"

static target_t create_target(std::vector<texture_ref_t> output);

scene_t::scene_t(scene_file_t& scene_file)
  : m_ubo(0, {
    ubo_t::field_t(ubo_t::VEC2, "u_resolution"),
    ubo_t::field_t(ubo_t::FLOAT, "u_time"),
    ubo_t::field_t(ubo_t::FLOAT, "u_frame")
  }),
  m_time(0.0) {
  m_passes.reserve(16);
  
  m_width = scene_file.get_width();
  m_height = scene_file.get_height();
  
  for (auto& buffer : scene_file.get_buffers()) {
    add_buffer(
      buffer.get_name(),
      buffer.get_width(),
      buffer.get_height()
    );
  }
  
  for (auto& image : scene_file.get_images()) {
    add_image(
      image.get_name(),
      image.get_src()
    );
  }
  
  for (auto& buffer : scene_file.get_buffers()) {
    add_buffer(
      buffer.get_name(),
      buffer.get_width(),
      buffer.get_height()
    );
  }
  
  for (auto& shader : scene_file.get_shaders()) {
    add_shader(
      shader.get_name(),
      shader.get_src(),
      shader.get_channels()
    );
  }
  
  for (auto& pass : scene_file.get_renderer()) {
    add_pass(
      pass.get_shader(),
      pass.get_input(),
      pass.get_output()
    );
  }
}

void scene_t::render() {
  m_time += 0.015;
  m_ubo.sub("u_time", &m_time, 0, sizeof(m_time));
  
  for (auto& pass : m_passes) {
    pass.bind(m_ubo);
    m_mesh.draw();
  }
}

void scene_t::add_pass(std::string shader, std::vector<std::string> input, std::vector<std::string> output) {
  std::vector<texture_ref_t> bindings;
  for (auto& name : input) {
    bindings.push_back(m_textures.at(name));
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

void scene_t::add_buffer(std::string name, int width, int height) {
  m_textures.try_emplace(name, width, height, GL_RGBA, GL_RGBA32F, GL_FLOAT);
}

void scene_t::add_image(std::string name, std::string src) {
  m_textures.try_emplace(name, src.c_str());
}

void scene_t::add_shader(std::string name, std::string src, std::vector<std::string> channels) {
  std::stringstream src_vertex, src_fragment;
  src_vertex << R"(
layout (location = 0) in vec2 v_pos;
void main() { gl_Position = vec4(v_pos, 0.0, 1.0); }
  )";
  src_fragment << m_ubo.get_definition() << std::endl;
  src_fragment << shader_read_source(src.c_str()).rdbuf() << std::endl;
  m_shaders.try_emplace(name, src_vertex, src_fragment);
  shader_t& shader = m_shaders.at(name);
  m_ubo.attach_shader(shader);
  
  for (long unsigned int i = 0; i < channels.size(); i++) {
    shader.uniform_int(channels[i].c_str(), i);
  }
}

scene_t::pass_t::pass_t(shader_t& shader, std::vector<texture_ref_t> input, std::vector<texture_ref_t> output, int width, int height)
  : m_shader(shader),
    m_bindings(input),
    m_target(create_target(output)),
    m_width(width),
    m_height(height) {}

target_t create_target(std::vector<texture_ref_t> output) {
  std::vector<binding_t> bindings;
  
  for (unsigned long int i = 0; i < output.size(); i++) {
    bindings.push_back(binding_t(GL_COLOR_ATTACHMENT0 + i, output[i]));
  }
  
  return target_t(bindings);
}

void scene_t::pass_t::bind(ubo_t& ubo) {
  glViewport(0, 0, m_width, m_height);
  float resolution[] = { m_width, m_height };
  ubo.sub("u_resolution", resolution, 0, sizeof(resolution));
  
  m_shader.bind();
  for (unsigned long int i = 0; i < m_bindings.size(); i++) {
    m_bindings[i].get().bind(i);
  }
  m_target.bind();
}
