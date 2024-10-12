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
  class image_data_t {
  public:
    std::string name;
    std::string src;
    inline image_data_t(std::string _name, std::string _src)
      : name(_name), src(_src) {}
  };

  class buffer_data_t {
  public:
    std::string name;
    int width;
    int height;
    inline buffer_data_t(std::string _name, int _width, int _height)
      : name(_name), width(_width), height(_height) {}
  };

  class shader_data_t {
  public:
    std::string name;
    std::vector<std::string> channels;
    std::string src;
    
    inline shader_data_t(std::string _name, std::vector<std::string> _channels, std::string _src)
      : name(_name), channels(_channels), src(_src) {}
      
  };

  class pass_data_t {
  public:
    std::vector<std::string> input;
    std::string shader;
    std::vector<std::string> output;
    
    pass_data_t(std::vector<std::string> _input, std::string _shader, std::vector<std::string> _output)
      : input(_input), shader(_shader), output(_output) {}
  };

  scene_t(
    int width,
    int height,
    ubo_input_t& ubo_input,
    std::vector<image_data_t> images,
    std::vector<buffer_data_t> buffers,
    std::vector<shader_data_t> shaders,
    std::vector<pass_data_t> passes
  );
  
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
    inline pass_t(std::vector<texture_ref_t> input, shader_t& shader, std::vector<target_t::binding_t> output, int width, int height)
      : m_shader(shader), m_input(input), m_target(output), m_width(width), m_height(height) {}
    
    void begin(ubo_input_t& input);
    void end();
  };
  
  ubo_input_t& m_input;
  std::map<std::string, shader_t> m_shaders;
  std::map<std::string, texture_t> m_textures;
  std::vector<pass_t> m_passes;
  int m_width;
  int m_height;
  
  void shader_add(shader_data_t data);
  void image_add(image_data_t data);
  void buffer_add(buffer_data_t data);
  void pass_add(pass_data_t data);
};

#endif
