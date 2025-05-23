#ifndef INPUT_H
#define INPUT_H

#include <array>

class input_t {
private:
  std::array<float, 8> m_axis;
  int m_size;
  
  int m_move1;
  int m_move2;
  
  std::array<int, 4> m_buttons;
  std::array<std::pair<int, int>, 16> m_keys;
  int m_num_keys;

public:
  input_t();
  int size();
  float get_axis(int axis);
  void bind_key(int axis, int key);
  void bind_button(int axis, int button);
  void bind_move(int axis1, int axis2);
  void bind_move1(int axis);
  void bind_move2(int axis);
  void key_event(int key, bool action);
  void button_event(int key, bool action);
  void move_event(float x, float y);
};

#endif
