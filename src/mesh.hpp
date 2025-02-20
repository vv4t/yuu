#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>

class mesh_t {
private:
  GLuint m_vao;
  GLuint m_vbo;

public:
  mesh_t();
  ~mesh_t();
  void draw();
};

#endif
