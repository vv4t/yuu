#pragma use "../lib/ray_march.glsl"
#pragma use "../lib/math.glsl"
#pragma use "../lib/pbr.glsl"

float rand(vec2 co) {
  return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float map(vec3 p) {
  float d = plane(p, vec3(0.0, 1.0, 0.0), -4.0);
  
  float gap = 32.0;
  vec3 a = fract(p / gap) * gap;
  vec3 b = floor(p / gap) * gap;
  vec3 q = vec3(a.x, p.y, a.z);
  float h = 8.0;
  
  float ot = rand(b.xz);
  float os = (rand(b.zx) * 2.0 - 1.0) * 4.0;

  vec3 o = vec3(16.0, h / 2.0, 16.0);
  
  for (float z = -gap; z <= gap; z += gap) {
    for (float x = -gap; x <= gap; x += gap) {
      float s = pow(cos(-p.y + u_time * 4.0), 2.0) * pow(cos(-p.y * 0.3 + u_time * 3.0), 2.0) * 0.4 + 0.1;
      float pole = cube(q, o + vec3(x, 0.0, z), vec3(s, h, s));
      
      float rt = u_time * os + ot;
      vec3 qo = (q - o);
      vec3 rq = qo * mat3(rotate_y(rt)) + o;
      vec3 ro = o + vec3(0.0, cos(atan(qo.z, qo.x) * 3.0), 0.0);
      
      float r1 = cube(rq, ro + vec3(0.0, 5.0, 0.0), vec3(3.0, 0.5, 3.0));
      float r2 = cube(rq, ro + vec3(0.0, 5.0, 0.0), vec3(2.5, 2.0, 2.5));
      
      d = min(d, min(pole, max(r1, -r2)));
    }
  }
  
  return d;
}

float ray_intersect(vec3 ro, vec3 rd)
{
  vec3 n = vec3(0.0, 1.0, 0.0);
  float d = -4.0;
  
  float td = dot(n * d - ro, n) / dot(rd, n);
  
  if (td < 0.0) {
    return MAX_DISTANCE;
  }
  
  return td;
}

out vec4 frag_color;

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution * 2.0 - 1.0;
  uv.x *= u_resolution.x / u_resolution.y;
  
  vec3 view_pos = vec3(0.0, 0.0, u_time * 4.0);
  mat4 view_mat = mat4(1.0);
  
  vec3 ld = normalize(vec3(+0.5, -0.5, +0.25));
  
  vec3 rd = normalize(vec3(view_mat * vec4(uv, 1.0, 1.0)));
  float td = ray_march(view_pos, rd);
  td = min(td, ray_intersect(view_pos, rd));
  
  if (td < MAX_DISTANCE) {
    vec3 p = view_pos + td * rd;
    vec3 V = normalize(view_pos - p);
    vec3 N = map_normal(p);
    
    vec3 light = 0.5 + 0.5 * vec3(1.0) * dot(-ld, N) * shadow(p, -ld, 1000.0);
    
    frag_color.xyz = light;
  } else {
    float q = 0.5 + (0.5 - uv.y) * 0.3;
    frag_color.xyz = vec3(q, q, 1.0);
  }
  frag_color.w = 1.0;
}
