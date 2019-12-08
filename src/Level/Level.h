#ifndef SFML_GAME_LEVEL_H
#define SFML_GAME_LEVEL_H
#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct Object
{
    int GetPropertyInt(string name);
    float GetPropertyFloat(string name);
    string GetPropertyString(string name);
    string name;
    string type;
    Rect<int> rect;
    map<string, string> properties;
    Sprite sprite;
};

struct Layer
{
    int opacity;
    vector<Sprite> tiles;
};

class Level
{
  public:
    bool LoadFromFile(string filename);
    Object GetObject(string name);
    vector<Object> GetObjects(string name);
    void Draw(RenderWindow &window);
    int GetWindowWidth();
    Vector2i GetTileSize();
    int columns;
    int rows;
    int width, height, tileWidth, tileHeight;

  private:
    int firstTileID;
    Rect<float> drawingBounds;
    Texture tilesetImage;
    vector<Object> objects;
    vector<Layer> layers;
};
#endif
