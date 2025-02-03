out vec4 frag_color;

uniform sampler2D u_buffer;

void main() {
  frag_color.xyz = texture2D(u_buffer, gl_FragCoord.xy).xyz;
  frag_color.w = 1.0;
}
