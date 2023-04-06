uniform vec4 u_Color;
uniform int u_Number;

const vec2 lightPosition = vec2(0.0, 0.0);
const vec3 white = vec3(1.0);

vec3 colorLookup(vec3 point) {
  if(u_Number == 0)
    return white;

  // If the ball is striped
  if(u_Number > 8 && abs(point.y) > 0.55)
      return mix(vec3(u_Color), white, smoothstep(0.55, 0.56, abs(point.y)));

  float dist = distance(point.xy, vec2(0,0));
  if(dist < 0.4) 
    return white;

  dist = smoothstep(0.4, 0.41, dist);
  return mix(white, vec3(u_Color), dist);
}

void main() {
  vec2 center = vec2(0.5, 0.5);

  float dist = distance(center, gl_TexCoord[0].xy) / 0.5f;
  float alpha = dist > 0.99 ? 1.0 - smoothstep(0.99, 1.0, dist) : 1.0;

  vec2 uv = gl_TexCoord[0].xy * 2.0 - 1.0;
  vec3 normal = vec3(uv, sqrt(1.0 - clamp(dot(uv, uv), 0.0, 1.0)));
  vec3 map = 0.5 + 0.5 * normal;

  vec3 light = normalize(vec3(lightPosition.x, lightPosition.y, 1));
  float brightness = clamp(dot(light, normal), 0.1, 1.0);

  gl_FragColor = vec4(colorLookup(normal) * brightness, alpha);
}
