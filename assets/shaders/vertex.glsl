#version 330 core

// Vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

out vec3 fPos;
out vec3 fNormal;
out vec3 fColor;
out vec2 fTexCoord;

// Camera view and projection
uniform mat4 cameraTransform;

// Vertex transform
uniform mat4 transform;

void main() {
   // Position of the model in the world space
   fPos = vec3(transform * vec4(aPos, 1.0));

   // Position of the vertex in the clip space
   gl_Position = cameraTransform * vec4(fPos, 1.0);
   
   // Normals are not affected by model translation or scaling, only rotation
   fNormal = normalize(mat3(transpose(inverse(transform))) * aNormal);

   fColor = aColor;
   fTexCoord = aTexCoord;
} 