#pragma once

#include <fstream>
#include <vector>

/**
 * @brief Class used for wrapping exception context from the MapLoader class.
*/
class MapLoaderException : public std::exception
{
private:
    std::string _message;

public:
    /**
     * @brief Create a new instance of the object.
     * @param message Programmer given context.
    */
    inline MapLoaderException(const std::string& message) : _message(message) { }
    /**
     * @brief Message getter.
     * @return Exception context.
    */
    inline std::string Message() const { return _message; }
    /**
     * @brief Exception specifier.
     * @return Exception specifier.
    */
    inline const char* what() const noexcept override { return "MapLoaderException"; }
};

/**
 * @brief Class used for storing map layout.
*/
class Map
{
public:
    std::vector<std::vector<int32_t>> Layout;

    /**
     * @brief Create a new instance of the object.
     * @param layout Data in form of 2D array.
    */
    Map(const std::vector<std::vector<int32_t>>& layout);
};

/**
 * @brief Class used for loading maps.
*/
class MapLoader
{
private:
    static const char CHAR_WALL = '#';

    int32_t _rows;
    int32_t _columns;
    std::string _mapPath;
    std::ifstream _ifstream;

public:
    /**
     * @brief Create a new instance of the object.
     * @param fileName Map file path.
     * @param rows Number of rows.
     * @param columns Number of columns.
    */
    MapLoader(const std::string& fileName, int32_t rows, int32_t columns);
    /**
     * @brief Load the data.
     * @return Map object with the loaded data.
    */
    Map Load();
    /**
     * @brief Map file path getter.
     * @return Value of the map file path.
    */
    std::string GetMapPath() const;

private:
    /**
     * @brief Parse character from the data.
     * @param ch Character to be parsed.
     * @return Numerical value.
    */
    static int32_t ParseFromChar(char ch);
    /**
     * @brief Validates the map. Uses DFS.
     * @param layout Data in form of 2D array.
     * @return True if data are valid map.
    */
    static bool IsValid(const std::vector<std::vector<int32_t>>& layout);
};
