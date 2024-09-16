#ifndef SHADER_SCENE_H
#define SHADER_SCENE_H

#include "scene.h"
#include "shader.h"
#include "uniform_buffer.h"

class shader_scene_t : public scene_t {
private:
  shader_t m_shader;
  mesh_t m_quad;
  uniform_buffer_t m_input_buffer;
  float m_time;

public:
  shader_scene_t(const char* src);
  
  void load(input_t& input, vertex_buffer_t& vertex_buffer);
  void render(const input_t& input) override;
};

#endif
