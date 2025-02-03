#include <iostream>
#include <glad/glad.h>
#include "scene_file.hpp"
#include "scene.hpp"
#include "input.hpp"
#include "window.hpp"

int main(int argc, const char *argv[]) {
  std::string src(argv[1]);
  scene_file_t scene_file(src);
  
  input_t input;
  window_t window(800, 600, "yuu", input);
  
  scene_t scene(scene_file);
  
  int old_time = window.get_time();
  int lag_time = 0;
  
  while (window.poll()) {
    if (lag_time > 0) {
      lag_time -= 15;
      scene.render();
      window.swap();
    }

    int now_time = window.get_time();
    lag_time += now_time - old_time;
    old_time = now_time;
  }
  
  return 0;
}
