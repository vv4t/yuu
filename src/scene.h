#ifndef SCENE_H
#define SCENE_H

#include "input.h"
#include "vertex_buffer.h"
#include "uniform_buffer.h"
#include "texture.h"
#include "shader.h"
#include "pass.h"
#include <map>

class scene_t {
private:
  mesh_t m_quad;
  uniform_buffer_t m_input_buffer;
  std::map<const char*, shader_t> m_shaders;
  std::map<const char*, texture_t> m_textures;
  std::vector<pass_t> m_passes;
  float m_time;

public:
  scene_t();
  void add_shader(std::vector<const char*> channels, const char *name, const char *src);
  void add_image(const char *name, const char *src);
  void add_pass(std::vector<const char*> input, const char *shader);
  void load(input_t& input, vertex_buffer_t& vertex_buffer);
  void render(const input_t& input);
};

#endif
