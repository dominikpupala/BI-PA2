#include "ScoreCounter.h"

ScoreCounter::ScoreCounter() : _score(0), _hitMultiplier(0), _speedMultiplier(SPEED_MULTIPLIER) { }

void ScoreCounter::AddScore()
{
    _score += (++_hitMultiplier) * SCORE_PER_BRICK * _speedMultiplier;
}

void ScoreCounter::AddBonusScore(uint32_t bonus)
{
    _score += bonus;
}

void ScoreCounter::ResetMultiplier()
{
    _hitMultiplier = 0;
}

void ScoreCounter::IncreaseSpeedMultiplier()
{
    ++_speedMultiplier;
}

uint32_t ScoreCounter::GetScore() const
{
    return _score;
}
