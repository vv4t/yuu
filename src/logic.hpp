#ifndef LOGIC_HPP
#define LOGIC_HPP

extern "C" {
  #include <lua.h>
}

#include "ubo.hpp"
#include "scene_file.hpp"
#include <vector>

class logic_t {
private:
  lua_State* L;
  std::vector<ubo_t::field_t> m_fields;
  std::vector<float> m_buffer;
  bool m_enabled;

  void init_lua_float(std::string name);
  void init_lua_vector(std::string name, std::vector<std::string> fields);
  void refresh_data();

  int read_float(const char* name, int offset);
  int read_vec2(const char* name, int offset);
  int read_vec3(const char* name, int offset);
  int read_vec4(const char* name, int offset);

public:
  logic_t();
  ~logic_t();
  
  void update();
  void add_script(std::string name);
  void define_float(std::string name);
  void define_vec2(std::string name);
  void define_vec3(std::string name);
  void define_vec4(std::string name);

  std::vector<float>& get_data();
};

#endif
