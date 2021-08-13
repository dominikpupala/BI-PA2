#include "GameObjects.h"

#include <cstdlib>
#include <algorithm>

GameObject::GameObject(const std::shared_ptr<Texture>& texture, int32_t x, int32_t y, int32_t width, int32_t height)
    : _x(x), _y(y), _width(width), _height(height), _texture(texture) { }

GameObject::~GameObject() { }

void GameObject::Draw(const RenderManager& renderer) const
{
    renderer.Draw(_texture->GetTexture(), { _x, _y, _width, _height });
}

std::shared_ptr<IDrawable> GameObject::Clone() const
{
    return std::make_shared<GameObject>(*this);
}

int32_t GameObject::GetX() const
{
    return _x;
}

int32_t GameObject::GetY() const
{
    return _y;
}

int32_t GameObject::GetWidth() const
{
    return _width;
}

int32_t GameObject::GetHeight() const
{
    return _height;
}

Player::Player(const std::shared_ptr<Texture>& platform, int32_t x, int32_t y, int32_t width, int32_t height, int32_t maxSize, int32_t speed)
    : GameObject(platform, x, y, width, height), _speed(speed), _maxSize(maxSize) { }

std::shared_ptr<IDrawable> Player::Clone() const
{
    return std::make_shared<Player>(*this);
}

void Player::Move(bool direction)
{
    _x += (direction) ? -_speed : _speed;
}

void Player::IncreaseSize()
{
    _width += (_maxSize - _width) / 2;
}

void Player::IncreaseSpeed()
{
    _speed += 2;
}

void Player::CollisionBoundary(int32_t x, int32_t width)
{
    if (_x <= x) _x = x + 1;
    else if (_x >= width - _width) _x = width - _width - 1;
}

Brick::Brick(const std::shared_ptr<Texture>& brick, int32_t x, int32_t y, int32_t width, int32_t height, int32_t health)
    : GameObject(brick, x, y, width, height), _health(health) { }

std::shared_ptr<IDrawable> Brick::Clone() const
{
    return std::make_shared<Brick>(*this);
}

void Brick::DecreaseHealth(const std::shared_ptr<Texture>& brick)
{
    --_health;
    _texture = brick;
}

int32_t Brick::GetHealth() const
{
    return _health;
}

Ball::Ball(const std::shared_ptr<Texture>& ball, int32_t x, int32_t y, int32_t width, int32_t height, int32_t speed)
    : GameObject(ball, x, y, width, height), _speed(speed), _xDirection(0), _yDirection(0) { }

std::shared_ptr<IDrawable> Ball::Clone() const
{
    return std::make_shared<Ball>(*this);
}

void Ball::Start()
{
    srand(time(0));
    int dir = rand() % 2;

    _xDirection = dir ? 1 : -1;
    _yDirection = -1;
}

void Ball::Update()
{
    _x = NewPositionX();
    _y = NewPositionY();
}

void Ball::FollowPlayer(const Player& player)
{
    _x = -_width / 2 + player.GetX() + player.GetWidth() / 2;
    _y = -_height + player.GetY();
}

void Ball::IncreaseSpeed()
{
    _speed += 1;
}

bool Ball::IsUnder(int32_t height)
{
    return _y >= height;
}

void Ball::CollisionBoundary(int32_t x, int32_t y, int32_t width)
{
    if (_x <= x) _xDirection = 1;
    else if (_x >= width - _width) _xDirection = -1;

    if (_y <= y) _yDirection = 1;
}

bool Ball::CollisionCheck(const std::shared_ptr<GameObject>& object)
{
    // lets call this double buffered AABB -> AABBAABB or AAAABBBB?
    int32_t futurePosX = NewPositionX();
    int32_t futurePosY = NewPositionY();

    if ((futurePosX + _width < object->GetX() || futurePosX > object->GetX() + object->GetWidth()) ||
        (futurePosY + _height < object->GetY() || futurePosY > object->GetY() + object->GetHeight()))
        return false;

    bool collisionX = _x + _width >= object->GetX() && _x <= object->GetX() + object->GetWidth();
    bool collisionY = _y + _height >= object->GetY() && _y <= object->GetY() + object->GetHeight();

    if (collisionX && collisionY)
        return false;

    _xDirection = collisionX ? _xDirection : -_xDirection;
    _yDirection = collisionY ? _yDirection : -_yDirection;

    return true;
}

int32_t Ball::NewPositionX() const
{
    return _x + _xDirection * _speed;
}

int32_t Ball::NewPositionY() const
{
    return _y + _yDirection * _speed;
}

Bonus::Bonus(const std::shared_ptr<Texture>& bonus, int32_t x, int32_t y, int32_t width, int32_t height, Type type)
    : GameObject(bonus, x, y, width, height), _type(type) { }

std::shared_ptr<IDrawable> Bonus::Clone() const
{
    return std::make_shared<Bonus>(*this);
}

void Bonus::Update()
{
    _y += SPEED;
}

Bonus::Type Bonus::GetType() const
{
    return _type;
}

BonusManager::BonusManager(const std::vector<std::shared_ptr<Texture>>& textures, int32_t width, int32_t height, int32_t propability)
    : _width(width), _height(height), _propability(propability), _textures(textures)
{
    if (_propability < 0 || _propability > 100)
        _propability = PROPABILITY_DEFAULT;
}

void BonusManager::Draw(const RenderManager& renderer) const
{
    for (size_t i = 0; i < _bonuses.size(); ++i)
        _bonuses[i]->Draw(renderer);
}

std::shared_ptr<IDrawable> BonusManager::Clone() const
{
    return std::make_shared<BonusManager>(*this);
}

