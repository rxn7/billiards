#version 130 

uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform float u_LightRadius;
uniform float u_DiffuseIntensity;
uniform float u_AmbientIntensity;
uniform vec2 u_BallPositions[16];
uniform vec2 u_Size;
uniform int u_BallCount;
uniform sampler2D u_Texture;
uniform float u_BallRadius;

void main() {
  vec3 pixelPosition = vec3(gl_TexCoord[0].xy * u_Size - u_Size * 0.5, 0.0);
  vec3 deltaToLight = u_LightPosition - pixelPosition;
  vec3 lightDirection = normalize(deltaToLight);
  float distanceToLight = length(deltaToLight);

  float brightness = clamp(dot(lightDirection, vec3(0.0, 0.0, 1.0)), 0.0, 1.0) * clamp(distanceToLight / u_LightPosition.z, 0.0, 1.0) + u_AmbientIntensity;

  gl_FragColor = texture2D(u_Texture, gl_TexCoord[0].xy) * vec4(u_LightColor.xyz, 1.0f) * brightness;
}
