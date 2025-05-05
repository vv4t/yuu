#pragma use "../lib/ray_march.glsl"
#pragma use "../lib/light.glsl"
#pragma use "../lib/math.glsl"

out vec4 frag_color;
in vec2 frag_coord;

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution * 2.0 - 1.0;
  uv.x *= u_resolution.x / u_resolution.y;
  
  vec3 view_pos = vec3(0.0, -3.0 + sin(u_time), -5.0);
  mat4 view_mat = mat4(1.0) * rotate_y(sin(u_time));
  
  vec3 rd = normalize(vec3(view_mat * vec4(uv, 1.0, 1.0)));
  float td = ray_march(view_pos, rd);
  
  vec3 albedo = vec3(1.0, 1.0, 1.0);
  float metallic = 0.1;
  float roughness = 0.0;
  
  if (td > 0.0) {
    vec3 light = vec3(0.0);
    
    vec3 p = view_pos + rd * td;
    
    if (p.x > -2.0 && p.x < 2.0 && p.y > -5.0 && p.y < 2.0 && p.z > 4.0) {
      frag_color = vec4(1.0, 3.0, 3.0, 1.0);
      return;
    }
    
    vec3 V = normalize(view_pos - p);
    vec3 N = map_normal(p);
    light += calc_point_lighting(p, V, N, albedo, metallic, roughness);
    
    frag_color.xyz = light;
  } else {
    frag_color.xyz = vec3(0.0);
  }
  
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
  s = min(s, sphere(p, vec3(0, -5, 0), 1.0));
  return s;
}

light_t lights[] = light_t[](
  light_t( vec3(0.0, 0.0, 5.0), vec3(0.5, 1.0, 1.0) * 10.0 )
  // light_t( vec3(-3.0, 2.0, 3.0), vec3(1.0, 0.5, 1.0) * 10.0 )
);

light_t lights_get(int num) {
  return lights[num];
}

int lights_count() {
  return 1;
}
