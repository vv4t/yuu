#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>

class quad_mesh_t {
private:
  GLuint m_vao;
  GLuint m_vbo;

public:
  quad_mesh_t();
  ~quad_mesh_t();
  void draw();
};

#endif
