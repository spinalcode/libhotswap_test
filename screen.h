// tile map
#define BG_TILE_SIZE_H 8
#define BG_TILE_SIZE_W 8
#define tbt BG_TILE_SIZE_H*BG_TILE_SIZE_W
#define htbt BG_TILE_SIZE_H*(BG_TILE_SIZE_W/2)

/*
     ______                 __                 _______                              
    |   __ \.-----.-----.--|  |.-----.----.   |     __|.----.----.-----.-----.-----.
    |      <|  -__|     |  _  ||  -__|   _|   |__     ||  __|   _|  -__|  -__|     |
    |___|__||_____|__|__|_____||_____|__|     |_______||____|__| |_____|_____|__|__|

*/

void loadPalette(const uint16_t* p, int numColours) {
    for (int i=0;i<numColours;i++)
        Pokitto::Display::palette[i] = p[i];
    Pokitto::Display::paletteptr = Pokitto::Display::palette;
}

void clearPalette(int numColours) {
    for (int i=0;i<numColours;i++)
        Pokitto::Display::palette[i] = 0;
    Pokitto::Display::paletteptr = Pokitto::Display::palette;
}

uint16_t alphaBlendRGB565(uint32_t fg, uint32_t bg, uint8_t alpha) {

    fg = (fg | fg << 16) & 0x07e0f81f;
    bg = (bg | bg << 16) & 0x07e0f81f;
    bg += (fg - bg) * alpha >> 5;
    bg &= 0x07e0f81f;
        return (uint16_t)(bg | bg >> 16);
}


