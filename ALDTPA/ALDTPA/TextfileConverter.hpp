#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "character.hpp"

/*
wood 1
tile 2
white brick 3
brick 4

SDL_Renderer *renderer, int count = 1, const std::string &fname = "image", const std::string &exten = ".bmp",
                int newX = 0, int newY = 0,bool newSolid=false, bool newMoveable =false)
*/

inline void TextToObjects(std::vector<Block *> &objectList, SDL_Renderer *renderer, const std::string &fileName = "../Assets/default_file.txt")
{
    std::ifstream myFile(fileName);
    if (!myFile.is_open())
    {
        std::cerr << "[WARN]: Could not open level file: " << fileName << std::endl;
        return;
    }

    std::string line;
    int currY = 0;

    while (std::getline(myFile, line))
    {
        int currX = 0;

        for (char index : line)
        {
            std::string currImg = "chinchilla";
            bool collide = false;

            if (index == '1')
            {
                currImg = "wood";
                collide = true;
            }
            else if (index == '2')
            {
                currImg = "tile";
            }
            else if (index == '3')
            {
                currImg = "white_brick";
            }
            else if (index == '4')
            {
                currImg = "brick";
                collide = true;
            }

            objectList.push_back(new Block(renderer, 1, currImg, ".bmp", currX, currY, collide));
            currX += 32;
        }

        currY += 32;
    }

    myFile.close();
}
