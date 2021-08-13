#pragma once

#include "Utility.h"

#include <SDL2/SDL.h>
#include <string>

/**
 * @brief Class used for wrapping exception context from the RenderManager class.
*/
class RenderManagerException : public std::exception
{
private:
    std::string _sdl;
    std::string _message;

public:
    /**
     * @brief Create a new instance of the object.
     * @param message Programmer given context.
    */
    inline RenderManagerException(const std::string& message) : _sdl(SDL_GetError()), _message(message) { }
    /**
     * @brief Message getter.
     * @return Exception context.
    */
    inline std::string Message() const { return _message + " [" + _sdl + "]"; }
    /**
     * @brief Exception specifier.
     * @return Exception specifier. 
    */
    inline const char* what() const noexcept override { return "RenderManagerException"; }
};

/**
 * @brief Class used for rendering. Wrapper around the SDL_Window & the SDL_Renderer functionality.
 */
class RenderManager
{
private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;

public:
    /**
    * @brief Create a new instance of the object with uininitialized SDL context.
    */
    RenderManager();
    /**
     * @brief Free initialized SDL context before destroying a instance of the object.
     */
    ~RenderManager();

    /**
     * @brief Initialize SDL context.
     * @param title Window title.
     * @param x Window position on horizontal axis.
     * @param y Window position on vertical axis.
     * @param width Window width.
     * @param height Window height.
     * @param fullscreen Fullscreen flag.
     * @param color Window clear color.
     */
    void Init(const std::string& title, int32_t x, int32_t y, int32_t width, int32_t height, bool fullscreen, Color color);
    /**
     * @brief Buffer SDL draw data.
     * @param texture Texture to be drawn.
     * @param rectangle Rectangle to be drawn to.
     */
    void Draw(SDL_Texture* texture, const SDL_Rect& rectangle) const;
    /**
     * @brief Clear screen.
    */
    void Clear() const;
    /**
     * @brief Present all the buffered SDL draw data.
    */
    void Present() const;
    /**
     * @brief Set a new clear color.
     * @param color Desired clear color.
    */
    void SetClearColor(Color color) const;
    /**
     * @brief Renderer getter.
     * @return Pointer to the initialized renderer.
    */
    SDL_Renderer* GetRenderer() const;
};

