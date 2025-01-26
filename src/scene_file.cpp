#include "scene_file.hpp"

#include <iostream>
#include <filesystem>
#include <regex>
#include <sstream>

scene_file_t::scene_file_t() {
  Yaml::Node root;
  Yaml::Parse(root, "scene.yml");
  
  parse_scene(root);
}

bool scene_file_t::parse_scene(Yaml::Node& node) {
  if (!expect_map(node, "scene")) return false;
  
  Yaml::Node& scene = node["scene"];
  if (!expect_int(scene, "width")) return false;
  if (!expect_int(scene, "height")) return false;
  
  m_width = scene["width"].As<int>();
  m_height = scene["height"].As<int>();
  
  if (!parse_buffers(scene)) return false;
  if (!parse_shaders(scene)) return false;
  
  return true;
}

bool scene_file_t::parse_shaders(Yaml::Node& node) {
  Yaml::Node& shaders = node["shaders"];
  if (shaders.IsNone()) return true;
  else if (!expect_map(node, "shaders")) return false;

  for (auto it = shaders.Begin(); it != shaders.End(); it++) {
    std::string name = (*it).first;
    if (!expect_map(shaders, name.c_str())) return false;
    Yaml::Node& body = (*it).second;
    
    Yaml::Node& src_node = body["src"];
    Yaml::Node& channels_node = body["channels"];
    
    if (!expect_string(body, "src")) return false;
    
    if (!channels_node.IsNone()) {
      expect_string(body, "channels");
      std::string channels = channels_node.As<std::string>();
    }
  }
  
  return true;
}

bool scene_file_t::parse_buffers(Yaml::Node& node) {
  Yaml::Node& buffers = node["buffers"];
  if (buffers.IsNone()) return true;
  else if (!expect_map(node, "buffers")) return false;
  
  std::regex match_width_height("(\\d+)x(\\d+)");
  std::regex match_default("default");
  
  for (auto it = buffers.Begin(); it != buffers.End(); it++) {
    std::string name = (*it).first;
    if (!expect_string(buffers, name.c_str())) return false;
    std::string body = (*it).second.As<std::string>();
    std::smatch matches;
    
    if (std::regex_match(body, matches, match_width_height)) {
      int width = std::stoi(matches[1].str());
      int height = std::stoi(matches[2].str());
      m_buffers.push_back(scene_file_t::buffer_t(name, width, height));
    } else if (std::regex_match(body, matches, match_default)) {
      m_buffers.push_back(scene_file_t::buffer_t(name, m_width, m_height));
    } else {
      type_error("buffer", "'default' or [width]x[height]");
    }
  }
  
  return true;
}

bool scene_file_t::expect_map(Yaml::Node& root, std::string name) {
  if (root[name].IsNone())
    type_error(name, "object");
  
  return !root[name].IsNone();
}

bool scene_file_t::expect_int(Yaml::Node& root, std::string name) {
  if (!root[name].IsScalar())
    type_error(name, "int");
  
  return root[name].IsScalar();
}

bool scene_file_t::expect_string(Yaml::Node& root, std::string name) {
  if (!root[name].IsScalar())
    type_error(name, "string");
  
  return root[name].IsScalar();
}

bool scene_file_t::expect_array(Yaml::Node& root, std::string name) {
  if (!root[name].IsSequence())
    type_error(name, "array");
  
  return root[name].IsScalar();
}

void scene_file_t::type_error(std::string name, std::string type) {
  std::stringstream message;
  message << "error: expected '" << name << "' as " << type << "." << std::endl;
  error(message.str());
}

void scene_file_t::error(std::string message) {
  std::cerr << "error: " << message << std::endl;
}
