#ifndef SCENE_H
#define SCENE_H

#include "vertex_buffer.h"

class scene_t {
public:
  virtual void load(vertex_buffer_t& vertex_buffer) = 0;
  virtual void render() = 0;
};

#endif