void spriteFill(std::uint8_t* line, std::uint32_t y, bool skip){

    if(spriteLine[y]==0) return;

    #define width 16

    auto scanLine = &Pokitto::Display::palette[16]; // start 32 pixels in because of underflow
    #define width 16
    #define halfWidth 8

    int y2 = y;
    
    spriteLine[y]=0;
    if(spriteCount>=0){
        for(int32_t spriteIndex = 1; spriteIndex<=spriteCount; spriteIndex++){
            auto &sprite = sprites[spriteIndex];
            if(sprite.bit==1){
                if((int)y >= sprite.y && (int)y < sprite.y + sprite.imageData[0]){
                    if(sprite.x>-width && sprite.x<PROJ_LCDWIDTH){
                        if(sprite.hFlip){
                            uint32_t so = 2+(2 * (y2-sprite.y));
                            auto si = &sprite.imageData[so+3];
                            auto sl = &scanLine[sprite.x];
                            auto st = &scanLine[sprite.x + sprite.imageData[0]];
                            auto palette = sprite.paletteData;
                            auto pixel = *si;
                            #define midLoop()\
                                if(((pixel=*si)>>7)&3) *sl = palette[(pixel>>7)&1];\
                                sl++;\
                                if((pixel>>6)&1) *sl = palette[(pixel>>6)&1];\
                                sl++;\
                                if((pixel>>5)&1) *sl = palette[(pixel>>5)&1];\
                                sl++;\
                                if((pixel>>4)&1) *sl = palette[(pixel>>4)&1];\
                                sl++;\
                                if((pixel>>3)&1) *sl = palette[(pixel>>3)&1];\
                                sl++;\
                                if((pixel>>2)&1) *sl = palette[(pixel>>2)&1];\
                                sl++;\
                                if((pixel>>1)&1) *sl = palette[(pixel>>1)&1];\
                                sl++;\
                                if(pixel&1) *sl = palette[pixel&1];\
                                si--; sl++;
                            while(sl < st)
                                midLoop();
                        }else{
                            uint32_t so = 2+(2 * (y2-sprite.y));
                            auto si = &sprite.imageData[so];
                            auto sl = &scanLine[sprite.x];
                            int sw = sprite.imageData[0];
                            auto palette = sprite.paletteData;
                            auto pixel = *si;
                            #define midLoop()\
                                if(((pixel=*si)>>7)&3) *sl = palette[(pixel>>7)&1];\
                                sl++;\
                                if((pixel>>6)&1) *sl = palette[(pixel>>6)&1];\
                                sl++;\
                                if((pixel>>5)&1) *sl = palette[(pixel>>5)&1];\
                                sl++;\
                                if((pixel>>4)&1) *sl = palette[(pixel>>4)&1];\
                                sl++;\
                                if((pixel>>3)&1) *sl = palette[(pixel>>3)&1];\
                                sl++;\
                                if((pixel>>2)&1) *sl = palette[(pixel>>2)&1];\
                                sl++;\
                                if((pixel>>1)&1) *sl = palette[(pixel>>1)&1];\
                                sl++;\
                                if(pixel&1) *sl = palette[pixel&1];\
                                si++; sl++;
                            midLoop();
                        }
                    } // if X
                } // if Y
            } // 1bpp
            if(sprite.bit==2){
                if((int)y >= sprite.y && (int)y < sprite.y + sprite.imageData[0]){
                    if(sprite.x>-width && sprite.x<PROJ_LCDWIDTH){
                        if(sprite.hFlip){
                            uint32_t so = 2+(2 * (y2-sprite.y));
                            auto si = &sprite.imageData[so+3];
                            auto sl = &scanLine[sprite.x];
                            auto palette = sprite.paletteData;
                            auto pixel = *si;
                            #define midLoop()\
                                if(((pixel=*si)>>6)&3) *sl = palette[(pixel>>6)&3];\
                                sl++;\
                                if((pixel>>4)&3) *sl = palette[(pixel>>4)&3];\
                                sl++;\
                                if((pixel>>2)&3) *sl = palette[(pixel>>2)&3];\
                                sl++;\
                                if((pixel)&3) *sl = palette[(pixel)&3];\
                                *si--; sl++;
                            midLoop(); midLoop();
                        }else{
                            uint32_t so = 2+(2 * (y2-sprite.y));
                            auto si = &sprite.imageData[so];
                            auto sl = &scanLine[sprite.x];
                            auto palette = sprite.paletteData;
                            auto pixel = *si;
                            #define midLoop()\
                                if(((pixel=*si)>>6)&3) *sl = palette[(pixel>>6)&3];\
                                sl++;\
                                if((pixel>>4)&3) *sl = palette[(pixel>>4)&3];\
                                sl++;\
                                if((pixel>>2)&3) *sl = palette[(pixel>>2)&3];\
                                sl++;\
                                if((pixel)&3) *sl = palette[(pixel)&3];\
                                *si++; sl++;
                            midLoop(); midLoop();
                        }
                    } // if X
                } // if Y
            } // 2bpp
            if(sprite.bit==4){
                if((int)y >= sprite.y && (int)y < sprite.y + sprite.imageData[0]){
                    if(sprite.x>-width && sprite.x<PROJ_LCDWIDTH){
                        if(sprite.hFlip){
                            uint32_t so = 2+(halfWidth * (y2-sprite.y));
                            auto si = &sprite.imageData[so+7];
                            auto sl = &scanLine[sprite.x];
                            auto palette = sprite.paletteData;
                            #define midLoop()\
                                if(auto pixel = *si&15) *sl = palette[pixel];\
                                sl++;\
                                if(auto pixel = *si>>4) *sl = palette[pixel];\
                                si--; sl++;
                            midLoop(); midLoop(); midLoop(); midLoop();
                            midLoop(); midLoop(); midLoop(); midLoop(); 
                        }else{
                            uint32_t so = 2+(halfWidth * (y2-sprite.y));
                            auto si = &sprite.imageData[so];
                            auto sl = &scanLine[sprite.x];
                            auto palette = sprite.paletteData;
                            #define midLoop()\
                                if(auto pixel = *si>>4) *sl = palette[pixel];\
                                sl++;\
                                if(auto pixel = *si&15) *sl = palette[pixel];\
                                si++; sl++;
                            midLoop(); midLoop(); midLoop(); midLoop();
                            midLoop(); midLoop(); midLoop(); midLoop(); 
                        }
                    } // if X
                } // if Y
            } // 4bpp
            if(sprite.bit==8){
                if((int)y >= sprite.y && (int)y < sprite.y + sprite.imageData[0]){
                    if(sprite.x>-width && sprite.x<PROJ_LCDWIDTH){
                        if(sprite.hFlip){
                            uint32_t so = 2+(width * (y2-sprite.y));
                            auto si = &sprite.imageData[so+15];
                            auto sl = &scanLine[sprite.x];
                            auto palette = sprite.paletteData;
                            #define midLoop()\
                                if(auto pixel = *si) *sl = palette[pixel];\
                                si--; sl++;
                            midLoop(); midLoop(); midLoop(); midLoop();
                            midLoop(); midLoop(); midLoop(); midLoop(); 
                            midLoop(); midLoop(); midLoop(); midLoop();
                            midLoop(); midLoop(); midLoop(); midLoop(); 
                        }else{
                            uint32_t so = 2+(width * (y2-sprite.y));
                            auto si = &sprite.imageData[so];
                            auto sl = &scanLine[sprite.x];
                            auto palette = sprite.paletteData;
                            #define midLoop()\
                                if(auto pixel = *si) *sl = palette[pixel];\
                                si++; sl++;
                            midLoop(); midLoop(); midLoop(); midLoop();
                            midLoop(); midLoop(); midLoop(); midLoop(); 
                            midLoop(); midLoop(); midLoop(); midLoop();
                            midLoop(); midLoop(); midLoop(); midLoop(); 
                        }
                    } // if X
                } // if Y
            } // 8bpp
        } // for spriteCount
    } // sprite count >1
    return;
}

