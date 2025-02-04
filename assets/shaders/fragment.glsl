#version 330 core

in vec3 crntPos;
in vec3 normal;
in vec3 color;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

vec4 pointLight() {
   vec3 lightVector = lightPos - crntPos;
   float lightDistance = length(lightVector);

   // Attenuation parameters
   float a = 1.0f;
   float b = 0.5f;
   float intensity = 1.0f / (a * lightDistance * lightDistance + b * lightDistance + 1.0f);

   // Ambient lighting
   float ambient = 0.2f;

   // Diffuse lighting
   vec3 normal = normalize(normal);
   vec3 lightDir = normalize(lightVector);
   float diffuse = max(dot(normal, lightDir), 0.0f);

   // Specular lighting
   float specularLight = 0.5f;
   vec3 viewDir = normalize(cameraPos - crntPos);
   vec3 reflectDir = reflect(-lightDir, normal);
   float specularAmount = pow(max(dot(viewDir, reflectDir), 0.0f), 16);
   float specular = specularAmount * specularLight;

   return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

vec4 directionalLight() {
   // Ambient lighting
   float ambient = 0.2f;

   // Diffuse lighting
   vec3 normal = normalize(normal);
   vec3 lightDir = normalize(vec3(1.0f, 1.0f, 0.0f));
   float diffuse = max(dot(normal, lightDir), 0.0f);

   // Specular lighting
   float specularLight = 0.5f;
   vec3 viewDir = normalize(cameraPos - crntPos);
   vec3 reflectDir = reflect(-lightDir, normal);
   float specularAmount = pow(max(dot(viewDir, reflectDir), 0.0f), 16);
   float specular = specularAmount * specularLight;

   return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight() {
   
   float innerCone = 0.95f;
   float outerCone = 0.90f;

   // Ambient lighting
   float ambient = 0.2f;

   // Diffuse lighting
   vec3 normal = normalize(normal);
   vec3 lightDir = normalize(lightPos - crntPos);
   float diffuse = max(dot(normal, lightDir), 0.0f);

   // Specular lighting
   float specularLight = 0.5f;
   vec3 viewDir = normalize(cameraPos - crntPos);
   vec3 reflectDir = reflect(-lightDir, normal);
   float specularAmount = pow(max(dot(viewDir, reflectDir), 0.0f), 16);
   float specular = specularAmount * specularLight;

   float theta = dot(vec3(0.0f, -1.0f, 0.0f), -lightDir);
   float intensity = clamp((theta - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

   return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

void main() {
   FragColor = pointLight();
} 