#ifndef TARGET_H
#define TARGET_H

#include <glad/glad.h>
#include <vector>
#include <utility>
#include "texture.h"

class target_t {
private:
  GLuint m_framebuffer;

public:
  class binding_t {
  public:
    GLuint attachment;
    const texture_t& texture;
    
    inline binding_t(GLuint attachment_, const texture_t& texture_)
      : attachment(attachment_), texture(texture_) {}
  };
  
  target_t(std::vector<binding_t> bindings);
  ~target_t();
  void bind();
  void unbind();
};

#endif
