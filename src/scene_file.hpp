#ifndef SCENE_FILE_HPP
#define SCENE_FILE_HPP

#include <map>
#include <vector>
#include <string>
#include <filesystem>
#include <yaml/Yaml.hpp>

class scene_file_t {
public:
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
    
    std::string get_name() { return m_name; }
    int get_width() { return m_width; }
    int get_height() { return m_height; }
  };
  
  class data_t {
  private:
    std::string m_type;
    std::string m_name;

  public:
    data_t(std::string type, std::string name)
      : m_type(type),
        m_name(name) {}
    
    std::string get_type() { return m_type; }
    std::string get_name() { return m_name; }
  };

  class image_t {
  private:
    std::string m_name;
    std::string m_src;
  
  public:
    image_t(std::string name, std::string src)
      : m_name(name),
        m_src(src) {}
    
    std::string get_name() { return m_name; }
    std::string get_src() { return m_src; }
  };

  class cubemap_t {
  private:
    std::string m_name;
    std::string m_src;

  public:
    cubemap_t(std::string name, std::string src)
      : m_name(name),
        m_src(src) {}
    
    std::string get_name() { return m_name; }
    std::string get_src() { return m_src; }
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
    
    std::string get_name() { return m_name; }
    std::string get_src() { return m_src; }
    std::vector<std::string> get_channels() { return m_channels; }
  };
  
  class pass_t {
  private:
    std::string m_shader;
    std::vector<std::string> m_input;
    std::vector<std::string> m_output;
  
  public:
    pass_t(
      std::string shader,
      std::vector<std::string> input,
      std::vector<std::string> output
    )
      : m_shader(shader),
        m_input(input),
        m_output(output) {}
    
    std::string get_shader() { return m_shader; }
    std::vector<std::string> get_input() { return m_input; }
    std::vector<std::string> get_output() { return m_output; }
  };

private:
  std::filesystem::path m_base;

  int m_width;
  int m_height;
  bool m_failed;
  
  std::vector<data_t> m_data;
  std::vector<std::string> m_scripts;
  std::vector<cubemap_t> m_cubemaps;
  std::vector<image_t> m_images;
  std::vector<buffer_t> m_buffers;
  std::vector<shader_t> m_shaders;
  std::vector<pass_t> m_renderer;

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
  bool parse_renderer(Yaml::Node& node);
  bool parse_logic(Yaml::Node& node);
  bool parse_logic_data(Yaml::Node& node);
  bool parse_logic_scripts(Yaml::Node& node);

public:
  scene_file_t(std::string src);
  bool validate();
  std::vector<data_t> get_data() { return m_data; }
  std::vector<std::string> get_scripts() { return m_scripts; }
  std::vector<buffer_t> get_buffers() { return m_buffers; }
  std::vector<image_t> get_images() { return m_images; }
  std::vector<cubemap_t> get_cubemaps() { return m_cubemaps; }
  std::vector<shader_t> get_shaders() { return m_shaders; }
  std::vector<pass_t> get_renderer() { return m_renderer; }
  int get_width() { return m_width; }
  int get_height() { return m_height; }
};

#endif
