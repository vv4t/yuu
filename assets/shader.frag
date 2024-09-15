#pragma include "test.glsl"

in vec2 vs_uv;

out vec4 frag_color;

void main()
{
  frag_color = vec4(vs_uv * BRIGHTNESS, 0.0, 1.0);
}
