#version 300 es

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec2 v_uv;

void main()
{
  gl_Position = vec4(v_pos, 1.0);
}
