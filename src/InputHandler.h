#pragma once

#include <SDL2/SDL.h>

/**
 * @brief Class used for handling SDL inputs and events.
*/
class InputHandler
{
public:
    /**
     * @brief Enumclass for the state.
    */
    enum class State { STALE, QUIT } State;
    enum { KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_SPACE, KEY_ESCAPE, KEYS_COUNT };
    bool KeyMap[KEYS_COUNT];

public:
    /**
     * @brief Create a new instance of the object.
    */
    InputHandler();
    /**
     * @brief Process events and inputs.
    */
    void Process();

private:
    /**
     * @brief Process releasing a key.
     * @param keyCode Code of a released key.
    */
    void ProcessKeyUp(const SDL_Keycode& keyCode);
    /**
     * @brief Process pressing a key.
     * @param keyCode Code of a pressed key.
    */
    void ProcessKeyDown(const SDL_Keycode& keyCode);
};

