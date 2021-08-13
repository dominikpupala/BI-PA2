#include "HighscoreLoader.h"

#include <string>
#include <sstream>
#include <algorithm>

HighscoreLoader::HighscoreLoader(const std::string& fileName)
    : _filePath(fileName) { }

void HighscoreLoader::Load(const std::string& mapFilter)
{
    std::ifstream ifs(_filePath, std::ios::in);

    if (ifs.fail())
        throw HighscoreLoaderException("Failed to initialize output file stream!");

    int32_t score;
    int32_t lives;
    std::string map;
    std::string player;

    std::string buffer;
    while (std::getline(ifs, buffer) && !buffer.empty())
    {
        std::istringstream iss(buffer, std::ios::in);

        if (!(iss >> map >> score >> lives >> player) || iss.peek() != -1)
            throw HighscoreLoaderException("Invalid number of arguments for entry!");

        if (map != mapFilter)
            continue;

        _scores.push_back({ score, lives, map, player });
    }

    std::sort(_scores.begin(), _scores.end(), [](Highscore l, Highscore r) -> bool { return l.Score == r.Score ? l.Lives > r.Lives : l.Score > r.Score; });
}

void HighscoreLoader::PrintHighscore(std::ostream& os) const
{
    for (size_t i = 0; i < _scores.size(); ++i)
        os << _scores[i].Score << "\t" << _scores[i].Lives << "\t" << _scores[i].Player << std::endl;
}

void HighscoreLoader::AppendHighscore(const Highscore& score)
{
    std::ofstream ofs(_filePath, std::ios::out | std::ios::app);

    if (ofs.fail())
        throw HighscoreLoaderException("Failed to initialize input file stream!");

    if (!(ofs << score.Map << "\t" << score.Score << "\t" << score.Lives << "\t" << score.Player << std::endl))
        throw HighscoreLoaderException("Failed to append the highscore!");
}
