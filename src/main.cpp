#include "Animation\ProtoDRMorphAnimation.h"
#include "Animation\CubeAnimation.h"
#include "Controllers\ProtoDRController.h"

const uint8_t maxBrightness = 20;
ProtoDRController protoDRController = ProtoDRController(maxBrightness, ProtoDRController::LEFT);
ProtoDRMorphAnimation protoDRMorph;
CubeAnimation cube;

void setup() {
    Serial.begin(115200);
    Serial.println("\nStarting...");
    
    leds.begin();

    protoDRController.Initialize();
}

void loop() {
    for (float i = 0.0f; i < 1.0f; i += 1.0f / 720.0f) {
        protoDRMorph.Update(i);
        //cube.Update(i);

        protoDRController.Render(protoDRMorph.GetScene());
        //protoDRController.Render(cube.GetScene());
        protoDRController.Display();

        Serial.print("Rendered in ");
        Serial.print(protoDRController.GetRenderTime(), 4);
        Serial.println("s");
    }
}
