#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <glad/glad.h>
#include <vector>
#include "bindable.hpp"

class cubemap_t : public bindable_t {
private:
  GLuint m_cubemap;

public:
  cubemap_t(const char *src);
  ~cubemap_t();
  void bind(int channel) override;
};

using cubemap_ref_t = std::reference_wrapper<cubemap_t>;

#endif
