#include "raylib.h"

#ifndef PROPS_H
#define PROPS_H

typedef struct PropDef
{
    int isActive;
    int textureId;
    char *label;
    Texture2D *tex;
    float rotation;
    int gridCoords[2];
    int eventId;
    int deckId;
    int isEventLive;
    float pulseRate;
    float currentAlpha;
} PropDef;

void InitProps(void);
void AddProp(PropDef prop);
PropDef *GetProp(int gridX, int gridY);

#endif // PROPS_H