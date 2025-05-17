#ifndef UBO_HPP
#define UBO_HPP

#include <string>
#include <map>
#include <vector>
#include <glad/glad.h>
#include "shader.hpp"

class ubo_t {
public:
  enum type_t {
    FLOAT,
    VEC2,
    VEC3,
    VEC4
  };
  
  class field_t {
  private:
    type_t m_type;
    std::string m_name;
  
  public:
    field_t(type_t type, std::string name) : m_type(type), m_name(name) {}
    std::string& get_name() { return m_name; }
    type_t get_type() { return m_type; }
    std::string get_definition();
  };

private:
  GLuint m_ubo;
  std::string m_definition;
  std::map<std::size_t, int> m_mapping;
  int m_binding;

public:
  ubo_t(int binding, std::vector<field_t> fields);
  ~ubo_t();
  std::string get_definition();
  void sub(std::string name, void* data, int offset, int size);
  void attach_shader(shader_t& shader);
};

#endif
