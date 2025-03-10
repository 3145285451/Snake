#ifndef RESOURCE_H
#define RESOURCE_H

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

class ResourceManager
{
public:
    Texture titleTexture;
    Texture startTexture;
    Texture winTexture;
    Texture loseTexture;
    Texture replayTexture;
    Texture exitTexture;

    bool loadResources()
    {
        if (!titleTexture.loadFromFile("../assets/title.jpg"))
        {
            cerr << "Error loading assets/title.jpg" << endl;
            return false;
        }
        if (!startTexture.loadFromFile("../assets/start.jpg"))
        {
            cerr << "Error loading assets/start.jpg" << endl;
            return false;
        }
        if (!winTexture.loadFromFile("../assets/win.jpg"))
        {
            cerr << "Error loading assets/win.jpg" << endl;
            return false;
        }
        if (!loseTexture.loadFromFile("../assets/lose.jpg"))
        {
            cerr << "Error loading assets/lose.jpg" << endl;
            return false;
        }
        if (!replayTexture.loadFromFile("../assets/replay.jpg"))
        {
            cerr << "Error loading assets/replay.jpg" << endl;
            return false;
        }
        if (!exitTexture.loadFromFile("../assets/exit.jpg"))
        {
            cerr << "Error loading assets/exit.jpg" << endl;
            return false;
        }
        return true;
    }
};

#endif