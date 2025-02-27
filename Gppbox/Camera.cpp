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

void Camera::setShake(int time, float amplitude) {
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
        shakeTime -= static_cast<int>(game->gameTime - lastShakeUpdate);
        lastShakeUpdate = game->gameTime;
        if (shakeTime < 0) shakeTime = 0;
    }
    // Shake indefinitely
    else if (shakeTime < 0) {
        makeCameraShake();
    }
}

void Camera::update()
{
    checkShake();
    
    x = game->player->xx + shakeX;
    y = game->player->yy + shakeY;
    
    auto view = game->win->getDefaultView();
    view.setCenter(x, y);
    game->win->setView(view);
}

void Camera::imgui()
{
    // Camera header
    if (ImGui::CollapsingHeader("Camera")) {
        ImGui::Value("x", x);
        ImGui::Value("y", y);

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