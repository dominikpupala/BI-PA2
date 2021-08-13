#pragma once

#include <fstream>
#include <vector>

/**
 * @brief Class used for wrapping exception context from the HighscoreLoader class.
*/
class HighscoreLoaderException : public std::exception
{
private:
    std::string _message;

public:
    /**
     * @brief Create a new instance of the object.
     * @param message Programmer given context.
    */
    inline HighscoreLoaderException(const std::string& message) : _message(message) { }
    /**
     * @brief Message getter.
     * @return Exception context.
    */
    inline std::string Message() const { return _message; }
    /**
     * @brief Exception specifier.
     * @return Exception specifier.
    */
    inline const char* what() const noexcept override { return "HighscoreLoaderException"; }
};

/**
 * @brief Structure used for storing highscore data.
*/
struct Highscore
{
    int32_t Score;
    int32_t Lives;
    std::string Map;
    std::string Player;
};

/**
 * @brief Class used for loading and writing highscores.
*/
class HighscoreLoader
{
private:
    std::string _filePath;
    std::vector<Highscore> _scores;

public:
    /**
     * @brief Create a new instance of the object.
     * @param fileName Score file path.
    */
    HighscoreLoader(const std::string& fileName);
    /**
     * @brief Load the data.
     * @param map Map file path to filter highscores.
    */
    void Load(const std::string& map);
    /**
     * @brief Print highscores in descending order.
     * @param os Output stream.
    */
    void PrintHighscore(std::ostream& os) const;
    /**
     * @brief Append the score.
     * @param score New score to append.
    */
    void AppendHighscore(const Highscore& score);
};

