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

  public:
    inline pass_t(std::vector<texture_ref_t> input, shader_t& shader, std::vector<target_t::binding_t> output)
      : m_shader(shader), m_input(input), m_target(output) {}
    
    inline void begin() {
      for (std::size_t i = 0; i < m_input.size(); i++) {
        m_input[i].get().bind(i);
      }
      m_shader.bind();
      m_target.bind();
    }
    
    inline void end() {
      m_target.unbind();
    }
  };
  
  std::map<const char*, shader_t> m_shaders;
  std::map<const char*, texture_t> m_textures;
  std::vector<pass_t> m_passes;
  
  void shader_add(shader_data_t data, ubo_input_t& ubo_input);
  void image_add(image_data_t data);
  void buffer_add(buffer_data_t data);
  void pass_add(pass_data_t data);
};

#endif
