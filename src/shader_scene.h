#ifndef SHADER_SCENE_H
#define SHADER_SCENE_H

#include "scene.h"
#include "shader.h"

class shader_scene_t : public scene_t {
private:
  shader_t m_shader;
  mesh_t m_quad;

public:
  shader_scene_t(const char* src);
  
  void load(vertex_buffer_t& vertex_buffer);
  void render() override; 
};

#endif
