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

ubo_t::ubo_t(int binding, std::vector<field_t> fields) {
  int top = 0;
  m_definition += "layout (std140) uniform ubo { ";
  for (auto field : fields) {
    m_definition += field.get_definition();
    m_mapping[field.get_name()] = top * 4;
    top += shader_type_data[field.get_type()].size;
  }
  m_definition += "};";
  
  glGenBuffers(1, &m_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
  glBufferData(GL_UNIFORM_BUFFER, 64, NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ubo);
}

ubo_t::~ubo_t() {
  glDeleteBuffers(1, &m_ubo);
}

std::string ubo_t::get_definition() {
  return m_definition;
}

std::string ubo_t::field_t::get_definition() {
  return std::string(shader_type_data[m_type].name) + " " + m_name + "; ";
}

void ubo_t::sub(std::string name, void* data, int offset, int size) {
  glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(m_data), &m_data); 
}
