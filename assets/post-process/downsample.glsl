out vec4 frag_color;
in vec2 frag_coord;

uniform sampler2D screen;

// https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom
void main() {
  vec2 texel = 1.0 / vec2(800.0, 600.0);
  float x = texel.x;
  float y = texel.y;

  // Take 13 samples around current texel:
  // a - b - c
  // - j - k -
  // d - e - f
  // - l - m -
  // g - h - i
  // === ('e' is the current texel) ===
  vec3 a = texture(screen, vec2(frag_coord.x - 2.0*x, frag_coord.y + 2.0*y)).rgb;
  vec3 b = texture(screen, vec2(frag_coord.x,       frag_coord.y + 2.0*y)).rgb;
  vec3 c = texture(screen, vec2(frag_coord.x + 2.0*x, frag_coord.y + 2.0*y)).rgb;

  vec3 d = texture(screen, vec2(frag_coord.x - 2.0*x, frag_coord.y)).rgb;
  vec3 e = texture(screen, vec2(frag_coord.x,       frag_coord.y)).rgb;
  vec3 f = texture(screen, vec2(frag_coord.x + 2.0*x, frag_coord.y)).rgb;

  vec3 g = texture(screen, vec2(frag_coord.x - 2.0*x, frag_coord.y - 2.0*y)).rgb;
  vec3 h = texture(screen, vec2(frag_coord.x,       frag_coord.y - 2.0*y)).rgb;
  vec3 i = texture(screen, vec2(frag_coord.x + 2.0*x, frag_coord.y - 2.0*y)).rgb;

  vec3 j = texture(screen, vec2(frag_coord.x - x, frag_coord.y + y)).rgb;
  vec3 k = texture(screen, vec2(frag_coord.x + x, frag_coord.y + y)).rgb;
  vec3 l = texture(screen, vec2(frag_coord.x - x, frag_coord.y - y)).rgb;
  vec3 m = texture(screen, vec2(frag_coord.x + x, frag_coord.y - y)).rgb;

  // Apply weighted distribution:
  // 0.5 + 0.125 + 0.125 + 0.125 + 0.125 = 1
  // a,b,d,e * 0.125
  // b,c,e,f * 0.125
  // d,e,g,h * 0.125
  // e,f,h,i * 0.125
  // j,k,l,m * 0.5
  // This shows 5 square areas that are being sampled. But some of them overlap,
  // so to have an energy preserving downsample we need to make some adjustments.
  // The weights are the distributed, so that the sum of j,k,l,m (e.g.)
  // contribute 0.5 to the final color output. The code below is written
  // to effectively yield this sum. We get:
  // 0.125*5 + 0.03125*4 + 0.0625*4 = 1
  vec3 downsample;
  downsample = e*0.125;
  downsample += (a+c+g+i)*0.03125;
  downsample += (b+d+f+h)*0.0625;
  downsample += (j+k+l+m)*0.125;
  
  frag_color = vec4(downsample, 1.0);
}
