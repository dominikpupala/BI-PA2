#include "FrameLimiter.h"

#include <SDL2/SDL.h>

FrameLimiter::FrameLimiter(uint32_t targetFPS)
    : _tick(0), _elapsed(0), _targeted(1000 / targetFPS) { }

void FrameLimiter::Start()
{
    _tick = SDL_GetTicks();
}

void FrameLimiter::End()
{
    if (_targeted > (_elapsed = SDL_GetTicks() - _tick))
        SDL_Delay(_targeted - _elapsed);
}
