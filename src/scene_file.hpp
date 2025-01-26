#ifndef SCENE_FILE_HPP
#define SCENE_FILE_HPP

#include <map>
#include <vector>
#include <string>
#include <yaml/Yaml.hpp>

class scene_file_t {
private:
  class buffer_t {
  private:
    std::string m_name;
    int m_width;
    int m_height;
    
  public:
    buffer_t(std::string name, int width, int height)
      : m_name(name),
        m_width(width),
        m_height(height) {}
    int get_width() { return m_width; }
    int get_height() { return m_height; }
  };
  
  class shader_t {
  private:
    std::string m_name;
    std::string m_src;
    std::vector<std::string> m_channels;

  public:
    shader_t(std::string name, std::string src, std::vector<std::string> channels)
      : m_name(name),
        m_src(src),
        m_channels(channels) {}
  };

private:
  int m_width;
  int m_height;
  
  std::vector<buffer_t> m_buffers;
  std::vector<shader_t> m_shaders;

private:
  void error(std::string message);
  void type_error(std::string name, std::string type);
  bool expect_map(Yaml::Node& root, std::string name);
  bool expect_array(Yaml::Node& root, std::string name);
  bool expect_int(Yaml::Node& root, std::string name);
  bool expect_string(Yaml::Node& root, std::string name);
  bool parse_scene(Yaml::Node& node);
  bool parse_buffers(Yaml::Node& node);
  bool parse_shaders(Yaml::Node& node);

public:
  scene_file_t();
};

#endif
