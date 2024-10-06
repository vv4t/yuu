#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <functional>

class texture_t {
private:
  GLuint m_texture;
  GLuint m_type;

public:
  texture_t(const char *src);
  texture_t(int width, int height, GLuint format, GLuint internalformat, GLuint type);
  ~texture_t();
  void bind(int channel);
  GLuint get_texture() const;
};

using texture_ref_t = std::reference_wrapper<texture_t>;

#endif
