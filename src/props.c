#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "props.h"
#include "screens.h"

PropDef props[20][20] = {0};

void InitProps(void)
{
    for (int x = 0; x < 20; x++)
    {
        for (int y= 0; y < 20; y++)
        {
            props[x][y].isActive = false;
        }
    }
}

void AddProp(PropDef prop)
{
    props[prop.gridCoords[0]][prop.gridCoords[1]].isActive = prop.isActive;
    props[prop.gridCoords[0]][prop.gridCoords[1]].gridCoords[0] = prop.gridCoords[0];
    props[prop.gridCoords[0]][prop.gridCoords[1]].gridCoords[1] = prop.gridCoords[1];
    props[prop.gridCoords[0]][prop.gridCoords[1]].rotation = prop.rotation;
    props[prop.gridCoords[0]][prop.gridCoords[1]].tex = prop.tex;
    props[prop.gridCoords[0]][prop.gridCoords[1]].textureId = prop.textureId;
    props[prop.gridCoords[0]][prop.gridCoords[1]].eventId = prop.eventId;
    props[prop.gridCoords[0]][prop.gridCoords[1]].label = prop.label;
    if (props[prop.gridCoords[0]][prop.gridCoords[1]].eventId > 0)
    {
        props[prop.gridCoords[0]][prop.gridCoords[1]].isEventLive = true;
    }
}

PropDef *GetProp(int gridX, int gridY)
{
    if (props[gridX][gridY].isActive) return &props[gridX][gridY];
    return NULL;
}