#include "scene.h"
#include <fstream>
#include <sstream>
#include <iostream>

struct ubo_input {
  vec2 mouse;
  float time;
};

scene_t::scene_t()
  : m_input_buffer(0, "ubo_input", sizeof(struct ubo_input)),
    m_time(0.0) {
  m_passes.reserve(16);
}

void scene_t::add_shader(std::vector<const char*> channels, const char *name, const char *src) {
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
  
  m_shaders.try_emplace(name, src_vertex, src_fragment);
  
  shader_t& shader = m_shaders.at(name);
  shader.bind();
  for (std::size_t i = 0; i < channels.size(); i++) {
    GLuint location = shader.get_uniform_location(channels[i]);
    glUniform1i(location, i);
  }
  
}

void scene_t::add_image(const char* name, const char* src) {
  m_textures.try_emplace(name, src);
}

void scene_t::add_buffer(const char *name, int width, int height) {
  m_textures.try_emplace(name, width, height, GL_RGBA, GL_RGBA16F, GL_FLOAT);
}

void scene_t::add_pass(std::vector<const char*> input, const char *shader, std::vector<const char*> output) {
  std::vector<texture_ref_t> textures;
  for (const char* name : input) {
    textures.push_back(m_textures.at(name));
  }
  
  std::vector<binding_t> bindings;
  for (std::size_t i = 0; i < output.size(); i++) {
    bindings.push_back(binding_t(GL_COLOR_ATTACHMENT0 + i, m_textures.at(output[i])));
  }
  
  m_passes.emplace_back(textures, m_shaders.at(shader), bindings);
}

void scene_t::load(input_t& input, vertex_buffer_t& vertex_buffer) {
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

void scene_t::render(const input_t& input) {
  m_time += 0.015;
  
  struct ubo_input data;
  data.mouse = (vec2(input.get_axis(0), input.get_axis(1)) * 2.0 - 1.0) * mat2(vec2(800.0 / 600.0, 0), vec2(0, 1));
  data.time = m_time;
  m_input_buffer.sub(&data, 0, sizeof(data));
  
  for (pass_t& pass : m_passes) {
    pass.begin();
    m_quad.draw();
    pass.end();
  }
}