void BonusManager::Clear()
{
    _bonuses.clear();
}

void BonusManager::Generate(int32_t x, int32_t y)
{
    int temp = rand() % 100;

    if (temp >= _propability)
        return;

    temp = rand() % (int32_t)Bonus::Type::TYPE_COUNT;
    _bonuses.push_back(std::make_shared<Bonus>(_textures[temp], x - _width / 2, y - _height / 2, _width, _height, (Bonus::Type)temp));
}

void BonusManager::CollisionPlayer(Player& player, Ball& ball, ScoreCounter& score)
{
    for (auto& bonus : _bonuses)
    {
        if (!bonus)
            continue;

        bonus->Update();

        // clear unreachable bonuses
        if (bonus->GetY() > OUT_OF_BOUNDS_CONSTANT)
        {
            _bonuses.erase(std::find(_bonuses.begin(), _bonuses.end(), bonus));
            continue;
        }

        // collision 
        if (bonus->GetX() + bonus->GetWidth() < player.GetX() || bonus->GetX() > player.GetX() + player.GetWidth() ||
            bonus->GetY() + bonus->GetHeight() < player.GetY() || bonus->GetY() > player.GetY() + player.GetHeight())
            continue;

        switch (bonus->GetType())
        {
        case Bonus::Type::BIGGER_PLATFORM:
            score.AddBonusScore(5);
            player.IncreaseSize();
            break;
        case Bonus::Type::FASTER_PLATFORM:
            score.AddBonusScore(5);
            player.IncreaseSpeed();
            break;
        case Bonus::Type::FASTER_BALL:
            score.AddBonusScore(15);
            ball.IncreaseSpeed();
            score.IncreaseSpeedMultiplier();
            break;
        case Bonus::Type::SCORE_100:
            score.AddBonusScore(100);
            break;
        case Bonus::Type::SCORE_200:
            score.AddBonusScore(200);
            break;
        case Bonus::Type::SCORE_300:
            score.AddBonusScore(300);
            break;
        default:
            break;
        }

        _bonuses.erase(std::find(_bonuses.begin(), _bonuses.end(), bonus));
    }
}

BrickManager::BrickManager(const std::vector<std::shared_ptr<Texture>>& destroyable, const std::shared_ptr<Texture>& undestroyable, const Map& map, int32_t x, int32_t y, int32_t width, int32_t height)
    : _undestroyableTexture(undestroyable), _destroyableTextures(destroyable)
{
    for (size_t i = 0; i < map.Layout.size(); ++i) for (size_t j = 0; j < map.Layout[0].size(); ++j)
    {
        if (map.Layout[i][j] == 0)
            continue;

        _bricks.push_back(std::make_shared<Brick>(map.Layout[i][j] == -1 ? _undestroyableTexture : _destroyableTextures[map.Layout[i][j] - 1], x + j * width, y + i * height, width, height, map.Layout[i][j] - 1));
    }
}

void BrickManager::Draw(const RenderManager& renderer) const
{
    for (size_t i = 0; i < _bricks.size(); ++i)
        _bricks[i]->Draw(renderer);
}

std::shared_ptr<IDrawable> BrickManager::Clone() const
{
    return std::make_shared<BrickManager>(*this);
}

bool BrickManager::IsFinished() const
{
    return std::find_if(_bricks.begin(), _bricks.end(), [](const std::shared_ptr<Brick>& brick)-> bool { return brick->GetHealth() >= 0; }) == _bricks.end();
}

void BrickManager::CollisionBall(Ball& ball, BonusManager& bonuses, ScoreCounter& scorer)
{
    for (auto& brick : _bricks)
    {
        if (!brick || !ball.CollisionCheck(brick) || brick->GetHealth() < 0)
            continue;

        if (brick->GetHealth() > 0)
        {
            brick->DecreaseHealth(_destroyableTextures[brick->GetHealth() - 1]);
        }
        else
        {
            scorer.AddScore();
            bonuses.Generate(brick->GetX() + brick->GetWidth() / 2, brick->GetY() + brick->GetHeight() / 2);

            _bricks.erase(std::find(_bricks.begin(), _bricks.end(), brick));
        }
    }
}

Background::Background(const std::vector<std::shared_ptr<IDrawable>>& layers)
    : _layers(layers) { }

void Background::Draw(const RenderManager& renderer) const
{
    for (size_t i = 0; i < _layers.size(); ++i)
        _layers[i]->Draw(renderer);
}

std::shared_ptr<IDrawable> Background::Clone() const
{
    return std::make_shared<Background>(*this);
}

Health::Health(const std::shared_ptr<Texture>& ball, const std::shared_ptr<Texture>& label, int32_t lives, int32_t x, int32_t y, int32_t labelWidth, int32_t labelHeight, int32_t ballWidth, int32_t ballHeight)
    : _x(x), _y(y), _lives(lives), _ballWidth(ballWidth), _ballHeight(ballHeight), _labelWidth(labelWidth), _labelHeight(labelHeight), _ball(ball), _label(label) { }

void Health::Draw(const RenderManager& renderer) const
{
    renderer.Draw(_label->GetTexture(), { _x, _y, _labelWidth, _labelHeight });

    for (int32_t i = 0; i < _lives; ++i)
        renderer.Draw(_ball->GetTexture(), { _x + _labelWidth + (int32_t)i * (_ballWidth + SPACING), _y + _labelHeight / 2 - _ballHeight / 2, _ballWidth, _ballHeight });
}

std::shared_ptr<IDrawable> Health::Clone() const
{
    return std::make_shared<Health>(*this);
}

void Health::DecreaseHealth()
{
    --_lives;
}

int32_t Health::GetHealth() const
{
    return _lives;
}
