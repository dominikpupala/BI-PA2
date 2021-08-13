#include "RenderManager.h"

RenderManager::RenderManager()
    : _window(nullptr), _renderer(nullptr) { }

RenderManager::~RenderManager()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    _renderer = nullptr;
    _window = nullptr;
    SDL_Quit();
}

void RenderManager::Init(const std::string& title, int32_t x, int32_t y, int32_t width, int32_t height, bool fullscreen, Color color)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) // returns 0 on success 
        throw RenderManagerException("Initialiazing SDL failed!");

    if ((_window = SDL_CreateWindow(title.c_str(), x, y, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0)) == nullptr)
        throw RenderManagerException("Creating SDL window failed!");

    if ((_renderer = SDL_CreateRenderer(_window, -1, 0)) == nullptr)
        throw RenderManagerException("Creating SDL renderer failed!");

    SetClearColor(color);
}

void RenderManager::Draw(SDL_Texture* texture, const SDL_Rect& rectangle) const
{
    SDL_RenderCopy(_renderer, texture, nullptr, &rectangle);
}

void RenderManager::Clear() const
{
    SDL_RenderClear(_renderer);
}

void RenderManager::Present() const
{
    SDL_RenderPresent(_renderer);
}

void RenderManager::SetClearColor(Color color) const
{
    if (SDL_SetRenderDrawColor(_renderer, color.R, color.G, color.B, color.A) != 0) // returns 0 on success
        throw RenderManagerException("Setting up SDL clear color failed!");
}

SDL_Renderer* RenderManager::GetRenderer() const
{
    if (_renderer == nullptr)
        throw RenderManagerException("SDL renderer is not initialized!");

    return _renderer;
}
