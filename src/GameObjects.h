#pragma once

#include <memory>
#include <vector>

#include "MapLoader.h"
#include "ScoreCounter.h"
#include "RenderManager.h"
#include "TextureLoader.h"

/**
 * @brief Interface used for objects drawable using the RenderManager class.
*/
class IDrawable
{
public:
    virtual ~IDrawable() { };
    virtual void Draw(const RenderManager& renderer) const = 0;
    virtual std::shared_ptr<IDrawable> Clone() const = 0;
};

/**
 * @brief Base class used for wrapping basic context of entity used in a game.
*/
class GameObject : public IDrawable
{
protected:
    int32_t _x;
    int32_t _y;
    int32_t _width;
    int32_t _height;
    std::shared_ptr<Texture> _texture;

public:
    /**
     * @brief Create a new instance of the object.
     * @param texture Object texture.
     * @param x Object position on horizontal axis.
     * @param y Object position on vertical axis.
     * @param width Object width.
     * @param height Object height.
    */
    GameObject(const std::shared_ptr<Texture>& texture, int32_t x, int32_t y, int32_t width, int32_t height);
    /**
     * @brief Virtual destructor. This class is meant to be inherited.
    */
    virtual ~GameObject();
    /**
     * @brief Draw the object.
     * @param renderer Target renderer.
    */
    virtual void Draw(const RenderManager& renderer) const override;
    /**
     * @brief Clone the object.
     * @return Smart pointer to the object.
    */
    virtual std::shared_ptr<IDrawable> Clone() const override;

    /**
     * @brief The object position on horizontal axis getter.
     * @return Value of the position.
    */
    int32_t GetX() const;
    /**
     * @brief The object position on vertical axis getter.
     * @return Value of the position.
    */
    int32_t GetY() const;
    /**
     * @brief The object width.
     * @return Value of the width.
    */
    int32_t GetWidth() const;
    /**
     * @brief The object height.
     * @return Value of the height.
    */
    int32_t GetHeight() const;
};

/**
 * @brief Class used for player controlled object - the platform.
*/
class Player : public GameObject
{
private:
    int32_t _speed;
    int32_t _maxSize;

public:
    /**
     * @brief Create a new instance of the object.
     * @param platform Object texture.
     * @param x Object position on horizontal axis.
     * @param y Object position on vertical axis.
     * @param width Object width.
     * @param height Object height.
     * @param maxSize Object max size.
     * @param speed Object speed.
    */
    Player(const std::shared_ptr<Texture>& platform, int32_t x, int32_t y, int32_t width, int32_t height, int32_t maxSize, int32_t speed);
    /**
     * @brief Clone the object.
     * @return Smart pointer to the object.
    */
    virtual std::shared_ptr<IDrawable> Clone() const override;

    /**
     * @brief Move the object.
     * @param direction Direction flag - positive is for left.
    */
    void Move(bool direction);
    /**
     * @brief Increase size of the object. Size is increased by half of the difference of the max size and the current size.
    */
    void IncreaseSize();
    /**
     * @brief Increase speed of the object. It is additive increase.
    */
    void IncreaseSpeed();
    /**
     * @brief Check collision of the object with the playable boundary.
     * @param x Boundary position on horizontal axis.
     * @param width Boundary width.
    */
    void CollisionBoundary(int32_t x, int32_t width);
};

/**
 * @brief Class used for brick object.
*/
class Brick : public GameObject
{
private:
    int32_t _health;

public:
    /**
     * @brief Create a new instance of the object.
     * @param brick Object texture.
     * @param x Object position on horizontal axis.
     * @param y Object position on vertical axis.
     * @param width Object width.
     * @param height Object height.
     * @param health Object health.
    */
    Brick(const std::shared_ptr<Texture>& brick, int32_t x, int32_t y, int32_t width, int32_t height, int32_t health);
    /**
     * @brief Clone the object.
     * @return Smart pointer to the object.
    */
    virtual std::shared_ptr<IDrawable> Clone() const override;

    /**
     * @brief Decrease health of the object.
     * @param brick Texture representating change in health of the object.
    */
    void DecreaseHealth(const std::shared_ptr<Texture>& brick);
    /**
     * @brief Health getter.
     * @return Value of the health.
    */
    int32_t GetHealth() const;
};

