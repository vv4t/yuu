out vec4 frag_color;
in vec2 frag_coord;

uniform sampler2D first;
uniform sampler2D second;

void main() {
  frag_color.rgb = mix(texture(first, frag_coord).rgb, texture(second, frag_coord).rgb, 0.5);
  frag_color.w = 1.0;
}
