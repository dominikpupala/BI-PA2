#pragma once

#include "GameObjects.h"
#include "InputHandler.h"
#include "FrameLimiter.h"
#include "HighscoreLoader.h"

/**
 * @brief Class used for the main game logic.
*/
class Game
{
private:
    static const uint32_t WINDOW_FPS = 60;
    static const uint32_t WINDOW_WIDTH = 580;
    static const uint32_t WINDOW_HEIGHT = 720;

    static const uint32_t FRAME_BRICK_OFFSET = 34;
    static const uint32_t FRAME_WIDTH_OFFSET = 16;
    static const uint32_t FRAME_HEIGHT_OFFSET = 3;

    static const uint32_t BRICK_ROWS = 10;
    static const uint32_t BRICK_COLUMNS = 8;

    static const int32_t INITIAL_LIVES = 3;
    static const int32_t INITIAL_SPEED_BALL = 5;
    static const int32_t INITIAL_SPEED_PLAYER = 7;
    static const int32_t INITIAL_BONUS_PROPABILITY = 31;

	/**
	 * @brief Enumclass for the application state.
	*/
	enum class AppState
	{
		DEFAULT,
		RUNNING,
		QUITING,
	} _appState;

    /**
     * @brief Enumclass for the game state.
    */
    enum class GameState
    {
        IDLE,
        PLAY,
        STOP
    } _gameState;

    MapLoader _maper;
    InputHandler _input;
    FrameLimiter _framer;
    ScoreCounter _counter;
    RenderManager _renderer;
    HighscoreLoader _scorer;

    std::string _playerName;

    std::vector<std::shared_ptr<IDrawable>> _drawContext;

    std::shared_ptr<Health> _lives;
    std::shared_ptr<BrickManager> _bricks;
    std::shared_ptr<BonusManager> _bonuses;

    std::shared_ptr<Background> _background;
    std::shared_ptr<Background> _loseScreen;
    std::shared_ptr<Background> _winScreen;
    std::shared_ptr<Player> _player;
    std::shared_ptr<Ball> _ball;

public:
	/**
	 * @brief Create a new instance of the game.
	 * @param mapPath Map file path.
	 * @param scorePath Score file path.
	 * @param playerName Player's name.
	*/
	Game(const std::string& mapPath, const std::string& scorePath, const std::string& playerName);

    /**
	 * @brief Initialize the game. Mostly the game resources and the logic states.
	*/
	void Init();
    /**
     * @brief Begin the game loop.
    */
    void Play();

private:
	/**
	 * @brief Draw the game context.
	*/
	void Draw();
	/**
	 * @brief Update the game context.
	*/
	void Update();
	/**
	 * @brief Process events and user inputs for the game.
	*/
	void ProcessEvents();
    /**
     * @brief End the game.
     * @param win Player win flag.
    */
    void EndGame(bool win);
};
