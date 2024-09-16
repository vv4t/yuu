out vec4 frag_color;
in vec2 frag_coord;

void main() {
  float a = length(frag_coord - mouse) < 0.1 ? 1.0 : 0.0;
  frag_color = vec4(a);
}
