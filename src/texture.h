#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <functional>

class texture_t {
private:
  GLuint m_texture;
  GLuint m_type;
  int m_width;
  int m_height;

public:
  texture_t(const char *src);
  texture_t(int width, int height, GLuint format, GLuint internalformat, GLuint type);
  ~texture_t();
  void bind(int channel);
  int get_width() const;
  int get_height() const;
  GLuint get_texture() const;
};

using texture_ref_t = std::reference_wrapper<texture_t>;

#endif
