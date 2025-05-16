#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <vector>
#include "bindable.hpp"

class texture_t : public bindable_t {
private:
  GLuint m_texture;
  GLuint m_type;
  int m_width;
  int m_height;

public:
  texture_t(const char *src);
  texture_t(int width, int height, GLuint format, GLuint internalformat, GLuint type, std::vector<unsigned int> data);
  texture_t(int width, int height, GLuint format, GLuint internalformat, GLuint type);
  ~texture_t();
  void bind(int channel) override;
  GLuint get_texture() const;
  int get_width() { return m_width; }
  int get_height() { return m_height; }
};

using texture_ref_t = std::reference_wrapper<texture_t>;

#endif
