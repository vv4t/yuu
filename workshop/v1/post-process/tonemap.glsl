out vec4 frag_color;
in vec2 frag_coord;

uniform sampler2D screen;

float gamma = 0.5;
float exposure = 3.0;

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution;
  vec3 color = texture(screen, uv).rgb;
  color = vec3(1.0) - exp(-color * exposure);
  color = pow(color, vec3(1.0 / gamma));
  frag_color = vec4(color, 1.0);
}
