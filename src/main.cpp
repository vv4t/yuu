#include <iostream>
#include <glad/glad.h>
#include <vector>
#include "input.h"
#include "window.h"
#include "shader.h"
#include "vertex.h"
#include "target.h"
#include "camera.h"
#include "texture.h"
#include "vertex_buffer.h"
#include "uniform_buffer.h"
#include "scene.h"

#define WIDTH 800
#define HEIGHT 600

int main() {
  input_t input;
  window_t window(WIDTH, HEIGHT, "nui", input);
  
  vertex_buffer_t vertex_buffer(256);
  vertex_buffer.bind();
  
  scene_t scene;
  scene.add_image("test", "assets/1.jpg");
  scene.add_shader({ "first" }, "forest", "assets/test.glsl");
  scene.add_pass({ "test" }, "forest");
  scene.load(input, vertex_buffer);
  
  while (window.poll()) {
    glClear(GL_COLOR_BUFFER_BIT);
    scene.render(input);
    window.swap();
  }
  
  return 0;
}
