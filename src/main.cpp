#include "model.h"
#include "light.h"
#include "display.h"
#include "primitives.h"

int main() {
    Display display(800, 800, "OpenGL");

    Shader shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

    Light light(shader, glm::vec3(0.0f, 0.5f, 1.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    Camera camera(display.getWidth(), display.getHeight(), glm::vec3(0.0f, 0.0f, 4.0f), 45.0f, 0.1f, 1000.0f);

    Mesh plane = Primitives::plane(Texture("assets/textures/testDiffuse.png", "diffuse", 0), Texture("assets/textures/testSpecular.png", "specular", 1));
    plane.translate(glm::vec3(0.0f, -1.0f, 0.0f));
    plane.scale(glm::vec3(4.0f, 4.0f, 4.0f));

    Model model("assets/models/test/scene.gltf");

    while(display.isActive()) {
        
        model.rotate(glm::angleAxis(glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

        camera.inputs(display.getWindow());
        camera.update();
        plane.draw(shader, camera);
        model.draw(shader, camera);
        display.update();
    }

    return 0;
}