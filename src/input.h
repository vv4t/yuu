#ifndef INPUT_H
#define INPUT_H

#include <utility>

class input_t {
public:
  input_t() {}
  virtual void on_key_press(int key, bool action) = 0;
  virtual void on_mouse_move(float x, float y) = 0;
};

using input_ref_t = std::reference_wrapper<input_t>;

#endif
