#include <Pokitto.h>
#include <LibHotswap>
#include <File>
#include "globals.h"
#include "font.h"
#include "screen.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PB = Pokitto::Buttons;

void drawSprite(int x, int y, const uint8_t *imageData,const uint16_t *paletteData, bool hFlip, uint8_t bit){
    // if out of screen bounds, don't bother
    if(x<-16 || x>220) return;
    if(y<-imageData[1] || y>175) return;
    if(++spriteCount>NUMSPRITES-1)spriteCount=NUMSPRITES-1; // don't overflow the sprite max
    sprites[spriteCount].x = x;
    sprites[spriteCount].y = y;
    sprites[spriteCount].imageData = imageData;
    sprites[spriteCount].paletteData = paletteData;
    sprites[spriteCount].hFlip = hFlip;
    sprites[spriteCount].bit = bit;
    // make sure we know there are sprites on these lines
    int y2 = y+sprites[spriteCount].imageData[1];
    for(int t=y; t<y2; t++){
        if(t>=0 && t<176)
            spriteLine[t]=1;
    }
}

// print text
void myPrint(char x, char y, const char* text) {
  uint8_t numChars = strlen(text);
  uint8_t x1 = 0;//2+x+28*y;
  for (uint8_t t = 0; t < numChars; t++) {
    uint8_t character = text[t] - 32;
    drawSprite(x+((x1++)*8), y, font88[character], font88_pal,0,2);
  }
}

void loadMapChunk(int mx, int my, const char* fileName){
    auto minMap = &bg.miniMap[0];
    uint32_t t=0;
    // The map in RAM is 2x2 physical screens
    bg.miniMap[t++] = 28*2;
    bg.miniMap[t++] = 22*2;
    // load a section of the map from the level file into an array
    File file;
    if(file.openRO(fileName)){
        uint16_t mWidth;
        file.read(&mWidth, 2);
        for(int y=0; y<bg.miniMap[1]; y++){ // bg.miniMap[1] = 1024 here?
            file.seek(4+(mx*2)+(mWidth*2)*(my+y));
            file.read(&bg.miniMap[t], bg.miniMap[0]*2);
            t+=bg.miniMap[0];
        }
        file.close();
    }
}

int main() {


    PC::begin();

    // load the data into hotswap
    int paletteSize = 512;
    levelData = LevelData::load("map_data/01data.bin"); 
    for(int t=0; t<256; t++){
        gamePalette[t] = (levelData[1+t*2]<<8)|levelData[t*2];
    }
    
    tiles = &levelData[paletteSize + 2];
    uint16_t tilesOffset = *reinterpret_cast<const uint16_t*>(&levelData[paletteSize]);
    collisionTile = &levelData[paletteSize + 2 + tilesOffset];

    // Load some map data
    loadMapChunk(0,30,"map_data/01.bin");
    
    // assign the linefiller
    Pokitto::Display::lineFillers[0] = myBGFiller; // map layer
    Pokitto::Display::lineFillers[1] = spriteFill; // map layer

    
    
    while (1) {
        if(PC::update()) continue;

        spriteCount=0;
    
        char tempText[32];
        sprintf(tempText,"FPS:%d",fpsCount);
        myPrint(0,168,tempText);
    
        fpsCounter++;
        if(PC::getTime() >= lastMillis+1000){
            lastMillis = PC::getTime();
            fpsCount = fpsCounter;
            fpsCounter = 0;
        }
    }    
    
    
    
    return 1;
}

