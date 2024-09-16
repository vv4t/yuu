#ifndef SCENE_H
#define SCENE_H

#include "input.h"
#include "vertex_buffer.h"

class scene_t {
public:
  virtual void load(input_t& input, vertex_buffer_t& vertex_buffer) = 0;
  virtual void render(const input_t& input) = 0;
};

#endif
