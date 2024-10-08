
out vec4 frag_color;
in vec2 frag_coord;

uniform sampler2D first;
uniform sampler2D second;

void main()
{
  frag_color = texture(first, frag_coord);
}