/**
 * @brief Class used for ball object.
*/
class Ball : public GameObject
{
private:
    int32_t _speed;
    int32_t _xDirection; // unit vector
    int32_t _yDirection; // unit vector

public:
    /**
     * @brief Create a new instance of the object.
     * @param ball Object texture.
     * @param x Object position on horizontal axis.
     * @param y Object position on vertical axis.
     * @param width Object width.
     * @param height Object height.
     * @param speed Object speed.
    */
    Ball(const std::shared_ptr<Texture>& ball, int32_t x, int32_t y, int32_t width, int32_t height, int32_t speed);
    /**
     * @brief Clone the object.
     * @return Smart pointer to the object.
    */
    virtual std::shared_ptr<IDrawable> Clone() const override;

    /**
     * @brief Start the object movement.
    */
    void Start();
    /**
     * @brief Update the object movement.
    */
    void Update();
    /**
     * @brief Follow player with the object movement.
     * @param player Player object to follow.
    */
    void FollowPlayer(const Player& player);
    /**
     * @brief Increase speed of the object. It is additive increase.
    */
    void IncreaseSpeed();
    /**
     * @brief Check collision of the object with the fail boundary.
     * @param height Boundary height.
     * @return True if inside the fail boundary.
    */
    bool IsUnder(int32_t height);
    /**
     * @brief Check collision of the object with other GameObject. Double buffered axis aligned bounding box algorithm.
     * @param object Object to check collision with.
     * @return True if objects collided.
    */
    bool CollisionCheck(const std::shared_ptr<GameObject>& object);
    /**
     * @brief Check collision of the object with the playable boundary.
     * @param x Boundary position on horizontal axis.
     * @param y Boundary position on vertical axis.
     * @param width Boundary width.
    */
    void CollisionBoundary(int32_t x, int32_t y, int32_t width);

private:
    /**
     * @brief Calculate the next assumed position on horizontal axis.
     * @return Value of the assummed position.
    */
    int32_t NewPositionX() const;
    /**
     * @brief Calculate the next assumed position on vertical axis.
     * @return Value of the assummed position.
    */
    int32_t NewPositionY() const;
};

/**
 * @brief Class used for bonus object.
*/
class Bonus : public GameObject
{
public:
    /**
     * @brief Enumclass for bonus type.
    */
    enum class Type
    {
        BIGGER_PLATFORM,
        FASTER_PLATFORM,
        FASTER_BALL,
        SCORE_100,
        SCORE_200,
        SCORE_300,
        TYPE_COUNT
    };

private:
    static const int32_t SPEED = 3;

    Type _type;

public:
    /**
     * @brief Create a new instance of the object.
     * @param bonus Object texture.
     * @param x Object position on horizontal axis.
     * @param y Object position on vertical axis.
     * @param width Object width.
     * @param height Object height.
     * @param type Object type.
    */
    Bonus(const std::shared_ptr<Texture>& bonus, int32_t x, int32_t y, int32_t width, int32_t height, Type type);
    /**
     * @brief Clone the object.
     * @return Smart pointer to the object.
    */
    virtual std::shared_ptr<IDrawable> Clone() const override;

    /**
     * @brief Update the object movement.
    */
    void Update();
    /**
     * @brief Type getter.
     * @return Value of the type.
    */
    Type GetType() const;
};

/**
 * @brief Class used for managing Bonus class objects.
*/
class BonusManager : public IDrawable
{
private:
    static const int32_t PROPABILITY_DEFAULT = 5;
    static const int32_t OUT_OF_BOUNDS_CONSTANT = 1000;

    int32_t _width;
    int32_t _height;
    int32_t _propability;
    std::vector<std::shared_ptr<Bonus>> _bonuses;
    std::vector<std::shared_ptr<Texture>> _textures;

public:
    /**
     * @brief Create a new instance of the object manager.
     * @param textures Object manager texture atlas for the Bonus class objects.
     * @param width Bonus object width.
     * @param height Bonus object height.
     * @param propability Bonus propability chance.
    */
    BonusManager(const std::vector<std::shared_ptr<Texture>>& textures, int32_t width, int32_t height, int32_t propability);
    /**
      * @brief Draw the object manager.
      * @param renderer Target renderer.
     */
    virtual void Draw(const RenderManager& renderer) const override;
    /**
     * @brief Clone the object manager.
     * @return Smart pointer to the object manager.
    */
    virtual std::shared_ptr<IDrawable> Clone() const override;

