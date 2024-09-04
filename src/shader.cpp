#include "shader.h"
#include <iostream>

GLuint shader_compile(GLuint type, const char *src);

shader_t::shader_t(std::stringstream& src_vertex, std::stringstream& src_fragment) {
  GLuint vertex_shader = shader_compile(GL_VERTEX_SHADER, src_vertex.str().c_str());
  GLuint fragment_shader = shader_compile(GL_FRAGMENT_SHADER, src_fragment.str().c_str());
  
  m_program = glCreateProgram();
  
  glAttachShader(m_program, vertex_shader);
  glAttachShader(m_program, fragment_shader);
  
  int success;
  glLinkProgram(m_program);
  glGetProgramiv(m_program, GL_LINK_STATUS, &success);
  
  if (!success) {
    static char info[1024];
    glGetProgramInfoLog(m_program, sizeof(info), NULL, info);
    std::cerr << "error: shader_compile: " << info;
    throw std::runtime_error("Failed to link shader.");
  }
  
  glDetachShader(m_program, vertex_shader);
  glDetachShader(m_program, vertex_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

void shader_t::bind() {
  glUseProgram(m_program);
}

GLuint shader_compile(GLuint type, const char *src) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, NULL);
  
  int success;
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  
  if (!success) {
    static GLchar info[1024];
    glGetShaderInfoLog(shader, sizeof(info), NULL, info);
    std::cerr << "error: shader_compile: " << info;
    throw std::runtime_error("Failed to compile shader.");
  }
  
  return shader;
}
