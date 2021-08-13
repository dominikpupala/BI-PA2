#pragma once

#include <SDL2/SDL_image.h>
#include <string>
#include <memory>

/**
 * @brief Class used for wrapping exception context from the TextureLoader class.
*/
class TextureLoaderException : public std::exception
{
private:
    std::string _sdl;
    std::string _message;

public:
    /**
     * @brief Create a new instance of the object.
     * @param message Programmer given context.
    */
    inline TextureLoaderException(const std::string& message) : _sdl(SDL_GetError()), _message(message) { }
    /**
     * @brief Message getter.
     * @return Exception context.
    */
    inline std::string Message() const { return _message + " [" + _sdl + "]"; }
    /**
     * @brief Exception specifier.
     * @return Exception specifier.
    */
    inline const char* what() const noexcept override { return "TextureLoaderException"; }
};

/**
 * @brief Class used for storing texture. Wrapper around the SDL_Texture.
*/
class Texture
{
private:
    SDL_Texture* _texture;

public:
    /**
     * @brief Create a new instance of the object.
     * @param texture Pointer to the allocated texture.
    */
    Texture(SDL_Texture* texture);
    /**
     * @brief Free loaded SDL_Texture before destroying a instance of the object.
     */
    ~Texture();

    /**
     * @brief Texture getter.
     * @return Pointer to the loaded texture.
    */
    SDL_Texture* GetTexture() const;
};

/**
 * @brief Class used for loading the Texture objects.
*/
class TextureLoader
{
public:
    /**
     * @brief Load texture from the file.
     * @param path File path.
     * @param renderer Renderer to load the texture.
     * @return Smart pointer to the Texture object.
    */
    static std::shared_ptr<Texture> Load(const std::string& path, SDL_Renderer* renderer);
};
