#version 130 

uniform bool u_ShadowMappingEnabled;
uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform float u_LightRadius;
uniform float u_DiffuseIntensity;
uniform float u_AmbientIntensity;
uniform vec2 u_BallPositions[16];
uniform vec2 u_Size;
uniform int u_BallCount;
uniform sampler2D u_Texture;
uniform float u_BallRadiusSqr;

bool intersectSphere(vec3 sphereCenter, vec3 rayOrigin, vec3 rayDirection) {
    vec3 L = sphereCenter - rayOrigin;
    float tca = dot(L, rayDirection);
    float d2 = dot(L, L) - tca * tca;
    if (d2 > u_BallRadiusSqr) {
        return false;
    }
    float thc = sqrt(u_BallRadiusSqr - d2);
    float t0 = tca - thc;
    if (t0 > t1) {
        float temp = t0;
        float t1 = tca + thc;
        t0 = t1;
        t1 = temp;
    }
    if (t0 < 0) {
        t0 = t1;
        if (t0 < 0) {
            return false;
        }
    }
    return true;
}

void main() {
  vec3 pixelPosition = vec3(gl_TexCoord[0].xy * u_Size - u_Size * 0.5, 0.0);

  vec3 directionToLight = u_LightPosition - pixelPosition;
  float distanceToLight = length(directionToLight);
  directionToLight = normalize(directionToLight);

  float diffuse = clamp(dot(directionToLight, vec3(0.0, 0.0, 1.0)), 0.0, 1.0) * clamp(distanceToLight / u_LightPosition.z, 0.0, 1.0) * u_DiffuseIntensity;
  gl_FragColor = texture2D(u_Texture, gl_TexCoord[0].xy) * vec4(u_LightColor.xyz, 1.0f) * (diffuse + u_AmbientIntensity);

  if(u_ShadowMappingEnabled) {
      bool isOccluded = false;
      for(int i=0; i<u_BallCount; ++i) {
        vec3 ballPosition = vec3(u_BallPositions[i].xy, 0.0);
        if(intersectSphere(ballPosition, pixelPosition, directionToLight)) {
            isOccluded = true;
            break;
        }
      }

      if(isOccluded) 
        gl_FragColor *= vec4(0.1, 0.1, 0.1, 1.0);
  }
}
