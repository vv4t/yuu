#ifndef SCENE_H
#define SCENE_H

#include "target.h"
#include "input.h"
#include "ubo_input.h"
#include "texture.h"
#include "shader.h"
#include "quad_mesh.h"
#include <map>

class scene_t {
public:
  scene_t(int width, int height, ubo_input_t& ubo_input);
  void shader_add(std::string name, std::vector<std::string> channels, std::string src);
  void image_add(std::string name, std::string src);
  void buffer_add(std::string name, int width, int height);
  void pass_add(std::vector<std::string> input, std::string shader, std::vector<std::string> output);
  void render(quad_mesh_t& mesh);

private:
  class pass_t {
  private:
    shader_t& m_shader;
    std::vector<texture_ref_t> m_input;
    target_t m_target;
    int m_width;
    int m_height;

  public:
    inline pass_t(
      std::vector<texture_ref_t> input,
      shader_t& shader,
      std::vector<target_t::binding_t> output,
      int width,
      int height
    ) : m_shader(shader),
        m_input(input),
        m_target(output),
        m_width(width),
        m_height(height) {}
    
    void begin(ubo_input_t& input);
    void end();
  };
  
  ubo_input_t& m_input;
  std::map<std::string, shader_t> m_shaders;
  std::map<std::string, texture_t> m_textures;
  std::vector<pass_t> m_passes;
  int m_width;
  int m_height;
  int m_frame;
};

#endif
