#include "ubo_input.h"
#include <iostream>

ubo_input_t::ubo_input_t(int binding)
  : m_binding(binding) {
  glGenBuffers(1, &m_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(m_data), NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ubo);
}

void ubo_input_t::on_key_press(int key, bool action) {
  
}

void ubo_input_t::on_mouse_move(float x, float y) {
  m_data.mouse_x = x;
  m_data.mouse_y = y;
}

void ubo_input_t::set_viewport(int width, int height) {
  m_data.width = width;
  m_data.height = height;
}

void ubo_input_t::update() {
  m_data.time += 0.015;
  glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(m_data), &m_data);
}

void ubo_input_t::attach_shader(const shader_t& shader) {
  shader.bind();
  GLuint location = glGetUniformBlockIndex(shader.get_program(), "ubo_input");
  glUniformBlockBinding(shader.get_program(), location, m_binding);
}

ubo_input_t::~ubo_input_t() {
  glDeleteBuffers(1, &m_ubo);
}
