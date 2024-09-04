#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <vector>
#include "window.h"
#include "shader.h"
#include "vertex.h"
#include "vertex_buffer.h"

int main() {
  window_t::init();
  window_t window(800, 600, "yuu");
  
  vertex_buffer_t vertex_buffer(1024);
  
  std::vector<vertex_t> vertices;
  vertices.push_back(vertex_t(vec3(0, 0, 0), vec2(0, 0)));
  vertices.push_back(vertex_t(vec3(0, 1, 0), vec2(0, 1)));
  vertices.push_back(vertex_t(vec3(1, 1, 0), vec2(1, 1)));
  mesh_t mesh = vertex_buffer.push(vertices);
  
  std::stringstream src_vertex, src_fragment;
  src_vertex << std::ifstream("assets/shader.vert").rdbuf();
  src_fragment << std::ifstream("assets/shader.frag").rdbuf();
  shader_t shader(src_vertex, src_fragment);
  
  while (window.poll()) {
    glClear(GL_COLOR_BUFFER_BIT);
    mesh.draw();
    window.swap();
  }
  
  return 0;
}
