#include <iostream>

#include "Game.h"

Game::Game(const std::string& mapPath, const std::string& scorePath, const std::string& playerName)
    : _appState(AppState::DEFAULT), _gameState(GameState::IDLE), _maper(mapPath, BRICK_ROWS, BRICK_COLUMNS), _framer(WINDOW_FPS), _scorer(scorePath), _playerName(playerName) { }

void Game::Init()
{
    try
    {
        // load map
        Map map = _maper.Load();

        // initialize renderer
        _renderer.Init("Resonating Voidness", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, false, { 0, 0, 0, 255 });

        // initialize textures
        std::shared_ptr<Texture> nebula1 = TextureLoader::Load("assets/Nebula1.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> nebula2 = TextureLoader::Load("assets/Nebula2.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> nebula3 = TextureLoader::Load("assets/Nebula3.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> stars = TextureLoader::Load("assets/Stars.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> frame = TextureLoader::Load("assets/Frame.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> ball = TextureLoader::Load("assets/Ball.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> platform = TextureLoader::Load("assets/Platform.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> brickYellow = TextureLoader::Load("assets/BrickYellow.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> brickGreen = TextureLoader::Load("assets/BrickGreen.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> brickBlue = TextureLoader::Load("assets/BrickBlue.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> brickGray = TextureLoader::Load("assets/BrickGray.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> brickRed = TextureLoader::Load("assets/BrickRed.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> bonusGreen = TextureLoader::Load("assets/BonusGreen.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> bonusBlue = TextureLoader::Load("assets/BonusBlue.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> bonusRed = TextureLoader::Load("assets/BonusRed.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> bonusTeal = TextureLoader::Load("assets/BonusTeal.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> bonusYellow = TextureLoader::Load("assets/BonusYellow.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> bonusPurple = TextureLoader::Load("assets/BonusPurple.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> healthLabel = TextureLoader::Load("assets/LivesLabel.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> endScreen = TextureLoader::Load("assets/EndScreen.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> winLabel = TextureLoader::Load("assets/WinLabel.png", _renderer.GetRenderer());
        std::shared_ptr<Texture> loseLabel = TextureLoader::Load("assets/LoseLabel.png", _renderer.GetRenderer());

        // initialize objects
        _background = std::make_shared<Background>(Background(
            {
                GameObject(nebula1, 0, 0, 751, 564).Clone(),
                GameObject(nebula2, 0, 0, 800, 719).Clone(),
                GameObject(nebula3, 0, 0, 791, 889).Clone(),
                GameObject(stars, 0, 0, 1024, 1024).Clone(),
                GameObject(frame, 0, 0, 580, 720).Clone()
            }));
        _winScreen = std::make_shared<Background>(Background(
            {
                GameObject(endScreen, 0, 0, 580, 720).Clone(),
                GameObject(winLabel, 0, 0, 580, 720).Clone()
            }));
        _loseScreen = std::make_shared<Background>(Background(
            {
                GameObject(endScreen, 0, 0, 580, 720).Clone(),
                GameObject(loseLabel, 0, 0, 580, 720).Clone()
            }));
        _lives = std::make_shared<Health>(Health(ball, healthLabel, INITIAL_LIVES, FRAME_BRICK_OFFSET, WINDOW_HEIGHT - 40, 95, 40, 24, 24));
        _ball = std::make_shared<Ball>(Ball(ball, WINDOW_WIDTH / 2 - 24 / 4, WINDOW_HEIGHT - 120, 24 / 2, 24 / 2, INITIAL_SPEED_BALL));
        _player = std::make_shared<Player>(Player(platform, WINDOW_WIDTH / 2 - 128 / 4, WINDOW_HEIGHT - 59, 128 / 2, 32 / 2, 128, INITIAL_SPEED_PLAYER));
        _bricks = std::make_shared<BrickManager>(BrickManager({ brickGreen, brickYellow, brickBlue, brickRed }, brickGray, map, FRAME_BRICK_OFFSET, FRAME_BRICK_OFFSET - FRAME_WIDTH_OFFSET + FRAME_HEIGHT_OFFSET, 64, 24));
        _bonuses = std::make_shared<BonusManager>(BonusManager({ bonusBlue, bonusGreen, bonusRed, bonusTeal, bonusYellow, bonusPurple }, 24, 24, INITIAL_BONUS_PROPABILITY));

        // set context
        _drawContext = { _background, _lives, _player, _ball, _bricks, _bonuses };
        _appState = AppState::RUNNING;
    }
    catch (const RenderManagerException& e) { std::cout << e.Message() << std::endl; }
    catch (const TextureLoaderException& e) { std::cout << e.Message() << std::endl; }
    catch (const MapLoaderException& e) { std::cout << e.Message() << std::endl; }
}

void Game::Play()
{
    while (_appState == AppState::RUNNING)
    {
        _framer.Start();

        Update();
        Draw();

        _framer.End();
    }
}

void Game::Draw()
{
    _renderer.Clear();

    for (size_t i = 0; i < _drawContext.size(); ++i)
        _drawContext[i]->Draw(_renderer);

    _renderer.Present();
}

void Game::Update()
{
    ProcessEvents();

    // check boundary for moving objects
    _player->CollisionBoundary(FRAME_WIDTH_OFFSET, WINDOW_WIDTH - FRAME_WIDTH_OFFSET);
    _ball->CollisionBoundary(FRAME_WIDTH_OFFSET, FRAME_HEIGHT_OFFSET, WINDOW_WIDTH - FRAME_WIDTH_OFFSET);

    // ball position
    if (_gameState == GameState::PLAY)
    {
        if (_ball->IsUnder(WINDOW_HEIGHT))
        {
            _lives->DecreaseHealth();

            if (_lives->GetHealth() == 0) EndGame(false);
            else _gameState = GameState::IDLE;

            _counter.ResetMultiplier();
            _bonuses->Clear();
            return;
        }

        if (_ball->CollisionCheck(_player))
            _counter.ResetMultiplier();

        _bonuses->CollisionPlayer(*_player, *_ball, _counter);
        _bricks->CollisionBall(*_ball, *_bonuses, _counter);

        if (_bricks->IsFinished())
        {
            EndGame(true);

            _counter.ResetMultiplier();
            _bonuses->Clear();
            return;
        }

        _ball->Update();
    }
    else if (_gameState == GameState::IDLE)
    {
        _ball->FollowPlayer(*_player);
    }
}

void Game::ProcessEvents()
{
    _input.Process();

    // handle quit event
    if (_input.State == InputHandler::State::QUIT || _input.KeyMap[InputHandler::KEY_ESCAPE])
    {
        _appState = AppState::QUITING;
        return;
    }

    // handle input
    if (_gameState == GameState::IDLE && _input.KeyMap[InputHandler::KEY_SPACE])
    {
        _gameState = GameState::PLAY;
        _ball->Start();
    }

    if (_gameState != GameState::STOP && _input.KeyMap[InputHandler::KEY_LEFT_ARROW] && !_input.KeyMap[InputHandler::KEY_RIGHT_ARROW])
        _player->Move(true);

    if (_gameState != GameState::STOP && _input.KeyMap[InputHandler::KEY_RIGHT_ARROW] && !_input.KeyMap[InputHandler::KEY_LEFT_ARROW])
        _player->Move(false);
}

void Game::EndGame(bool win)
{
    _gameState = GameState::STOP;
    _drawContext.push_back(win ? _winScreen : _loseScreen);

    try
    {
        std::cout << "Your score is " << _counter.GetScore() << " with " << _lives->GetHealth() << " lives left." << std::endl;
        std::cout << "=======================================================" << std::endl;
        std::cout << "Table of highscores for the current map:" << std::endl;
        std::cout << "=======================================================" << std::endl;

        _scorer.AppendHighscore({ (int32_t)_counter.GetScore(), _lives->GetHealth(), _maper.GetMapPath(), _playerName });
        _scorer.Load(_maper.GetMapPath());
        _scorer.PrintHighscore(std::cout);
    }
    catch (const HighscoreLoaderException& e) { std::cout << e.Message() << std::endl; }
}
