#include <iostream>
#include <glad/glad.h>
#include <vector>
#include "window.h"
#include "ubo_input.h"
#include "scene.h"
#include <yaml/Yaml.hpp>

#define WIDTH 800
#define HEIGHT 600

int main() {
  Yaml::Node root;
  Yaml::Parse(root, "assets/scene.yml");
  
  window_t window(WIDTH, HEIGHT, "nui");
  ubo_input_t ubo_input(0);
  quad_mesh_t mesh;
  
  window.add_input(ubo_input);
  
  scene_t scene(
    ubo_input,
    {
      scene_t::image_data_t("test", "assets/1.jpg"),
      scene_t::image_data_t("other", "assets/2.jpg")
    }, {
      scene_t::buffer_data_t("buffer", 640, 480)
    }, {
      scene_t::shader_data_t("test", { "first", "second" }, "assets/test.glsl")
    }, {
      scene_t::pass_data_t({ "test", "other" }, "test", { "buffer" }),
      scene_t::pass_data_t({ "buffer", "other" }, "test", {})
    }
  );
  
  while (window.poll()) {
    glClear(GL_COLOR_BUFFER_BIT);
    scene.render(mesh);
    window.swap();
  }
  
  return 0;
}
