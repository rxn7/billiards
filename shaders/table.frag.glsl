#version 130 

uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform float u_DiffuseIntensity;
uniform float u_AmbientIntensity;
uniform vec2 u_BallPositions[16];
uniform vec2 u_Size;
uniform int u_BallCount;
uniform sampler2D u_Texture;
uniform float u_BallRadiusSqr;

bool intersect(vec3 center, vec3 rayOrigin, vec3 rayDirection) {
  vec3 centerRelativeToRay = center - rayOrigin;

  float distanceToClosestPoint = dot(centerRelativeToRay, rayDirection);
  float d2 = dot(centerRelativeToRay, centerRelativeToRay) - distanceToClosestPoint * distanceToClosestPoint;

  return d2 <= u_BallRadiusSqr && distanceToClosestPoint - sqrt(u_BallRadiusSqr - d2) > 0.0;
}

void main() {
  vec3 pixelPosition = vec3(gl_TexCoord[0].xy * u_Size - u_Size * 0.5, 0.0);
  vec3 directionToLight = u_LightPosition - pixelPosition;
  float distanceToLight = length(directionToLight);
  directionToLight = normalize(directionToLight);

  float multiplier = 1.0;
  for(int i=0; i<u_BallCount && multiplier == 1.0; ++i)
    multiplier -= float(intersect(vec3(u_BallPositions[i].xy, 0.0), pixelPosition, directionToLight)) * 0.8;

  float diffuse = clamp(dot(directionToLight, vec3(0.0, 0.0, 1.0)), 0.0, 1.0) * clamp(distanceToLight / u_LightPosition.z, 0.0, 1.0) * u_DiffuseIntensity;
  gl_FragColor = texture2D(u_Texture, gl_TexCoord[0].xy) * vec4(u_LightColor.xyz, 1.0f) * (diffuse + u_AmbientIntensity) * multiplier;
}
