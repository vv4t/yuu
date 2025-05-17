#include "ubo.hpp"
#include <iostream>

static struct {
  const char* name;
  int size;
} shader_type_data[] = {
  { "float", 1 },
  { "vec2", 2 },
  { "vec3", 3 },
  { "vec4", 4 }
};

ubo_t::ubo_t(int binding, std::vector<field_t> fields) : m_binding(binding) {
  int top = 0;
  m_definition += "layout (std140) uniform ubo { ";
  for (auto field : fields) {
    m_definition += field.get_definition();
    m_mapping[std::hash<std::string>{}(field.get_name())] = top * 4;
    top += shader_type_data[field.get_type()].size;
  }
  m_definition += "};";
  
  glGenBuffers(1, &m_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
  glBufferData(GL_UNIFORM_BUFFER, top * 4, NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ubo);
}

ubo_t::~ubo_t() {
  glDeleteBuffers(1, &m_ubo);
}

std::string ubo_t::get_definition() {
  return m_definition;
}

std::string ubo_t::field_t::get_definition() {
  return std::string(shader_type_data[m_type].name) + " " + std::string(m_name) + "; ";
}

void ubo_t::sub(std::string name, void* data, int offset, int size) {
  std::size_t key = std::hash<std::string>{}(name);
  glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, m_mapping[key] + offset, size, data); 
}

void ubo_t::attach_shader(shader_t& shader) {
  shader.bind();
  GLuint location = glGetUniformBlockIndex(shader.get_program(), "ubo");
  glUniformBlockBinding(shader.get_program(), location, m_binding);
}
