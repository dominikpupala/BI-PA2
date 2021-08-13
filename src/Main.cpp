#include "Game.h"

#include <cassert>
#include <iostream>

bool test_map(std::string fileName)
{
    MapLoader mapTester(fileName, 10, 8);

    try
    {
        mapTester.Load();
    }
    catch (const MapLoaderException& e)
    {
        return false;
    }

    return true;
}

int main(int argc, char* argv[])
{
    static const std::string DEFAULT_PLAYER_NAME = "Anonymous";
    static const std::string DEFAULT_MAP_FILE_PATH = "examples/maps/Map4.txt";
    static const std::string DEFAULT_SCORE_FILE_PATH = "examples/Score.txt";

    if (argc > 3)
    {
        std::cout << "Invalid number of arguments!" << std::endl;
        return 1;
    }

    std::string map = argc > 1 ? argv[1] : DEFAULT_MAP_FILE_PATH;
    std::string player = argc > 2 ? argv[2] : DEFAULT_PLAYER_NAME;

    // test maploader
    assert(test_map("examples/maps/Map1.txt"));
    assert(test_map("examples/maps/Map2.txt"));
    assert(test_map("examples/maps/Map3.txt"));
    assert(test_map("examples/maps/Map4.txt"));
    assert(test_map("examples/maps/EasyMap0.txt"));
    assert(test_map("examples/maps/EasyMap1.txt"));
    assert(test_map("examples/maps/HardMap0.txt"));
    assert(test_map("examples/maps/HardMap1.txt"));
    assert(!test_map("examples/maps/BadMap0.txt"));
    assert(!test_map("examples/maps/BadMap1.txt"));
    assert(!test_map("examples/maps/BadMap2.txt"));

    // run game
    Game game(map, DEFAULT_SCORE_FILE_PATH, player);
    game.Init();
    game.Play();

    return 0;
}
