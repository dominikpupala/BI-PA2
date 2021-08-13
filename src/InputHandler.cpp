#include "InputHandler.h"

InputHandler::InputHandler()
    : State(State::STALE)
{
    for (size_t i = 0; i < KEYS_COUNT; ++i)
        KeyMap[i] = false;
}

void InputHandler::Process()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        State = State::QUIT;
        break;
    case SDL_KEYDOWN:
        ProcessKeyDown(event.key.keysym.sym);
        break;
    case SDL_KEYUP:
        ProcessKeyUp(event.key.keysym.sym);
        break;
    default:
        break;
    }
}

void InputHandler::ProcessKeyUp(const SDL_Keycode& keyCode)
{
    switch (keyCode)
    {
    case SDLK_LEFT:
        KeyMap[KEY_LEFT_ARROW] = false;
        break;
    case SDLK_RIGHT:
        KeyMap[KEY_RIGHT_ARROW] = false;
        break;
    case SDLK_UP:
        KeyMap[KEY_UP_ARROW] = false;
        break;
    case SDLK_DOWN:
        KeyMap[KEY_DOWN_ARROW] = false;
        break;
    case SDLK_SPACE:
        KeyMap[KEY_SPACE] = false;
        break;
    case SDLK_ESCAPE:
        KeyMap[KEY_ESCAPE] = false;
        break;
    default:
        break;
    }
}

void InputHandler::ProcessKeyDown(const SDL_Keycode& keyCode)
{
    switch (keyCode)
    {
    case SDLK_LEFT:
        KeyMap[KEY_LEFT_ARROW] = true;
        break;
    case SDLK_RIGHT:
        KeyMap[KEY_RIGHT_ARROW] = true;
        break;
    case SDLK_UP:
        KeyMap[KEY_UP_ARROW] = true;
        break;
    case SDLK_DOWN:
        KeyMap[KEY_DOWN_ARROW] = true;
        break;
    case SDLK_SPACE:
        KeyMap[KEY_SPACE] = true;
        break;
    case SDLK_ESCAPE:
        KeyMap[KEY_ESCAPE] = true;
        break;
    default:
        break;
    }
}
