#ifndef SCENE_HPP
#define SCENE_HPP

#include "scene_file.hpp"
#include "texture.hpp"
#include "target.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include <vector>

class scene_t {
public:
  class pass_t {
  private:
    shader_t& m_shader;
    std::vector<texture_ref_t> m_bindings;
    target_t m_target;
    
  public:
    pass_t(shader_t& shader, std::vector<texture_ref_t> input, std::vector<texture_ref_t> output);
    void bind();
  };
  
private:
  std::map<std::string, texture_t> m_textures;
  std::map<std::string, shader_t> m_shaders;
  std::vector<pass_t> m_passes;
  mesh_t mesh;

public:
  scene_t(scene_file_t& file);
  
  void render();
  
  void add_buffer(std::string name, int width, int height);
  void add_shader(std::string name, std::string src, std::vector<std::string> channels);
  void add_pass(std::string shader, std::vector<std::string> input, std::vector<std::string> output);
};

#endif
