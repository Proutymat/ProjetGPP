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
     sf::View view = game->win->getView();
 
     // Lerp the camera to the player's position
     view.setCenter(game->player->xx + shakeX, game->player->yy + shakeY);
 
     // Apply the view to the window
     game->win->setView(view);

    std::cout << game->player->xx << std::endl;
}