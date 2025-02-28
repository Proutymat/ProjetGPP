#include "Camera.hpp"
#include "Game.hpp"
#include <iostream>

/* CONSTRUCTORS */

Camera::Camera(Game* game) : game(game) {
    // Create the camera seed
    std::random_device rd;
    seed = std::mt19937(rd());

    lastShakeUpdate = game->gameTime;
}


/* METHODS */

void Camera::setShake(float time, float amplitude) {
    // Change shakeTime only if the new time is greater
    if (time > shakeTime || time < 0) {
        shakeTime = time;
        lastShakeUpdate = game->gameTime;
        shakeAmplitude = amplitude;
    }
}

void Camera::makeCameraShake() {
    // Calculating target offsets for camera shake
    float targetX = rand_float(seed) * shakeAmplitude;
    float targetY = rand_float(seed) * shakeAmplitude;

    // Applying camera shake by randomly adding or subtracting target offsets
    rand_bool(seed) ? shakeX += targetX : shakeX -= targetX;
    rand_bool(seed) ? shakeY += targetY : shakeY -= targetY;
}

void Camera::checkShake() {
    shakeX = 0;
    shakeY = 0;
    
    // Shake for a given time
    if (shakeTime > 0) {
        makeCameraShake();
        shakeTime -= game->gameTime - lastShakeUpdate;
        lastShakeUpdate = game->gameTime;
        if (shakeTime < 0) shakeTime = 0;
    }
    // Shake indefinitely
    else if (shakeTime < 0) {
        makeCameraShake();
    }
}

void Camera::update(float deltaTime)
{
    mouseX = sf::Mouse::getPosition(*game->win).x + x;
    mouseY = sf::Mouse::getPosition(*game->win).y + y;
    
    // Lerppppp motherfucker
    auto blend = static_cast<float>(1.0f - std::pow(0.5F, deltaTime * lerpSmoothingFactor));
    if (x - game->player.xx > 5 || x - game->player.xx < -5 ) x -= (x - game->player.xx) * blend;
    if (y - game->player.yy > 5 || y - game->player.yy < -5 ) y -= (y - game->player.yy) * blend;

    //printf("CAMERA %f %f\n",x, y);
    
    // Shaaaake bitch
    checkShake();

    //  std::cout << "CAMERA : " << x << " " << y << std::endl;

    auto view = game->win->getDefaultView();
    view.setCenter(x + shakeX, y + shakeY);
    game->win->setView(view);
}

void Camera::imgui()
{
    // Camera header
    if (ImGui::CollapsingHeader("Camera")) {
        ImGui::Value("x", x);
        ImGui::Value("y", y);
        ImGui::Value("ShakeTime", shakeTime);

        // Activate or deactivate the camera shake
        if (ImGui::Button("Shake")) {
            if (shakeTime == -1) setShake(0, 2);
            else setShake(-1, 2);
        }

        // Set the amplitude if the camera is shaking
        if (shakeTime != 0) {
            ImGui::SliderFloat("Shame Amplitude", &shakeAmplitude, 0, 100);
        }
    }
}