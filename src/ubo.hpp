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
    const char* m_name;
  
  public:
    field_t(type_t type, const char* name) : m_type(type), m_name(name) {}
    const char* get_name() { return m_name; }
    type_t get_type() { return m_type; }
    std::string get_definition();
  };

private:
  GLuint m_ubo;
  std::string m_definition;
  std::map<const char*, int> m_mapping;
  int m_binding;

public:
  ubo_t(int binding, std::vector<field_t> fields);
  ~ubo_t();
  std::string get_definition();
  void sub(const char* name, void* data, int offset, int size);
  void attach_shader(shader_t& shader);
};

#endif
