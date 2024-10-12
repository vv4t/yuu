#include "scene.h"
#include <fstream>
#include <sstream>
#include <iostream>

scene_t::scene_t(int width, int height, ubo_input_t& ubo_input)
  : m_input(ubo_input), m_width(width), m_height(height) {
  m_passes.reserve(64);
}

void scene_t::render(quad_mesh_t& mesh) {
  for (pass_t& pass : m_passes) {
    pass.begin(m_input);
    mesh.draw();
    pass.end();
  }
}

void scene_t::shader_add(std::string name, std::vector<std::string> channels, std::string src) {
  std::stringstream src_vertex, src_fragment;
  src_vertex << R"(
layout (location = 0) in vec2 v_pos;
layout (location = 1) in vec2 v_uv;

out vec2 frag_coord;

void main() {
  frag_coord = v_uv;
  gl_Position = vec4(v_pos, 0.0, 1.0);
}
  )";
  src_fragment << "layout (std140) uniform ubo_input { vec2 resolution; vec2 mouse; float time; };" << std::endl;
  src_fragment << shader_read_source(src.c_str()).rdbuf() << std::endl;
  m_shaders.try_emplace(name, src_vertex, src_fragment);
  
  shader_t& shader = m_shaders.at(name);
  m_input.attach_shader(shader);
  shader.bind();
  for (std::size_t i = 0; i < channels.size(); i++) {
    GLuint location = shader.get_uniform_location(channels[i].c_str());
    glUniform1i(location, i);
  }
}

void scene_t::image_add(std::string name, std::string src) {
  m_textures.try_emplace(name, src.c_str());
}

void scene_t::buffer_add(std::string name, int width, int height) {
  m_textures.try_emplace(name, width, height, GL_RGBA, GL_RGBA16F, GL_FLOAT);
}

void scene_t::pass_add(std::vector<std::string> input, std::string shader, std::vector<std::string> output) {
  std::vector<texture_ref_t> textures;
  for (std::string name : input) {
    textures.push_back(m_textures.at(name));
  }
  
  int width = m_width;
  int height = m_height;
  
  std::vector<target_t::binding_t> bindings;
  for (std::size_t i = 0; i < output.size(); i++) {
    texture_t& texture = m_textures.at(output[i]);
    width = std::min(width, texture.get_width());
    height = std::min(height, texture.get_height());
    bindings.push_back(target_t::binding_t(GL_COLOR_ATTACHMENT0 + i, texture));
  }
  
  m_passes.emplace_back(textures, m_shaders.at(shader), bindings, width, height);
}

void scene_t::pass_t::begin(ubo_input_t& input) {
  glViewport(0, 0, m_width, m_height);
  input.set_viewport(m_width, m_height);
  input.update();
  for (std::size_t i = 0; i < m_input.size(); i++) {
    m_input[i].get().bind(i);
  }
  m_shader.bind();
  m_target.bind();
}

void scene_t::pass_t::end() {
  m_target.unbind();
}
