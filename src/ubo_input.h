#ifndef UBO_BUFFER_H
#define UBO_BUFFER_H

#include <glad/glad.h>
#include "input.h"
#include "shader.h"

class ubo_input_t : public input_t {
private:
  struct {
    float mouse_x;
    float mouse_y;
    float time;
  } m_data;
  GLuint m_ubo;
  int m_binding;

public:
  ubo_input_t(int binding);
  ~ubo_input_t();
  void update();
  void on_key_press(int key, bool action);
  void on_mouse_move(float x, float y);
  void attach_shader(const shader_t& shader);
};

#endif
