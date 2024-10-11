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
public:
  class image_data_t {
  public:
    const char *name;
    const char *src;
    inline image_data_t(const char *_name, const char *_src)
      : name(_name), src(_src) {}
  };

  class buffer_data_t {
  public:
    const char *name;
    int width;
    int height;
    inline buffer_data_t(const char* _name, int _width, int _height)
      : name(_name), width(_width), height(_height) {}
  };

  class shader_data_t {
  public:
    const char *name;
    std::vector<const char*> channels;
    const char *src;
    
    inline shader_data_t(const char *_name, std::vector<const char*> _channels, const char *_src)
      : name(_name), channels(_channels), src(_src) {}
      
  };

  class pass_data_t {
  public:
    std::vector<const char*> input;
    const char *shader;
    std::vector<const char*> output;
    pass_data_t(std::vector<const char*> _input, const char *_shader, std::vector<const char*> _output)
      : input(_input), shader(_shader), output(_output) {}
  };

  scene_t(
    std::vector<image_data_t> images,
    std::vector<buffer_data_t> buffers,
    std::vector<shader_data_t> shaders,
    std::vector<pass_data_t> passes
  );
  void load(input_t& input, vertex_buffer_t& vertex_buffer);
  void render(const input_t& input);

private:
  mesh_t m_quad;
  uniform_buffer_t m_input_buffer;
  std::map<const char*, shader_t> m_shaders;
  std::map<const char*, texture_t> m_textures;
  std::vector<pass_t> m_passes;
  float m_time;
  
  void add_shader(shader_data_t data);
  void add_image(image_data_t data);
  void add_buffer(buffer_data_t data);
  void add_pass(pass_data_t data);
};

#endif
