#version 330 core

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 crntPos;

out vec4 FragColor;

uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main() {
   // Ambient
   float ambient = 0.2f;

   // Diffuse
   vec3 normal = normalize(normal);
   vec3 lightDir = normalize(lightPos - crntPos);
   float diffuse = max(dot(normal, lightDir), 0.0f);

   // Specular
   float specularLight = 0.5f;
   vec3 viewDir = normalize(cameraPos - crntPos);
   vec3 reflectDir = reflect(-lightDir, normal);
   float specularAmount = pow(max(dot(viewDir, reflectDir), 0.0f), 8);
   float specular = specularAmount * specularLight;

   FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
} 