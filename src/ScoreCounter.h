#pragma once

#include <cstdint>

/**
 * @brief Class used for score counting.
*/
class ScoreCounter
{
private:
    static const uint32_t SCORE_PER_BRICK = 25;
    static const uint32_t SPEED_MULTIPLIER = 4;

    uint32_t _score;
    uint32_t _hitMultiplier;
    uint32_t _speedMultiplier;

public:
    /**
     * @brief Create a new instance of the object.
    */
    ScoreCounter();
    /**
     * @brief Add score to the counter.
    */
    void AddScore();
    /**
     * @brief Add bonus score to the counter.
     * @param bonus Amount of the bonus.
    */
    void AddBonusScore(uint32_t bonus);
    /**
     * @brief Reset the multiplier.
    */
    void ResetMultiplier();
    /**
     * @brief Increase speed multiplier.
    */
    void IncreaseSpeedMultiplier();

    /**
     * @brief Score getter.
     * @return Value to the score.
    */
    uint32_t GetScore() const;
};

