#include "shader_scene.h"
#include <fstream>
#include <sstream>

shader_scene_t::shader_scene_t(const char* src)
  : m_shader([](const char *src) {
    std::stringstream src_vertex, src_fragment;
    src_vertex << R"(
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_uv;

out vec2 vs_uv;

void main() {
  vs_uv = v_uv;
  gl_Position = vec4(v_pos, 1.0);
}
    )";
    src_fragment << shader_read_source(src).rdbuf();
    return shader_t(src_vertex, src_fragment);
  }(src)) {
  
}

void shader_scene_t::load(vertex_buffer_t& vertex_buffer) {
  m_quad = vertex_buffer.push(
    std::vector<vertex_t> {
      vertex_t(vec3(+1, +1, 0), vec2(1, 1)),
      vertex_t(vec3(-1, +1, 0), vec2(0, 1)),
      vertex_t(vec3(-1, -1, 0), vec2(0, 0)),
      vertex_t(vec3(+1, -1, 0), vec2(1, 0)),
      vertex_t(vec3(+1, +1, 0), vec2(1, 1)),
      vertex_t(vec3(-1, -1, 0), vec2(0, 0))
    }
  );
}

void shader_scene_t::render() {
  m_shader.bind();
  m_quad.draw();
}