    /**
     * @brief Clear all managed bonuses.
    */
    void Clear();
    /**
     * @brief Generate a new bonus.
     * @param x Postion on horizontal axis to generate bonus.
     * @param y Postion on vertical axis to generate bonus.
    */
    void Generate(int32_t x, int32_t y);
    /**
     * @brief Check collision of the object manager with Player object.
     * @param player Player object to check collision with & apply bonus effects.
     * @param ball Ball object to apply bonus effects.
     * @param score ScoreCounter object to aply bonus effects.
    */
    void CollisionPlayer(Player& player, Ball& ball, ScoreCounter& score);
};

/**
 * @brief Class used for managing Brick class objects.
*/
class BrickManager : public IDrawable
{
private:
    std::shared_ptr<Texture> _undestroyableTexture;
    std::vector<std::shared_ptr<Texture>> _destroyableTextures;

    std::vector<std::shared_ptr<Brick>> _bricks;

public:
    /**
     * @brief Create a new instance of the object manager.
     * @param destroyable Object manager texture atlas for the Brick class objects that are destroyable.
     * @param undestroyable Object manager texture for the Brick class objects that are not destroyable.
     * @param map Object manager Map object for Brick objects layout.
     * @param x Object manager boundary position on horizontal axis.
     * @param y Object manager boundary position on vertical axis.
     * @param width Object manager boundary width.
     * @param height Object manager boundary height.
    */
    BrickManager(const std::vector<std::shared_ptr<Texture>>& destroyable, const std::shared_ptr<Texture>& undestroyable, const Map& map, int32_t x, int32_t y, int32_t width, int32_t height);
    /**
      * @brief Draw the object manager.
      * @param renderer Target renderer.
     */
    virtual void Draw(const RenderManager& renderer) const override;
    /**
     * @brief Clone the object manager.
     * @return Smart pointer to the object manager.
    */
    virtual std::shared_ptr<IDrawable> Clone() const override;

    /**
     * @brief Check if all destroyable Brick objects are destroyed.
     * @return True if all destroyable Brick objects are destroyed.
    */
    bool IsFinished() const;
    /**
     * @brief Check collision of the object manager with Ball object.
     * @param ball Ball object to check collision with.
     * @param bonuses BonusManager object to potential bonus generation.
     * @param scorer ScoreCounter object to aggregate score.
    */
    void CollisionBall(Ball& ball, BonusManager& bonuses, ScoreCounter& scorer);
};

/**
 * @brief Class used for screen background.
*/
class Background : public IDrawable
{
private:
    std::vector<std::shared_ptr<IDrawable>> _layers;

public:
    /**
     * @brief Create a new instance of the object.
     * @param layers Objects to be drawn as a background.
    */
    Background(const std::vector<std::shared_ptr<IDrawable>>& layers);
    /**
      * @brief Draw the object.
      * @param renderer Target renderer.
     */
    virtual void Draw(const RenderManager& renderer) const override;
    /**
     * @brief Clone the object.
     * @return Smart pointer to the object.
    */
    virtual std::shared_ptr<IDrawable> Clone() const override;
};

/**
 * @brief Class used for health ui.
*/
class Health : public IDrawable
{
private:
    static const int32_t SPACING = 5;

    int32_t _x;
    int32_t _y;
    int32_t _lives;
    int32_t _ballWidth;
    int32_t _ballHeight;
    int32_t _labelWidth;
    int32_t _labelHeight;
    std::shared_ptr<Texture> _ball;
    std::shared_ptr<Texture> _label;

public:
    /**
     * @brief Create a new instance of the object.
     * @param ball Object texture representing single life.
     * @param label Object texture representing label.
     * @param lives Number lives.
     * @param x Object position on horizontal axis.
     * @param y Object position on vertical axis.
     * @param labelWidth Width of the label texture.
     * @param labelHeight Height of the label texture.
     * @param ballWidth Width of the ball texture.
     * @param ballHeight Height of the ball texture.
    */
    Health(const std::shared_ptr<Texture>& ball, const std::shared_ptr<Texture>& label, int32_t lives, int32_t x, int32_t y, int32_t labelWidth, int32_t labelHeight, int32_t ballWidth, int32_t ballHeight);
    /**
      * @brief Draw the object.
      * @param renderer Target renderer.
     */
    virtual void Draw(const RenderManager& renderer) const override;
    /**
     * @brief Clone the object.
     * @return Smart pointer to the object.
    */
    virtual std::shared_ptr<IDrawable> Clone() const override;

    /**
     * @brief Decrease health of the object.
    */
    void DecreaseHealth();
    /**
     * @brief Health getter.
     * @return Value of the health.
    */
    int32_t GetHealth() const;
};
