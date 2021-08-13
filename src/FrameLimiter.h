#pragma once

#include <cstdint>

/**
 * @brief Class used for limiting framerate.
*/
class FrameLimiter
{
private:
    uint32_t _tick;
    uint32_t _elapsed;
    const uint32_t _targeted;

public:
    /**
     * @brief Create a new instance of the object.
     * @param targetFPS Desired framerate.
    */
    FrameLimiter(uint32_t targetFPS);
    /**
     * @brief Set the start of the frame.
    */
    void Start();
    /**
     * @brief Set the end of the frame. Delay in case of exceeding framerate.
    */
    void End();
};
