#include "scene_file.hpp"

#include <iostream>
#include <filesystem>
#include <regex>
#include <sstream>

std::vector<std::string> split_string(std::string target, char delimiter);

scene_file_t::scene_file_t() {
  Yaml::Node root;
  Yaml::Parse(root, "scene.yml");
  
  parse_scene(root);
}

bool scene_file_t::parse_scene(Yaml::Node& node) {
  Yaml::Node& scene = node["scene"];
  
  if (!expect_map(scene, "scene")) return false;
  
  Yaml::Node& width = scene["width"];
  Yaml::Node& height = scene["height"];
  
  if (!expect_int(width, "width")) return false;
  if (!expect_int(height, "height")) return false;
  
  m_width = width.As<int>();
  m_height = height.As<int>();
  
  if (!parse_buffers(scene)) return false;
  if (!parse_shaders(scene)) return false;
  if (!parse_renderer(scene)) return false;
  
  return true;
}

bool scene_file_t::parse_renderer(Yaml::Node& node) {
  Yaml::Node& renderer = node["renderer"];
  if (!expect_array(renderer, "renderer")) return false;
  
  std::regex match_shader("([a-zA-Z0-9_,]+)");
  std::regex match_shader_input("([a-zA-A0-9_,]+) ([a-zA-Z0-9_,]+)");
  std::regex match_shader_output("([a-zA-A0-9_,]+) -> ([a-zA-Z0-9_,]+)");
  std::regex match_shader_input_output("([a-zA-Z0-9_,]+) ([a-zA-Z0-9_,]+) -> ([a-zA-Z0-9_,]+)");
  
  for (auto it = renderer.Begin(); it != renderer.End(); it++) {
    Yaml::Node& body = (*it).second;
    if (!expect_string(body, "pass")) return false;
    std::string pass = body.As<std::string>();
    std::smatch matches;
    
    std::string shader;
    std::vector<std::string> input;
    std::vector<std::string> output;
    
    if (std::regex_match(pass, matches, match_shader)) {
      shader = matches[1].str();
    } else if (std::regex_match(pass, matches, match_shader_input)) {
      shader = matches[1].str();
      input = split_string(matches[2].str(), ',');
    } else if (std::regex_match(pass, matches, match_shader_output)) {
      shader = matches[1].str();
      output = split_string(matches[2].str(), ',');
    } else if (std::regex_match(pass, matches, match_shader_input_output)) {
      shader = matches[1].str();
      input = split_string(matches[2].str(), ',');
      output = split_string(matches[3].str(), ',');
    } else {
      type_error("pass", "[shader] [input] -> [output]");
      return false;
    }
    
    m_renderer.push_back(scene_file_t::pass_t(shader, input, output));
  }
  
  return true;
}

bool scene_file_t::parse_shaders(Yaml::Node& node) {
  Yaml::Node& shaders = node["shaders"];
  if (shaders.IsNone()) return true;
  else if (!expect_map(shaders, "shaders")) return false;

  for (auto it = shaders.Begin(); it != shaders.End(); it++) {
    std::string name = (*it).first;
    Yaml::Node& body = (*it).second;
    if (!expect_map(body, name.c_str())) return false;
    
    Yaml::Node& src_node = body["src"];
    Yaml::Node& channels_node = body["channels"];
    
    if (!expect_string(src_node, "src")) return false;
    
    std::string src = src_node.As<std::string>();
    std::vector<std::string> channels;
    
    if (!channels_node.IsNone()) {
      if (!expect_string(channels_node, "channels")) return false;
      channels = split_string(channels_node.As<std::string>(), ' ');
    }
    
    m_shaders.push_back(scene_file_t::shader_t(name, src, channels));
  }
  
  return true;
}

bool scene_file_t::parse_buffers(Yaml::Node& node) {
  Yaml::Node& buffers = node["buffers"];
  if (buffers.IsNone()) return true;
  else if (!expect_map(buffers, "buffers")) return false;
  
  std::regex match_width_height("(\\d+)x(\\d+)");
  std::regex match_default("default");
  
  for (auto it = buffers.Begin(); it != buffers.End(); it++) {
    std::string name = (*it).first;
    Yaml::Node& body = (*it).second;
    if (!expect_string(body, name.c_str())) return false;
    
    std::string buffer = body.As<std::string>();
    std::smatch matches;
    
    if (std::regex_match(buffer, matches, match_width_height)) {
      int width = std::stoi(matches[1].str());
      int height = std::stoi(matches[2].str());
      m_buffers.push_back(scene_file_t::buffer_t(name, width, height));
    } else if (std::regex_match(buffer, matches, match_default)) {
      m_buffers.push_back(scene_file_t::buffer_t(name, m_width, m_height));
    } else {
      type_error("buffer", "'default' or [width]x[height]");
    }
  }
  
  return true;
}

bool scene_file_t::expect_map(Yaml::Node& node, std::string name) {
  if (!node.IsMap())
    type_error(name, "object");
  
  return node.IsMap();
}

bool scene_file_t::expect_int(Yaml::Node& node, std::string name) {
  if (!node.IsScalar())
    type_error(name, "int");
  
  return node.IsScalar();
}

bool scene_file_t::expect_string(Yaml::Node& node, std::string name) {
  if (!node.IsScalar())
    type_error(name, "string");
  
  return node.IsScalar();
}

bool scene_file_t::expect_array(Yaml::Node& node, std::string name) {
  if (!node.IsSequence())
    type_error(name, "array");
  
  return node.IsSequence();
}

void scene_file_t::type_error(std::string name, std::string type) {
  std::stringstream message;
  message << "error: expected '" << name << "' as " << type << "." << std::endl;
  error(message.str());
}

void scene_file_t::error(std::string message) {
  std::cerr << "error: " << message << std::endl;
}

std::vector<std::string> split_string(std::string target, char delimiter) {
  std::stringstream ss(target);
  std::string part;
  std::vector<std::string> splits;
  while (getline(ss, part, delimiter)) {
    splits.push_back(part);
  }
  return splits;
}
