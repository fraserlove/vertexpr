#version 330 core

in vec3 fPos;
in vec3 fNormal;
in vec3 fColor;
in vec2 fTexCoord;

out vec4 FragColor;

// Diffuse and specular textures
uniform sampler2D diffuse0;
uniform sampler2D specular0;

// Light color and position
uniform vec4 lightColor;
uniform vec3 lightPos;

// Camera position
uniform vec3 cameraPos;

// TODO: Make this a uniform
vec3 emissionDir = normalize(vec3(0.0, 1.0, 1.0));

vec4 calculateLighting(vec3 lightDir, float intensity) {
   // Ambient lighting
   float ambient = 0.2;

   // Diffuse lighting
   vec3 normal = normalize(fNormal);
   float diffuse = max(dot(normal, lightDir), 0.0);

   // Specular lighting (Blinn-Phong)
   float specular = 0.0;
   if (diffuse > 0.0) {
      float specularLight = 0.5;
      vec3 viewDir = normalize(cameraPos - fPos);
      vec3 reflectDir = reflect(-lightDir, normal);
      vec3 halfwayDir = normalize(lightDir + viewDir);
      float specularAmount = pow(max(dot(normal, halfwayDir), 0.0), 16);
      specular = specularAmount * specularLight;
   }

   return (texture(diffuse0, fTexCoord) * (diffuse * intensity + ambient) + texture(specular0, fTexCoord).r * specular * intensity) * lightColor;
}

vec4 pointLight() {
   vec3 lightDir = normalize(lightPos - fPos);
   float lightDistance = length(lightPos - fPos);
   
   // Quadratic attenuation parameters
   float a = 0.5;
   float b = 0.3;

   float intensity = 1.0 / (a * lightDistance * lightDistance + b * lightDistance + 1.0);
   
   return calculateLighting(lightDir, intensity);
}

vec4 spotLight() {
   vec3 lightDir = normalize(lightPos - fPos);
   float theta = dot(emissionDir, lightDir);

   // Spot light parameters
   float innerCone = 0.95;
   float outerCone = 0.90;

   float intensity = clamp((theta - outerCone) / (innerCone - outerCone), 0.0, 1.0);
   
   return calculateLighting(lightDir, intensity);
}

vec4 directionalLight() {
   return calculateLighting(emissionDir, 1.0);
}

void main() {
    FragColor = pointLight();
}