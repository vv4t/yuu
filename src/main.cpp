#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <yaml/Yaml.hpp>
#include <filesystem>
#include "window.h"
#include "ubo_input.h"
#include "scene.h"

int main(int argc, const char *argv[]) {
  srand(time(NULL));
  if (argc != 2) {
    std::cerr << "usage: yuu [scene-yml]" << std::endl;
    return 1;
  }
  
  std::filesystem::path base = std::filesystem::path(argv[1]).parent_path();
  
  Yaml::Node root;
  Yaml::Parse(root, argv[1]);
  
  if (root["scene"].IsNone()) {
    throw std::runtime_error("missing top level 'scene'");
  }
  
  Yaml::Node& yml_width = root["scene"]["width"];
  Yaml::Node& yml_height = root["scene"]["height"];
  
  int width = 800;
  int height = 600;
  
  if (!yml_width.IsNone()) {
    width = yml_width.As<int>();
  }
  
  if (!yml_height.IsNone()) {
    height = yml_height.As<int>();
  }
  
  window_t window(width, height, "yuu");
  ubo_input_t ubo_input(0);
  quad_mesh_t mesh;
  window.add_input(ubo_input);
  scene_t scene(width, height, ubo_input);
  
  Yaml::Node& yml_images = root["scene"]["images"];
  Yaml::Node& yml_buffers = root["scene"]["buffers"];
  Yaml::Node& yml_shaders = root["scene"]["shaders"];
  Yaml::Node& yml_passes = root["scene"]["passes"];
  
  if (!yml_images.IsNone()) {
    for (auto it = yml_images.Begin(); it != yml_images.End(); it++) {
      std::string name = (*it).first;
      std::string src = (*it).second.As<std::string>();
      std::filesystem::path full = base / std::filesystem::path(src);
      scene.image_add(name, full.u8string());
    }
  }
  
  if (!yml_buffers.IsNone()) {
    for (auto it = yml_buffers.Begin(); it != yml_buffers.End(); it++) {
      std::string name = (*it).first;
      Yaml::Node& yml_width = (*it).second["width"];
      Yaml::Node& yml_height = (*it).second["height"];
      
      int buffer_width = width;
      int buffer_height = height;
      
      if (!yml_width.IsNone()) {
        buffer_width = yml_width.As<int>();
      }
      
      if (!yml_height.IsNone()) {
        buffer_height = yml_height.As<int>();
      }
      
      scene.buffer_add(name, buffer_width, buffer_height);
    }
  }
  
  for (auto it = yml_shaders.Begin(); it != yml_shaders.End(); it++) {
    Yaml::Node& yml_src = (*it).second["src"];
    Yaml::Node& yml_channels = (*it).second["channels"];
    
    std::vector<std::string> channels;
    if (!yml_channels.IsNone()) {
      for (auto channel = yml_channels.Begin(); channel != yml_channels.End(); channel++) {
        channels.push_back((*channel).second.As<std::string>());
      }
    }
    
    std::string name = (*it).first;
    std::string src = yml_src.As<std::string>();
    std::filesystem::path full = base / std::filesystem::path(src);
    
    scene.shader_add(name, channels, full.u8string());
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
    
    scene.pass_add(inputs, yml_shader.As<std::string>(), outputs);
  }
  
  while (window.poll()) {
    glClear(GL_COLOR_BUFFER_BIT);
    scene.render(mesh);
    window.swap();
  }
  
  return 0;
}
