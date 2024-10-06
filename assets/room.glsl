#pragma use "ray_march.glsl"
#pragma use "light.glsl"
#pragma use "math.glsl"
#pragma use "hdr.glsl"

float map(vec3 p)
{
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

out vec4 frag_color;
in vec2 frag_coord;

void main()
{
  vec3 view_pos = vec3(0.0, -3.0, -3.0);
  mat4 view_mat = mat4(1.0) * rotate_y(mouse.x * 1.5) * rotate_x(mouse.y * 1.5);
  
  vec3 rd = normalize(vec3(view_mat * vec4(frag_coord, 1.0, 1.0)));
  float td = ray_march(view_pos, rd);
  
  vec3 albedo = vec3(1.0, 1.0, 1.0);
  float metallic = 0.1;
  float roughness = 0.0;
  
  if (td > 0.0) {
    vec3 light = vec3(0.0);
    
    vec3 p = view_pos + rd * td;
    vec3 V = normalize(view_pos - p);
    vec3 N = map_normal(p);
    light += calc_point_lighting(p, V, N, albedo, metallic, roughness) * 0.7;
    
    vec3 R = reflect(-V, N);
    td = ray_march(p + R * 0.01, R);
    vec3 q = p + R * td;
    vec3 qV = normalize(p - q);
    vec3 qN = map_normal(q);
    vec3 qL = calc_point_lighting(q, qV, qN, albedo, metallic, roughness);
    light += qL * BRDF(vec3(1.0, 1.0, 1.0), 0.1, 0.5, -qV, V, N) * 0.3;
    
    frag_color.xyz = hdr_light(light, 1.0, 3.0);
  } else {
    frag_color.xyz = vec3(0.0);
  }
  
  frag_color.w = 1.0;
}
