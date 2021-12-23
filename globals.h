
// fps counter
int fpsCount=0;
long int fpsCounter;
long int lastMillis;


// pointers to our data
const uint8_t* levelData = nullptr;
const uint8_t* tiles = nullptr;
const uint8_t* collisionTile = nullptr;
// Make a hotswap area for level data
// size = 64*1024k, unique id = 0
using LevelData = Hotswap<50*1024, 0>; // multiple of 8kb = Palette and level tiles

uint16_t gamePalette[256];

uint8_t spriteLine[176]; // does this horizontal line have sprites on it?
uint8_t spriteCount; // are there any sprites to render during this frame?

// information needed to render a sprite
#define NUMSPRITES 16
struct SPRITE_DATA {
    const uint16_t *paletteData; // palette data
    const uint8_t *imageData; // image data
    int x;  // x postition
    int y;  // y position
    bool hFlip; // is sprite h-flipped?
    int offset; // tile render pixel offset
    uint8_t bit; // how many bpp is the image data?
} sprites[NUMSPRITES];

struct BACKGROUND_DATA {
    uint16_t miniMap[2+56*44]; // the window on the whole map
    int windowX; // position within the map window
    int windowY; // position within the map window
    int mapX; // scroll amount of main map
    int mapY; // scroll amount of main map
    int mapWidth; // width of map
    int mapHeight; // height of map
    int screenX = 0;
    int screenY = 0;

    int multiplyX;
    int multiplyY;
} bg;
