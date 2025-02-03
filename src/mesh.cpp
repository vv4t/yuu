#include "mesh.hpp"

mesh_t::mesh_t() {
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  
  float vertices[] = {
    +1, +1, 1, 1,
    -1, +1, 0, 1,
    -1, -1, 0, 0,
    +1, -1, 1, 0,
    +1, +1, 1, 1,
    -1, -1, 0, 0
  };
  
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), vertices, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (float*) 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (float*) 0 + 2);
}

void mesh_t::draw() {
  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

mesh_t::~mesh_t() {
  glDeleteVertexArrays(1, &m_vao);
  glDeleteBuffers(1, &m_vbo);
}
