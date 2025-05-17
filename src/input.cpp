#include "input.hpp"
#include <iostream>

input_t::input_t() {
  std::fill(std::begin(m_axis), std::end(m_axis), 0.0);
  std::fill(std::begin(m_keys), std::end(m_keys), std::pair<int, int>(0, -1));
  std::fill(std::begin(m_buttons), std::end(m_buttons), -1);
  m_size = 0;
  m_num_keys = 0;
  m_move1 = -1;
  m_move2 = -1;
}

void input_t::bind_key(int axis, int key) {
  m_size = std::max(m_size, axis);
  m_keys[m_num_keys++] = std::pair<int, int>(axis, key);
}

void input_t::bind_move(int axis1, int axis2) {
  m_size = std::max(m_size, axis1);
  m_size = std::max(m_size, axis2);
  m_move1 = axis1;
  m_move2 = axis2;
}

void input_t::bind_move1(int axis) {
  m_size = std::max(m_size, axis);
  m_move1 = axis;
}

void input_t::bind_move2(int axis) {
  m_size = std::max(m_size, axis);
  m_move2 = axis;
}

void input_t::bind_button(int axis, int button) {
  m_size = std::max(m_size, axis);
  m_buttons[button] = axis;
}

void input_t::key_event(int key, bool action) {
  for (int i = 0; i < m_num_keys; i++) {
    if (m_keys[i].second == key) {
      m_axis[m_keys[i].first] = action ? 1.0 : 0.0;
      break;
    }
  }
}

void input_t::button_event(int button, bool action) {
  if (m_buttons[button] >= 0) {
    m_axis[m_buttons[button]] = action ? 1.0 : 0.0;
  }
}

void input_t::move_event(float x, float y) {
  if (m_move1 >= 0) {
    m_axis[m_move1] = x;
  }
  
  if (m_move2 >= 0) {
    m_axis[m_move2] = y;
  }
}

float input_t::get_axis(int axis) {
  return m_axis[axis];
}

int input_t::size() {
  return m_size + 1;
}
