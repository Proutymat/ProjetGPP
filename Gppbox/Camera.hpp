#pragma once

#include <random>
#include <imgui.h>

class Game;

class Camera
{
public:

    /* ATTRIBUTES */

    float x = 0; /**< The x-coordinate of the camera's position */
    float y = 0; /**< The y-coordinate of the camera's position */
    int mouseX = 0; /**< The x-coordinate of the camera's mouse position */
    int mouseY = 0; /**< The y-coordinate of the camera's mouse position */

    float lerpSmoothingFactor = 11;

    // SHAKING ATTRIBUTES
    std::mt19937 seed; /**< The random number generator */
    std::uniform_int_distribution<int> rand_bool{0, 1}; /**< The random boolean generator */
    std::uniform_real_distribution<float> rand_float{0.0, 1.0}; /**< The random float generator */
    float shakeX = 0; /**< The x-coordinate of the camera's shake */
    float shakeY = 0; /**< The y-coordinate of the camera's shake */
    int shakeTime = 0; /**< The time to shake the camera, positives is the time to shake, 0 is not shaking, negatives shakes indefinitely */
    double lastShakeUpdate; /**< The time of the last shake update */
    float shakeAmplitude = 2; /**< The amplitude of the camera shake */

    
    /* CONSTRUCTORS */

    Camera(Game* game);


    /* METHODS */
    
    void setShake(int time, float amplitude = 2);
    void checkShake();
    void update(float deltaTime);
    void imgui();

private:
    Game* game;

    void makeCameraShake();
    
};
