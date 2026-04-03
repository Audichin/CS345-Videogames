#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include "character.hpp"
/*
wood 1
tile 2
white brick 3
brick 4

SDL_Renderer *renderer, int count = 1, const std::string &fname = "image", const std::string &exten = ".bmp",
                int newX = 0, int newY = 0,bool newSolid=false, bool newMoveable =false)
*/

void TextToObjects(vector<*Block> objectList,SDL_Renderer *renderer, string fileName="Assets/default_file.txt"){
    ifstream myFile(fileName);
    string line;
    string currImg = "tile";
    bool collide=false;
    int currX=0;
    int currY=0;
    cout<<"================================"<<endl;
    while(getline(myFile,line)){
        for index in line{
            if (index == 1) {currImg = "Assets/wood"; collide = true;}
            else if (index == 2) {currImg = "Assets/tile"; collide = false;}
            else if (index == 3) {currImg = "Assets/white_brick"; collide = false;}
            else if (index == 4) {currImg = "Assets/brick"; collide = true;}
            else {currImg = "chinchilla"; collide=false;}
            currX+=32;
            cout<<currImg<<endl;

            objectList.push_back(new Block(render,1,currImg,"bmp",currX,currY,collide));
        }
        currX=0;
        currY+=32;
    }


    myFile.close()
}