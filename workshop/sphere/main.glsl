#pragma use "../lib/ray_march.glsl"
#pragma use "../lib/math.glsl"

out vec4 frag_color;
in vec2 frag_coord;

float diffuse(vec3 V, vec3 L, vec3 N) {
  return dot(L, N);
}

float blinn_phong(vec3 V, vec3 L, vec3 N) {
  vec3 h = normalize(V + L);
  return pow(max(dot(N, h), 0.0), 16.0);
}

void main() {
  vec2 uv = frag_coord * 2.0 - 1.0;
  uv.x *= resolution.x / resolution.y;
  vec2 mp = mouse * 2.0 - 1.0;
  mp.x += M_PI;
  
  vec3 view_pos = (rotate_y(mp.x * 3.0) * rotate_x(mp.y * 1.5) * vec4(0.0, 0.0, -2.0, 1.0)).xyz;
  mat4 view_mat = mat4(1.0) * rotate_y(mp.x * 3.0) * rotate_x(mp.y * 1.5);
  
  vec3 rd = normalize(vec3(view_mat * vec4(uv, 1.0, 1.0)));
  float td = ray_march(view_pos, rd);
  
  if (td < MAX_DISTANCE) {
    
    vec3 p = view_pos + rd * td;
    vec3 N = vec3(0.0, 0.0, 1.0);
    vec3 V = normalize(vec3(0.0 + cos(time) * 4.0, 0.0, 3.0));
    
    float t = dot(N, V);
    float cosNV = t;
    float sinNV = sqrt(1.0 - cosNV * cosNV);
    
    mat3 B = mat3(
      vec3(-sinNV, 0.0, cosNV),
      vec3(0.0, 1.0, 0.0),
      vec3(cosNV, 0.0, sinNV)
    );
    
    mat3 T = mat3(
      vec3(16.0 * (1.0 + 0.2 *cosNV), 0.0, 0.0),
      vec3(0.0, 16.0, 0.0),
      vec3(0.0, 0.0, 1.0)
    ) * B;
    
    vec3 L = normalize(map_normal(p));
    vec3 LT = normalize(T * map_normal(p));
    
    float a = blinn_phong(V, L, vec3(0.0, 0.0, 1.0));
    float b = diffuse(V, LT, vec3(0.0, 0.0, 1.0));
    float c = pow(max(dot(reflect(-V,N),L), 0.0), 32.0);
    
    // frag_color.xyz = vec3(a);
    frag_color.xyz = vec3(b);
    // frag_color.xyz = vec3(abs(a - b));
  } else {
    frag_color.xyz = vec3(0.0, 0.5, 1.0);
  }
  frag_color.w = 1.0;
}

float map(vec3 p) {
  float s = MAX_DISTANCE;
  s = min(s, sphere(p, vec3(0.0, 0.0, 0.0), 1.0));
  s = min(s, cube(p, vec3(10.0, 0.0, 0.0), vec3(10.0, 0.05, 0.05)));
  s = min(s, cube(p, vec3(0.0, 0.0, 0.0), vec3(0.05, 10.0, 0.05)));
  s = min(s, cube(p, vec3(0.0, 0.0, 10.0), vec3(0.05, 0.05, 10.0)));
  return s;
}
