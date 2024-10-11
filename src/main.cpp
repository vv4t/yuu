#include <iostream>
#include <glad/glad.h>
#include <vector>
#include "input.h"
#include "window.h"
#include "shader.h"
#include "target.h"
#include "texture.h"
#include "uniform_buffer.h"
#include "scene.h"
#include <yaml/Yaml.hpp>

#define WIDTH 800
#define HEIGHT 600

int main() {
  Yaml::Node root;
  Yaml::Parse(root, "assets/scene.yml");
  
  input_t input;
  window_t window(WIDTH, HEIGHT, "nui", input);
  quad_mesh_t quad_mesh;
  
  scene_t scene(
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
  
  quad_mesh.bind();
  
  while (window.poll()) {
    glClear(GL_COLOR_BUFFER_BIT);
    scene.render(quad_mesh, input);
    window.swap();
  }
  
  return 0;
}
