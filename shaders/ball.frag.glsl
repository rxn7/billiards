#version 130 

#define PI 3.14159265358

uniform sampler2D u_NumbersTexture;

uniform vec3 u_Color;

uniform vec2 u_CameraToBallDirection;
uniform vec3 u_LightToBallDirection;
uniform mat3 u_RotationMatrix;

uniform int u_Number;

uniform vec3 u_LightColor;
uniform float u_DiffuseIntensity;
uniform float u_SpecularIntensity;
uniform float u_Shininess;
uniform float u_AmbientIntensity;

const float numberTextArea = 0.28;
const vec3 white = vec3(1.0);
const vec2 center = vec2(0.5, 0.5);
const int numberTextureGridSize = 4;
const float numberTextureGridSizeInverse = 1.0 / numberTextureGridSize;

vec3 getColorAtPoint(vec3 point) {
  int number = u_Number;

  // White ball is completely white
  if(number == 0)
    return white;

  // Convert the number to be 0-indexed
  number--;

  // If the point is inside the numberTextArea
  if(abs(point.x) < numberTextArea && abs(point.y) < numberTextArea) {
    vec2 uv = (point.xy + vec2(numberTextArea)) / (numberTextArea * 2);

    // The number on the back would be flipped without this 
    if(point.z < 0.0)
      uv.x = (1.0 - uv.x);

    // Get the uv of a number
    uv *= numberTextureGridSizeInverse;
    uv.x += (number % numberTextureGridSize) * numberTextureGridSizeInverse;
    uv.y += number / numberTextureGridSize * numberTextureGridSizeInverse;

    return texture2D(u_NumbersTexture, uv).rgb;;
  }

  // Render stripes
  if(number > 7 && abs(point.y) > 0.55)
      return mix(u_Color, white, smoothstep(0.55, 0.56, abs(point.y)));

  // Render the white circle in the middle
  float dist = distance(point.xy, vec2(0.0));
  if(dist < 0.4) 
    return white;

  // Smooth out the circle
  dist = smoothstep(0.4, 0.41, dist);

  return mix(white, u_Color, dist);
}

void main() {
  float dist = distance(center, gl_TexCoord[0].xy) * 2.0;
  float alpha = 1.0 - smoothstep(0.99, 1.0, dist);

  if(alpha == 0.0f)
    return;

  vec2 uv = gl_TexCoord[0].xy * 2.0 - 1.0;
  vec3 normal = vec3(uv, sqrt(1.0 - clamp(dot(uv, uv), 0.0, 1.0)));

  float diffuse = u_DiffuseIntensity * max(dot(normal, u_LightToBallDirection), 0.0);
  float specular = u_SpecularIntensity * pow(max(dot(u_LightToBallDirection, reflect(-u_LightToBallDirection, normal)), 0.0), u_Shininess);

  vec3 color = getColorAtPoint(normal * u_RotationMatrix) * u_LightColor * (u_AmbientIntensity + diffuse) + white * specular;

  gl_FragColor = vec4(color, alpha);
}
