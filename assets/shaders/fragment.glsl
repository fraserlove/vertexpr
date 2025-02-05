#version 330 core

in vec3 fPos;
in vec3 fNormal;
in vec3 fColor;
in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

// TODO: Currently hardcoded and will be a uniform later
vec3 emissionDir = normalize(vec3(0.0, 0.0, 1.0));

vec4 calculateLighting(vec3 lightDir, float intensity) {
   // Ambient lighting
   float ambient = 0.2;

   // Diffuse lighting
   vec3 normal = normalize(fNormal);
   float diffuse = max(dot(normal, lightDir), 0.0);

   // Specular highlight
   float specularLight = 0.5;
   vec3 viewDir = normalize(cameraPos - fPos);
   vec3 reflectDir = reflect(-lightDir, normal);
   float specularAmount = pow(max(dot(viewDir, reflectDir), 0.0), 16);
   float specular = specularAmount * specularLight;

   return (texture(diffuse0, fTexCoord) * (diffuse * intensity + ambient) + texture(specular0, fTexCoord).r * specular * intensity) * lightColor;
}

vec4 pointLight() {
   vec3 lightDir = normalize(lightPos - fPos);
   float lightDistance = length(lightPos - fPos);
   
   // Attenuation parameters
   float a = 1.0;
   float b = 0.5;

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