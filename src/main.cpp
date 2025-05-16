#include <iostream>
#include <glad/glad.h>
#include "scene_file.hpp"
#include "scene.hpp"
#include "input.hpp"
#include "window.hpp"

int main(int argc, const char *argv[]) {
  std::string src("scene.yml");

  if (argc == 2) {
    src = std::string(argv[1]);
  } else if (argc != 1) {
    std::cerr << "usage: " << argv[0] << " [scene]" << std::endl;
    return 1;
  }

  scene_file_t scene_file(src);
  
  if (!scene_file.validate()) {
    return 1;
  }
  
  input_t input;
  window_t window(scene_file.get_width(), scene_file.get_height(), "yuu", input);
  scene_t scene(input);
  scene.load_from_file(scene_file);
  
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
