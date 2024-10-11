#include <iostream>
#include <glad/glad.h>
#include <vector>
#include "window.h"
#include "ubo_input.h"
#include "scene.h"
#include <yaml/Yaml.hpp>

#define WIDTH 800
#define HEIGHT 600

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    std::cerr << "usage: yuu [scene-yml]" << std::endl;
    return 1;
  }
  
  Yaml::Node root;
  Yaml::Parse(root, argv[1]);
  
  if (root["scene"].IsNone()) {
    throw std::runtime_error("missing top level 'scene'");
  }
  
  Yaml::Node& yml_images = root["scene"]["images"];
  Yaml::Node& yml_buffers = root["scene"]["buffers"];
  Yaml::Node& yml_shaders = root["scene"]["shaders"];
  Yaml::Node& yml_passes = root["scene"]["passes"];
  
  std::vector<scene_t::image_data_t> images;
  std::vector<scene_t::buffer_data_t> buffers;
  std::vector<scene_t::shader_data_t> shaders;
  std::vector<scene_t::pass_data_t> passes;
  
  if (!yml_images.IsNone()) {
    for (auto it = yml_images.Begin(); it != yml_images.End(); it++) {
      images.push_back(scene_t::image_data_t((*it).first, (*it).second.As<std::string>()));
    }
  }
  
  if (!yml_buffers.IsNone()) {
    for (auto it = yml_buffers.Begin(); it != yml_buffers.End(); it++) {
      buffers.push_back(scene_t::buffer_data_t((*it).second.As<std::string>(), WIDTH, HEIGHT));
    }
  }
  
  for (auto it = yml_shaders.Begin(); it != yml_shaders.End(); it++) {
    std::string name = (*it).first;
    Yaml::Node& yml_src = (*it).second["src"];
    Yaml::Node& yml_channels = (*it).second["channels"];
    
    std::vector<std::string> channels;
    for (auto channel = yml_channels.Begin(); channel != yml_channels.End(); channel++) {
      channels.push_back((*channel).second.As<std::string>());
    }
    
    shaders.push_back(scene_t::shader_data_t(name, channels, yml_src.As<std::string>()));
  }
  
  for (auto it = yml_passes.Begin();  it != yml_passes.End(); it++) {
    Yaml::Node& yml_shader = (*it).second["shader"];
    Yaml::Node& yml_input = (*it).second["input"];
    Yaml::Node& yml_output = (*it).second["output"];
    
    std::vector<std::string> inputs;
    if (!yml_input.IsNone()) {
      for (auto input = yml_input.Begin(); input != yml_input.End(); input++) {
        inputs.push_back((*input).second.As<std::string>());
      }
    }
    
    std::vector<std::string> outputs;
    if (!yml_output.IsNone()) {
      for (auto output = yml_output.Begin(); output != yml_output.End(); output++) {
        outputs.push_back((*output).second.As<std::string>());
      }
    }
    
    passes.push_back(scene_t::pass_data_t(inputs, yml_shader.As<std::string>(), outputs));
  }
  
  window_t window(WIDTH, HEIGHT, "nui");
  ubo_input_t ubo_input(0);
  quad_mesh_t mesh;
  
  window.add_input(ubo_input);
  
  scene_t scene(ubo_input, images, buffers, shaders, passes);
  
  while (window.poll()) {
    glClear(GL_COLOR_BUFFER_BIT);
    ubo_input.update();
    scene.render(mesh);
    window.swap();
  }
  
  return 0;
}
