
out vec4 frag_color;
in vec2 frag_coord;

uniform sampler2D first;

void main()
{
  frag_color = texture(first, frag_coord);
}
