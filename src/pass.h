#ifndef PASS_H
#define PASS_H

#include "shader.h"
#include "texture.h"
#include "target.h"
#include <vector>

class pass_t {
private:
  shader_t& m_shader;
  std::vector<texture_ref_t> m_input;
  target_t m_target;

public:
  pass_t(std::vector<texture_ref_t> input, shader_t& shader, std::vector<binding_t> output);
  void begin();
  void end();
};


#endif
