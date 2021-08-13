#include "MapLoader.h"

#include <string>

Map::Map(const std::vector<std::vector<int32_t>>& layout)
    : Layout(layout) { }

MapLoader::MapLoader(const std::string& fileName, int32_t rows, int32_t columns)
    : _rows(rows), _columns(columns), _mapPath(fileName), _ifstream(fileName, std::ios::in) { }

Map MapLoader::Load()
{
    if (_rows < 1 || _columns < 1)
        throw MapLoaderException("Can't load a map with zero rows/columns parameters");

    if (_ifstream.fail())
        throw MapLoaderException("Failed to create input file stream!");

    std::vector<std::vector<int32_t>> layout(_rows, std::vector<int32_t>(_columns, 0));

    std::string temp;
    for (int32_t i = 0; i < _rows; ++i)
    {
        if (!(_ifstream >> temp))
            throw MapLoaderException("Invalid number of rows!");

        if ((int32_t)temp.length() != _columns)
            throw MapLoaderException("Invalid number of columns inside of a row!");

        for (size_t j = 0; j < temp.length(); ++j)
            layout[i][j] = ParseFromChar(temp[j]);
    }

    if (!_ifstream.eof())
        throw MapLoaderException("Invalid number of rows!");

    if (!IsValid(layout))
        throw MapLoaderException("The map is unfinishable!");

    return Map(layout);
}

std::string MapLoader::GetMapPath() const
{
    return _mapPath;
}

int32_t MapLoader::ParseFromChar(char ch)
{
    if (isdigit(ch)) return ch - '0';
    if (ch == CHAR_WALL) return -1;
    throw MapLoaderException("Invalid character!");
}

bool MapLoader::IsValid(const std::vector<std::vector<int32_t>>& layout)
{
    // TODO: in case of any changes to the game configuration - check if this is still valid
    /*
    We could check if there are any bricks behind a full row of unbreakable bricks, but since the ball
    diameter is 12 units and the smallest offset between walls and bricks is 18 units we don't have to.
    */

    uint32_t count = 0;
    std::vector<std::vector<int32_t>> grid(layout.size() + 2, std::vector<int32_t>(layout[0].size() + 2, 0));

    for (size_t i = 0; i < layout.size(); ++i) for (size_t j = 0; j < layout[0].size(); ++j)
    {
        grid[1 + i][1 + j] = layout[i][j];
        if (layout[i][j] > 0) ++count;
    }

    // zero breakable bricks
    if (count == 0)
        return false;

    std::pair<size_t, size_t> temp;
    std::vector<std::pair<size_t, size_t>> stack;
    stack.push_back({ 0, 0 });

    while (!stack.empty())
    {
        temp = stack.back();
        stack.pop_back();

        // if visited or wall
        if (grid[temp.first][temp.second] < 0)
            continue;

        // mark as visited
        grid[temp.first][temp.second] = -2;

        if (temp.second < grid[0].size() - 1)
            stack.push_back({ temp.first, temp.second + 1 });
        if (temp.first < grid.size() - 1)
            stack.push_back({ temp.first + 1, temp.second });
        if (temp.second > 0)
            stack.push_back({ temp.first, temp.second - 1 });
        if (temp.first > 0)
            stack.push_back({ temp.first - 1, temp.second });
    }

    // check for unreachable connected components
    for (size_t i = 0; i < grid.size(); i++) for (size_t j = 0; j < grid[0].size(); j++)
    {
        if (grid[i][j] > 0)
            return false;
    }

    return true;
}
