#include "cubemap.hpp"

#include <iostream>
#include <filesystem>
#include <SDL2/SDL_image.h>

static GLuint surface_format(SDL_Surface* surface);

cubemap_t::cubemap_t(const char* src) {
  glGenTextures(1, &m_cubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap);
  
  std::string faces[] = {
    "rt", "lf", "up", "dn", "ft", "bk"
  };
  
  std::filesystem::path base(src);

  for (int i = 0; i < 6; i++) {
    std::filesystem::path full = base / std::filesystem::path(faces[i] + ".jpg");
    std::string file = full.string();

    SDL_Surface *surface = IMG_Load(file.c_str());
    if (!surface) {   
      std::cerr << "error: " << file << ": " << SDL_GetError() << std::endl;
      throw std::runtime_error("failed to load image");
    }

    glTexImage2D(
      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
      0,
      GL_RGB,
      surface->w, surface->h,
      0,
      surface_format(surface),
      GL_UNSIGNED_BYTE,
      surface->pixels
    );
    
    SDL_FreeSurface(surface);
  }
  
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

cubemap_t::~cubemap_t() {
  glDeleteTextures(1, &m_cubemap);
}

void cubemap_t::bind(int channel) {
  glActiveTexture(GL_TEXTURE0 + channel);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap);
}

GLuint surface_format(SDL_Surface* surface) {
  switch (surface->format->BytesPerPixel) {
  case 3:
    return GL_RGB;
  case 4:
    return GL_RGBA;
  default:
    throw std::runtime_error("unknown surface format");
  }
}
