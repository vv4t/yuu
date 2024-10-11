#include "scene.h"
#include <fstream>
#include <sstream>
#include <iostream>

struct ubo_input {
  vec2 mouse;
  float time;
};

scene_t::scene_t(
  std::vector<image_data_t> images,
  std::vector<buffer_data_t> buffers,
  std::vector<shader_data_t> shaders,
  std::vector<pass_data_t> passes
) : m_input_buffer(0, "ubo_input", sizeof(struct ubo_input)),
    m_time(0.0) {
  m_passes.reserve(passes.size());
  
  for (image_data_t image : images) {
    add_image(image);
  }
  
  for (buffer_data_t buffer : buffers) {
    add_buffer(buffer);
  }
  
  for (shader_data_t shader : shaders) {
    add_shader(shader);
  }
  
  for (pass_data_t pass : passes) {
    add_pass(pass);
  }
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

void scene_t::add_shader(shader_data_t data) {
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
  src_fragment << shader_read_source(data.src).rdbuf();
  m_shaders.try_emplace(data.name, src_vertex, src_fragment);
  
  shader_t& shader = m_shaders.at(data.name);
  shader.bind();
  for (std::size_t i = 0; i < data.channels.size(); i++) {
    GLuint location = shader.get_uniform_location(data.channels[i]);
    glUniform1i(location, i);
  }
  
}

void scene_t::add_image(image_data_t data) {
  m_textures.try_emplace(data.name, data.src);
}

void scene_t::add_buffer(buffer_data_t data) {
  m_textures.try_emplace(data.name, data.width, data.height, GL_RGBA, GL_RGBA16F, GL_FLOAT);
}

void scene_t::add_pass(pass_data_t data) {
  std::vector<texture_ref_t> textures;
  for (const char* name : data.input) {
    textures.push_back(m_textures.at(name));
  }
  
  std::vector<binding_t> bindings;
  for (std::size_t i = 0; i < data.output.size(); i++) {
    bindings.push_back(binding_t(GL_COLOR_ATTACHMENT0 + i, m_textures.at(data.output[i])));
  }
  
  m_passes.emplace_back(textures, m_shaders.at(data.shader), bindings);
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
