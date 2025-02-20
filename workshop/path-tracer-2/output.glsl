out vec4 frag_color;
in vec2 frag_coord;

uniform sampler2D srcTexture;

void main() {
  frag_color.rgb = texture(srcTexture, frag_coord).rgb / (2.0 * float(frame));
  frag_color.a = 1.0;
}
