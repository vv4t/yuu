#include "scene.hpp"

static target_t create_target(std::vector<texture_ref_t> output);

scene_t::scene_t(scene_file_t& scene_file) {
  m_passes.reserve(16);
  
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
  for (auto& pass : m_passes) {
    pass.bind();
    mesh.draw();
  }
}

void scene_t::add_pass(std::string shader, std::vector<std::string> input, std::vector<std::string> output) {
  std::vector<texture_ref_t> bindings;
  for (auto& name : input) {
    bindings.push_back(m_textures.at(name));
  }
  
  std::vector<texture_ref_t> target;
  for (auto& name : output) {
    target.push_back(m_textures.at(name));
  }
  
  m_passes.emplace_back(m_shaders.at(shader), bindings, target);
}

void scene_t::add_buffer(std::string name, int width, int height) {
  m_textures.try_emplace(name, width, height, GL_RGBA, GL_RGBA32F, GL_FLOAT);
}

void scene_t::add_shader(std::string name, std::string src, std::vector<std::string> channels) {
  std::stringstream src_vertex, src_fragment;
  src_vertex << R"(
layout (location = 0) in vec2 v_pos;
void main() { gl_Position = vec4(v_pos, 0.0, 1.0); }
  )";
  src_fragment << shader_read_source(src.c_str()).rdbuf() << std::endl;
  m_shaders.try_emplace(name, src_vertex, src_fragment);
  shader_t& shader = m_shaders.at(name);
  
  for (long unsigned int i = 0; i < channels.size(); i++) {
    shader.uniform_int(channels[i].c_str(), i);
  }
}

scene_t::pass_t::pass_t(shader_t& shader, std::vector<texture_ref_t> input, std::vector<texture_ref_t> output)
  : m_shader(shader),
    m_bindings(input),
    m_target(create_target(output)) {}

target_t create_target(std::vector<texture_ref_t> output) {
  std::vector<binding_t> bindings;
  
  for (unsigned long int i = 0; i < output.size(); i++) {
    bindings.push_back(binding_t(GL_COLOR_ATTACHMENT0 + i, output[i]));
  }
  
  return target_t(bindings);
}

void scene_t::pass_t::bind() {
  m_shader.bind();
  for (unsigned long int i = 0; i < m_bindings.size(); i++) {
    m_bindings[i].get().bind(i);
  }
  m_target.bind();
}
