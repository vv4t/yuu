#include "logic.hpp"

extern "C" {
  #include <lualib.h>
  #include <lauxlib.h>
  #include <lua.h>
}

logic_t::logic_t()
  : L(luaL_newstate()),
    m_enabled(false) {
  luaL_openlibs(L);
}

logic_t::~logic_t() {
  lua_close(L);
}

void logic_t::add_script(std::string src) {
  m_enabled = true;
  if (luaL_dofile(L, src.c_str()) != LUA_OK) {
    std::cerr << "error: " << lua_tostring(L, -1) << std::endl;
    std::exit(1);
  }
}

void logic_t::define_float(std::string name) {
  init_lua_float(name);
  m_fields.push_back(ubo_t::field_t(ubo_t::type_t::FLOAT, name));
}

void logic_t::define_vec2(std::string name) {
  init_lua_vector(name, { "x", "y" });
  m_fields.push_back(ubo_t::field_t(ubo_t::type_t::VEC2, name));
}

void logic_t::define_vec3(std::string name) {
  init_lua_vector(name, { "x", "y", "z" });
  m_fields.push_back(ubo_t::field_t(ubo_t::type_t::VEC3, name));
}

void logic_t::define_vec4(std::string name) {
  init_lua_vector(name, { "x", "y", "z", "w" });
  m_fields.push_back(ubo_t::field_t(ubo_t::type_t::VEC4, name));
}

void logic_t::refresh_data() {
  int offset = 0;

  for (auto& field : m_fields) {
    std::string& name = field.get_name();
    switch (field.get_type()) {
    case ubo_t::type_t::FLOAT:
      offset = read_float(name.c_str(), offset);
      break;
    case ubo_t::type_t::VEC2:
      offset = read_vec2(name.c_str(), offset);
      break;
    case ubo_t::type_t::VEC3:
      offset = read_vec3(name.c_str(), offset);
      break;
    case ubo_t::type_t::VEC4:
      offset = read_vec4(name.c_str(), offset);
      break;
    }
  }
}

void logic_t::update() {
  if (!m_enabled) return;

  lua_getglobal(L, "update");
  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    std::cerr << "error: " << lua_tostring(L, -1) << std::endl;
    std::exit(1);
  }

  refresh_data();
}

int logic_t::read_float(const char* name, int offset) {
  lua_getglobal(L, name);
  m_buffer[offset++] = lua_tonumber(L, -1);
  lua_pop(L, 1);
  return offset;
}

int logic_t::read_vec2(const char* name, int offset) {
  lua_getglobal(L, name);
    lua_getfield(L, -1, "x");
    m_buffer[offset++] = lua_tonumber(L, -1);
    lua_getfield(L, -2, "y");
    m_buffer[offset++] = lua_tonumber(L, -1);
  lua_pop(L, 3);
  return offset;
}

int logic_t::read_vec3(const char* name, int offset) {
  lua_getglobal(L, name);
    lua_getfield(L, -1, "x");
    m_buffer[offset++] = lua_tonumber(L, -1);
    lua_getfield(L, -2, "y");
    m_buffer[offset++] = lua_tonumber(L, -1);
    lua_getfield(L, -3, "z");
    m_buffer[offset++] = lua_tonumber(L, -1);
  lua_pop(L, 4);
  return offset;
}

int logic_t::read_vec4(const char* name, int offset) {
  lua_getglobal(L, name);
    lua_getfield(L, -1, "x");
    m_buffer[offset++] = lua_tonumber(L, -1);
    lua_getfield(L, -2, "y");
    m_buffer[offset++] = lua_tonumber(L, -1);
    lua_getfield(L, -3, "z");
    m_buffer[offset++] = lua_tonumber(L, -1);
    lua_getfield(L, -4, "w");
    m_buffer[offset++] = lua_tonumber(L, -1);
  lua_pop(L, 5);
  return offset;
}

void logic_t::init_lua_float(std::string name) {
  lua_pushnumber(L, 0.0);
  lua_setglobal(L, name.c_str());
  m_buffer.push_back(0.0);
}

void logic_t::init_lua_vector(std::string name, std::vector<std::string> fields) {
  lua_newtable(L);
  for (auto field : fields) {
    lua_pushstring(L, field.c_str());
    lua_pushnumber(L, 0);
    lua_settable(L, -3);
    m_buffer.push_back(0.0);
  }
  lua_setglobal(L, name.c_str());
}

std::vector<float>& logic_t::get_data() {
  return m_buffer;
}
