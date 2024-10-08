#include "pass.h"

pass_t::pass_t(std::vector<texture_ref_t> input, shader_t& shader, std::vector<binding_t> output)
  : m_shader(shader),
    m_input(input),
    m_target(output) {
  
}

void pass_t::begin() {
  for (std::size_t i = 0; i < m_input.size(); i++) {
    m_input[i].get().bind(i);
  }
  m_shader.bind();
  m_target.bind();
}

void pass_t::end() {
  m_target.unbind();
}
