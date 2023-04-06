#version 130 

#define PI 3.14159265358

uniform vec4 u_Color;
uniform int u_Number;
uniform vec3 u_Rotation;
uniform sampler2D u_NumbersTexture;

const float numberTextArea = 0.28;
const float numberTextAreaEdge = 0.0033;
const vec2 lightPosition = vec2(0.0);
const vec3 white = vec3(1.0);

mat4 rotateX(float angle) {
  return mat4(
    1.0, 0.0, 0.0, 0.0,
    0.0, cos(angle), -sin(angle), 0.0,
    0.0, sin(angle), cos(angle), 0.0,
    0.0, 0.0, 0.0, 1.0
  );
}

mat4 rotateY(float angle) {
  return mat4(
    cos(angle), 0.0, sin(angle), 0.0,
    0.0, 1.0, 0.0, 0.0,
    -sin(angle), 0.0, cos(angle), 0.0,
    0.0, 0.0, 0.0, 1.0
  );
}

mat4 rotateZ(float angle) {
  return mat4(
    cos(angle), -sin(angle), 0.0, 0.0,
    sin(angle), cos(angle), 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
  );
}

vec3 colorLookup(vec3 point, int number) {
  if(number == 0)
    return white;

  number--;

  if(abs(point.x) < numberTextArea && abs(point.y) < numberTextArea) {
    vec2 uv = (point.xy + vec2(numberTextArea)) / (numberTextArea * 2);

    if(point.z < 0.0)
      uv.x = 1.0 - uv.x;

    // The grid is 4x4
    uv *= 0.25;
    uv.x += (number % 4) * 0.25;
    uv.y += number / 4 * 0.25;

    vec3 color = texture2D(u_NumbersTexture, uv).rgb;

    return color;
  }

  // If the ball is striped
  if(number > 7 && abs(point.y) > 0.55)
      return mix(vec3(u_Color), white, smoothstep(0.55, 0.56, abs(point.y)));

  float dist = distance(point.xy, vec2(0.0));
  if(dist < 0.4) 
    return white;

  dist = smoothstep(0.4, 0.41, dist);
  return mix(white, vec3(u_Color), dist);
}

float specular(vec3 light, vec3 normal) {
  vec3 r = reflect(-light, normal);
  vec3 spot = normal * 10.0;
  spot.z = 400.0;
  vec3 v = normalize(spot);
  float bounce = max(dot(r, v), 0.0);
  return pow(smoothstep(0.5, 1.0, pow(bounce, 10.0)), 30.0) * 0.9;
}

void main() {
  vec2 center = vec2(0.5, 0.5);

  float dist = distance(center, gl_TexCoord[0].xy) * 2.0;
  float alpha = dist > 0.99 ? 1.0 - smoothstep(0.99, 1.0, dist) : 1.0;

  vec2 uv = gl_TexCoord[0].xy * 2.0 - 1.0;
  vec3 normal = vec3(uv, sqrt(1.0 - clamp(dot(uv, uv), 0.0, 1.0)));
  vec3 map = 0.5 + 0.5 * normal;

  vec4 vert = vec4(normal, 0.0);
  vert = rotateX(u_Rotation.x) * vert;
  vert = rotateZ(u_Rotation.z) * vert;
  vert = rotateY(u_Rotation.y) * vert;

  vec3 light = normalize(vec3(lightPosition.x, lightPosition.y, 1));
  float brightness = clamp(dot(light, normal), 0.1, 1.0);

  vec3 diffuse = colorLookup(vert.xyz, u_Number) * brightness;
  float spec = specular(light, normal);

  gl_FragColor = vec4(diffuse + spec, alpha);
}
