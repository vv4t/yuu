#ifndef UBO_BUFFER_H
#define UBO_BUFFER_H

#include <glad/glad.h>
#include "input.h"
#include "shader.h"

class ubo_input_t : public input_t {
private:
  struct {
    float width;
    float height;
    float mouse_x;
    float mouse_y;
    float seed[3];
    float time;
    int frame;
  } m_data;
  GLuint m_ubo;
  int m_binding;

public:
  ubo_input_t(int binding);
  ~ubo_input_t();
  void update();
  void randomize();
  void on_key_press(int key, bool action);
  void on_mouse_move(float x, float y);
  void set_viewport(int width, int height);
  void set_frame(int frame);
  void attach_shader(const shader_t& shader);
};

#endif
