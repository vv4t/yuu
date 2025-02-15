#pragma use "../lib/ray_march.glsl"
#pragma use "../lib/math.glsl"

out vec4 frag_color;

float integrate_edge(vec3 v1, vec3 v2) {
  // Using built-in acos() function will result flaws
  // Using fitting result for calculating acos()
  float x = dot(v1, v2);
  float y = abs(x);

  float a = 0.8543985 + (0.4965155 + 0.0145206*y)*y;
  float b = 3.4175940 + (4.1616724 + y)*y;
  float v = a / b;

  float theta_sintheta = (x > 0.0) ? v : 0.5*inversesqrt(max(1.0 - x*x, 1e-7)) - v;

  return (cross(v1, v2)*theta_sintheta).z;
}

float ltc_evaluate(mat3 Minv, vec3 p) {
  vec3 L[4];
  L[0] = normalize(Minv * (vec3(-2.0, +2.0, 6.0) - p));
  L[1] = normalize(Minv * (vec3(-2.0, -6.0, 6.0) - p));
  L[2] = normalize(Minv * (vec3(+2.0, -6.0, 6.0) - p));
  L[3] = normalize(Minv * (vec3(+2.0, +2.0, 6.0) - p));
  
  float vsum = 0.0;
  vsum += integrate_edge(L[0], L[1]);
  vsum += integrate_edge(L[1], L[2]);
  vsum += integrate_edge(L[2], L[3]);
  vsum += integrate_edge(L[3], L[0]);
  return 1.0 / (2.0 * M_PI) * abs(vsum);
}

mat3 get_specular_ltc(float NdotV) {
  float t = acos(max(NdotV, 0.0));
  float cosNV = cos(t);
  float sinNV = sin(t);
  
  mat3 B = mat3(
    vec3(cosNV, 0.0, -sinNV),
    vec3(0.0, 1.0, 0.0),
    vec3(sinNV, 0.0, cosNV)
  );
  
  return mat3(
    vec3(16.0, 0.0, 0.0),
    vec3(0.0, 16.0, 0.0),
    vec3(0.0, 0.0, 1.0)
  ) * B;
}

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution * 2.0 - 1.0;
  uv.x *= u_resolution.x / u_resolution.y;
  
  vec3 view_pos = vec3(0.0, -3.0, -5.0);
  mat4 view_mat = mat4(1.0);
  
  vec3 rd = normalize(vec3(view_mat * vec4(uv, 1.0, 1.0)));
  float td = ray_march(view_pos, rd);
  vec3 p = view_pos + rd * td;
  vec3 N = map_normal(p);
  vec3 V = normalize(view_pos - p);
  
  vec3 T1,T2;
  T1 = normalize(V - N * dot(V, N));
  T2 = cross(N, T1);
  mat3 basis = transpose(mat3(T1, T2, N));
  
  float diffuse = ltc_evaluate(mat3(1.0) * basis, p);
  float specular = ltc_evaluate(get_specular_ltc(dot(N,V)) * basis, p); 

  frag_color.xyz = vec3(1.0) * (diffuse + specular) * vec3(1.0, 3.0, 3.0) * (1.0 + 0.05 * cos(u_time * 2.0));
  frag_color.w = 1.0;
}

float map(vec3 p) {
  float s = MAX_DISTANCE;
  s = min(s, plane(p, vec3(0, 0, -1), -6.0));
  s = min(s, plane(p, vec3(0, 0, +1), -6.0));
  s = min(s, plane(p, vec3(+1, 0, 0), -6.0));
  s = min(s, plane(p, vec3(-1, 0, 0), -6.0));
  s = min(s, plane(p, vec3(0, +1, 0), -6.0));
  s = min(s, plane(p, vec3(0, -1, 0), -6.0));
  return s;
}
