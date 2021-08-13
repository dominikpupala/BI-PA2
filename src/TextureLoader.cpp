#include "TextureLoader.h"

Texture::Texture(SDL_Texture* texture)
    : _texture(texture) { }

Texture::~Texture()
{
    SDL_DestroyTexture(_texture);
    _texture = nullptr;
}

SDL_Texture* Texture::GetTexture() const
{
    return _texture;
}

std::shared_ptr<Texture> TextureLoader::Load(const std::string& path, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // free surface before possibble exception

    if (texture == nullptr)
        throw TextureLoaderException("Loading texture has failed!");

    return std::make_shared<Texture>(texture);
}
