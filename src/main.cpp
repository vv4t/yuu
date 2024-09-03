#include <iostream>
#include "window.h"
#include <glad/glad.h>

int main() {
  window_t::init();
  window_t window(800, 600, "yuu");
  
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  
  while (window.poll()) {
    glClear(GL_COLOR_BUFFER_BIT);
    window.swap();
  }
  
  return 0;
}
