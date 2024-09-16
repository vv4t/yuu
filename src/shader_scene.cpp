#include "shader_scene.h"
#include <fstream>
#include <sstream>
#include <iostream>

struct ubo_input {
  vec2 mouse;
  float time;
};

shader_scene_t::shader_scene_t(const char* src)
  : m_shader([](const char *src) {
    std::stringstream src_vertex, src_fragment;
    src_vertex << R"(
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_uv;

out vec2 frag_coord;

void main() {
  frag_coord = (v_uv * 2.0 - 1.0) * mat2(800.0 / 600.0, 0.0, 0.0, 1.0);
  gl_Position = vec4(v_pos, 1.0);
}
    )";
    src_fragment << R"(
layout (std140) uniform ubo_input {
  vec2 mouse;
  float time;
};
    )";
    src_fragment << shader_read_source(src).rdbuf();
    return shader_t(src_vertex, src_fragment);
  }(src)),
  m_input_buffer(0, "ubo_input", sizeof(struct ubo_input)),
  m_time(0.0) {
  
}

void shader_scene_t::load(input_t& input, vertex_buffer_t& vertex_buffer) {
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
  
  input.bind_move(0, 1);
}

void shader_scene_t::render(const input_t& input) {
  m_time += 0.015;
  
  struct ubo_input data;
  data.mouse = (vec2(input.get_axis(0), input.get_axis(1)) * 2.0 - 1.0) * mat2(vec2(800.0 / 600.0, 0), vec2(0, 1));
  data.time = m_time;
  m_input_buffer.sub(&data, 0, sizeof(data));
  
  m_shader.bind();
  m_quad.draw();
}
