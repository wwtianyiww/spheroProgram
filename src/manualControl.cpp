#include "manualControl.hpp"
#include <SDL3/SDL.h>
#include <vector>

ManualControl::ManualControl()
    : gamepad_(nullptr)
    , running_(true)
    , leftStickX_(0.0f)
    , leftStickY_(0.0f)
    , rightStickX_(0.0f)
    , rightStickY_(0.0f)
    , leftTrigger_(0.0f)
    , rightTrigger_(0.0f)
     {}

ManualControl::~ManualControl() {
    if (gamepad_) {
        SDL_CloseGamepad(gamepad_);
    }
    SDL_Quit();
}

bool ManualControl::init() {
    // Initialize SDL with gamepad subsystem
    if (!SDL_Init(SDL_INIT_GAMEPAD)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Get number of connected gamepads
    int num_joysticks = 0;
    SDL_JoystickID* joysticks = SDL_GetJoysticks(&num_joysticks);

    if (num_joysticks == 0) {
        std::cerr << "No gamepads connected!" << std::endl;
        SDL_free(joysticks);
        return false;
    }

    std::cout << "Found " << num_joysticks << " gamepad(s)" << std::endl;

    // Open the first gamepad
    gamepad_ = SDL_OpenGamepad(joysticks[0]);
    SDL_free(joysticks);

    if (!gamepad_) {
        std::cerr << "Failed to open gamepad: " << SDL_GetError() << std::endl;
        return false;
    }

    std::cout << "Gamepad opened: " << SDL_GetGamepadName(gamepad_) << std::endl;
    return true;
}

void ManualControl::update() {
    SDL_Event event;

    // Poll for events
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                running_ = false;
                break;

            case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                // Axis values are -32768 to 32767, normalize to -1.0 to 1.0
                float value = event.gaxis.value / 32767.0f;

                switch (event.gaxis.axis) {
                    case SDL_GAMEPAD_AXIS_LEFTX: leftStickX_ = value; break;
                    case SDL_GAMEPAD_AXIS_LEFTY: leftStickY_ = value; break;
                    case SDL_GAMEPAD_AXIS_RIGHTX: rightStickX_ = value; break;
                    case SDL_GAMEPAD_AXIS_RIGHTY: rightStickY_ = value; break;
                    case SDL_GAMEPAD_AXIS_LEFT_TRIGGER: leftTrigger_ = value; break;
                    case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER: rightTrigger_ = value; break;
                }
                break;
        }
    }
}

void ManualControl::printState() {
    // Clear screen (optional)
    std::cout << "\033[2J\033[1;1H";  // ANSI escape codes

    std::cout << "=== Xbox Controller State ===" << std::endl;
    std::cout << "Left Stick:  X=" << leftStickX_ << " Y=" << leftStickY_ << std::endl;
    std::cout << "Right Stick: X=" << rightStickX_ << " Y=" << rightStickY_ << std::endl;
    std::cout << "Triggers:    L=" << leftTrigger_ << " R=" << rightTrigger_ << std::endl;
    std::cout << "Buttons: ";
    std::cout << std::endl;
    std::cout << "Press Ctrl+C to exit" << std::endl;
}



void ManualControl::movement() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                running_ = false;
                break;
            case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                float value = event.gaxis.value / 32767.0f;

                switch (event.gaxis.axis) {
                        //turn left right logic
                    case SDL_GAMEPAD_AXIS_LEFTX < 0:
                        leftStickX_ = value;


                        break;
                    //case SDL_GAMEPAD_AXIS_LEFTY: leftStickY_ = value; break;
                        //90 degree rotation left or right logic
                    case SDL_GAMEPAD_AXIS_LEFT_TRIGGER: leftTrigger_ = value; break;
                    case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER: rightTrigger_ = value; break;
                }
        }

    }


}