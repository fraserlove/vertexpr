#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

out vec3 fPos;
out vec3 fNormal;
out vec3 fColor;
out vec2 fTexCoord;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main() {
   // Position of the vertex in the world space
   fPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0));

   fNormal = aNormal;
   fColor = aColor;
   fTexCoord = aTexCoord;

   // Position of the vertex in the clip space
   gl_Position = transform * vec4(fPos, 1.0);
} 