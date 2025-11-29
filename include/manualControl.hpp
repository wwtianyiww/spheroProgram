#ifndef PROGRAMSPHERO_MANUALCONTROL_HPP
#define PROGRAMSPHERO_MANUALCONTROL_HPP
#include <SDL3/SDL.h>
#include <iostream>
class ManualControl {
public:
    ManualControl();
    ~ManualControl();

    bool init();
    void update();
    void printState();
    bool isRunning() const { return running_; }
    void movement();

private:
    SDL_Gamepad* gamepad_;
    bool running_;

    // Controller state
    float leftStickX_;
    float leftStickY_;
    float rightStickX_;
    float rightStickY_;
    float leftTrigger_;
    float rightTrigger_;
    float RPWM;
    float LPWM;


};



#endif //PROGRAMSPHERO_MANUALCONTROL_HPP