void myBGFiller(std::uint8_t* line, std::uint32_t y, bool skip){

    if(y==0){
        for(uint32_t x=0; x<220; ++x){
            line[x]=x+16;
        }        
    }

    #define TILEWIDTH 8
    #define TILEHEIGHT 8

    //gamePalette[0] = 0xF81F; //gamePalette.hpal[hline[y]];

    int32_t x = -(bg.windowX%TILEWIDTH)+16;
    uint32_t tileIndex = (bg.windowX/TILEWIDTH) + ((y+bg.windowY)/TILEWIDTH) * bg.miniMap[0];
    uint32_t jStart = ((y+bg.windowY)%TILEWIDTH) *TILEWIDTH; // current line in current tile

    uint32_t lineOffset;
    uint32_t thisTile;
    auto lineP = &Pokitto::Display::palette[16];

    #define bgTileLine()\
        thisTile = bg.miniMap[2+tileIndex++]&0x83FF;\
        lineOffset = ((thisTile&1023)*tbt) + jStart;\
        lineP = &Pokitto::Display::palette[x];\
        x+=8;\
        if(thisTile&32768){\
            auto tilesP = &tiles[7 + lineOffset];\
            *lineP++ = gamePalette[*tilesP--];\
            *lineP++ = gamePalette[*tilesP--];\
            *lineP++ = gamePalette[*tilesP--];\
            *lineP++ = gamePalette[*tilesP--];\
            *lineP++ = gamePalette[*tilesP--];\
            *lineP++ = gamePalette[*tilesP--];\
            *lineP++ = gamePalette[*tilesP--];\
            *lineP++ = gamePalette[*tilesP--];\
        }else{\
            auto tilesP = &tiles[lineOffset];\
            *lineP++ = gamePalette[*tilesP++];\
            *lineP++ = gamePalette[*tilesP++];\
            *lineP++ = gamePalette[*tilesP++];\
            *lineP++ = gamePalette[*tilesP++];\
            *lineP++ = gamePalette[*tilesP++];\
            *lineP++ = gamePalette[*tilesP++];\
            *lineP++ = gamePalette[*tilesP++];\
            *lineP++ = gamePalette[*tilesP++];\
        }

    // unrolling this loop got an extra 10fps
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine();
}
