#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "screens.h"
#include "scene.h"
#include "dialog.h"
#include "props.h"
#include "battle.h"
#include "game_flags.h"

/*

    This file is a tragedy. I started here and piled stuff in quickly and later on figured out how to do things better but haven't had time to go back and fix it.

*/

int GlobalScore = 0;

static const float playerVel = 150.0f;
const int mapWidth = 20;
const int mapHeight = 20;

#define BIG_STAR_COUNT 100

Vector3 bigStars[BIG_STAR_COUNT];

// if you use mapWidth and mapHeight here there's errors, just remember to update mapWidth and mapHeight
int map[10][20][20] = {
    // null
    {0},
    {
        // engineer
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //
    },                                                               //
    {
        // docks
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //
    },                                                               //
    {
        // living quarters
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //
    },
    {
        // taccom
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //
    },
    {
        //stratcom
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //
    },
    {
        // officer's deck
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //
    },
    {
        // med bay
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //
    },
    {
        // intel
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //
    },
    {
        // bridge
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0},
        {0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //
    }};

// DEBUG
void DrawDebug(void);

// FADING ROUTINES
void FadeIn(void);
void FadeOut(void);
void FadeOutAndIn(void);
void FadeOutAndChangeDeck(int newDeck);
void FadeOutAndChangeScene(int newScene);

// WALKING UI
void UpdateWalkingUI(float delta);
void DrawWalkingUI(void);

// WALKING MAP
void UpdateWalkingMap(float delta);
void DrawWalkingMap(void);
void UpdateBigStars(float delta);
void DrawBigStars(void);

// event
void RunEvent(int eventId);
void AfterFadeOut();
void AfterFadeIn();
void AfterLeaveDoor();

char *modeText;

int CurrentScene = 0;
int CurrentDeck = 3;
float CurrentDeckFlashTimeout = 0.25f;
int CurrentDeckHighlighted = 1;

Vector2 PlayerPos = {0};
int PlayerDirection = FACING_UP;
Rectangle PlayerColBox = {0};
int isFadingOut = 0;
int isFadingIn = 0;
float curtainFadeLevel = 0.0f;

// TRANSITION/FADE FLAGS
int fadeInDialogWaiting = 0;
int fadeOutDialogWaiting = 0;
int fadeBackInAutomatically = 0;
DialogTextEntry fadeInDialog = {0};
DialogTextEntry fadeOutDialog = {0};
int fadeOutDeckChange = -1;
int fadeOutSceneChange = -1;
int isWhiteout = 0;
float whiteoutLevel = 0.0f;

int allowedDecks[9] = {0};

char *pageText = "";
int pageSend = 0;

Color darkerGray = {
    50, 50, 50, 255};
Color transparentBlack = {
    0, 0, 0, 64};
Color transparentWhite = {
    255, 255, 255, 64};

char *npcNames[100] = {0};

Color deckColors[] = {
    BLACK,
    GRAY,
    ORANGE,
    GREEN,
    YELLOW,
    RED,
    PURPLE,
    BLUE,
    LIGHTGRAY,
    WHITE};

char *deckNames[] = {
    "NULL",
    "ENGR",
    "DOCK",
    "LiVE",
    "TCOM",
    "SCOM",
    "OFCR",
    "MED+",
    "iNTL",
    "BRDG"};

Color tileColors[8] = {
    BLACK,
    DARKGRAY,
    LIGHTGRAY,
    RED,
    RED,
    RED,
    RED,
    RED};

PropDef deckProps[][DECK_PROP_MAX] = {
    // null
    {},
    // engineering
    {
        {
            //
            IS_ACTIVE,
            TEX_RAYLIB,
            NO_LABEL,
            NULL,
            FACING_UP,
            {8, 8},
            EVENT_NULL,
            0,     // is even live
            100.0f //

        }, // pulse rate},
        {IS_ACTIVE,
         TEX_ENGINE_CORE,
         NO_LABEL,
         NULL,
         FACING_UP,
         {7, 7},
         EVENT_NULL}},
    // dock
    {
        {IS_ACTIVE,
         TEX_BAY_DOORS,
         NO_LABEL,
         NULL,
         FACING_UP,
         {6, 1},
         EVENT_NULL,
         1.0f},
        {IS_ACTIVE,
         TEX_SHUTTLE,
         NO_LABEL,
         NULL,
         FACING_UP,
         {7, 6},
         EVENT_NULL}},
    // living quarters
    {},
    // taccom
    {},
    // stratcom
    {},
    // officers quarters
    {},
    // med bay
    {},
    // intell
    {},
    // bridge
    {},
    // TOP 9 DECKS
    {
        {
            IS_ACTIVE, //
            TEX_DOOR,  //
            "DOWN",
            NULL,               //
            FACING_UP,          //
            {10, 16},           //
            EVENT_ELEVATOR_DOWN //
        }                       //
    },
    // BOTTOM 9 DECKS
    {
        {
            IS_ACTIVE,
            TEX_DOOR,
            "UP",
            NULL,
            FACING_UP,
            {8, 16},
            EVENT_ELEVATOR_UP, //
        }                      //
    }                          // the end
};

Scene scene[] = {
    {
        "Scene 01 - Intro", // name
        HAS_CUTSCENE,        // has cutscene
        {
            // cutscene entries
            {
                "Use WASD for directions and ENTER\nto interact.\n\nOr use the on screen controls with your\nmouse or touch screen",
                TEX_NULL,
                NULL,
                {0},
                NPC_NARRATOR,
                PORTRAIT_NOT_ON_RIGHT
            }
        },
        MODE_WALKING, // WALKING
        DECK_DOCK,    // starting deck
        {
            {
                IS_ACTIVE,
                TEX_BED,
                "BUNK 7",
                NULL,
                FACING_UP,
                {4, 8},
                EVENT_SCENE01_BED,
                DECK_LiVE // Put on the living quarters deck
            },            //
            {
                IS_ACTIVE,
                TEX_DOOR,
                "TAC-COM",
                NULL,
                FACING_DOWN,
                {9, 3},
                EVENT_SCENE01_TACCOM_DOOR,
                DECK_TCOM // put it here for testing, move it back to TCOM
            },            //
            {
                IS_ACTIVE,
                TEX_PLAYER,
                "BUDDY",
                NULL,
                FACING_DOWN,
                {8, 8},
                EVENT_SCENE01_BUDDY,
                DECK_LiVE //
            },
            {
                IS_ACTIVE,
                TEX_PLAYER,
                "RUIZ",
                NULL,
                FACING_RIGHT,
                {12, 4},
                EVENT_SCENE01_RUIZ,
                DECK_LiVE //
            },
            {IS_ACTIVE,
             TEX_PLAYER,
             "SMITH",
             NULL,
             FACING_RIGHT,
             {5, 6},
             EVENT_SCENE01_SMITH,
             DECK_DOCK}, //
            {
                IS_ACTIVE,
                TEX_PLAYER,
                "LEE",
                NULL,
                FACING_RIGHT,
                {4, 4},
                EVENT_SCENE01_LEE,
                DECK_ENGR}} // prop array
    },                      //
    {
        "Scene 02 - Taccom Battle Practice 01",
        NO_CUTSCENE,
        {},
        MODE_BATTLE,
        -1, // deck
        {}  // prop array
    },      //
    {
        "Scene 03 - Walking after Practice 1",
        NO_CUTSCENE,
        {}, // cutscene dialog
        MODE_WALKING,
        DECK_TCOM,
        {
            {
                IS_ACTIVE,
                TEX_DOOR,
                "TAC-COM",
                NULL,
                FACING_DOWN,
                {9, 3},
                EVENT_SCENE03_TACCOM_DOOR,
                DECK_TCOM // put it here for testing, move it back to TCOM
            },
            {IS_ACTIVE,
             TEX_PLAYER,
             "BUDDY",
             NULL,
             FACING_LEFT,
             {10, 5},
             EVENT_SCENE03_BUDDY,
             DECK_TCOM},
            {IS_ACTIVE,
             TEX_PLAYER,
             "LEE",
             NULL,
             FACING_RIGHT,
             {4, 4},
             EVENT_SCENE03_LEE,
             DECK_ENGR} //
        }               // props
    },                  //
    {
        "Scene 04 - Taccom Practice 2",
        NO_CUTSCENE,
        {},
        MODE_BATTLE,
        -1,
        {} //
    },
    {
        //
        "Scene 05 - Walking after practice 2",
        HAS_CUTSCENE,
        {
            //
            {
                //
                "I don't care what you think you saw.\nYou saw nothing.",
                TEX_NULL,
                NULL,
                {0},
                NPC_INTEL_TARI,
                PORTRAIT_ON_RIGHT //
            },
            {
                //
                "Aye aye...\nsir.",
                TEX_NULL,
                NULL,
                {0},
                NPC_COMM_SMITH,
                PORTRAIT_ON_RIGHT //
            },
            {
                //
                "What was that all about sir?",
                TEX_NULL,
                NULL,
                {0},
                NPC_SELF,
                PORTRAIT_NOT_ON_RIGHT //
            },
            {
                //
                "That was Intelligence Officer TARI.\nA mean S.O.B. and he just informed me\nthat we did not witness that simulation.\nI don't like this.\nIf you have anyone you can trust on board\nask them to keep an eye out.",
                TEX_NULL,
                NULL,
                {0},
                NPC_COMM_SMITH,
                PORTRAIT_ON_RIGHT //
            },
            {
                //
                "Understood sir.",
                TEX_NULL,
                NULL,
                {0},
                NPC_SELF,
                PORTRAIT_NOT_ON_RIGHT //
            }                         //
        },                            //
        MODE_WALKING,
        DECK_TCOM,
        {
            // props
            {
                IS_ACTIVE,
                TEX_PLAYER,
                "BUDDY",
                NULL,
                FACING_LEFT,
                {10, 5},
                EVENT_SCENE05_BUDDY,
                DECK_DOCK //
            },            //
            {
                IS_ACTIVE,
                TEX_BED,
                "BUNK 7",
                NULL,
                FACING_UP,
                {4, 8},
                EVENT_SCENE01_BED,
                DECK_LiVE // Put on the living quarters deck
            },            //
            {
                IS_ACTIVE,
                TEX_DOOR,
                "TAC-COM",
                NULL,
                FACING_DOWN,
                {9, 3},
                EVENT_SCENE05_TACCOM_DOOR,
                DECK_TCOM // put it here for testing, move it back to TCOM
            },
            {
                //
                IS_ACTIVE,
                TEX_PLAYER,
                "LEE",
                NULL,
                FACING_RIGHT,
                {4, 4},
                EVENT_SCENE05_LEE,
                DECK_ENGR //
            },            //
            {
                //
                IS_ACTIVE,
                TEX_PLAYER,
                "SMITH",
                NULL,
                FACING_LEFT,
                {10, 4},
                EVENT_SCENE05_SMITH,
                DECK_TCOM //
            }             //
                          //
        }                 //
    },                    //
    {                     //
     "Scene 06 - Taccom Mission 1",
     NO_CUTSCENE,
     {},
     MODE_BATTLE,
     -1,
     {}},
    {
        //
        "Scene 07 - after the attack",
        HAS_CUTSCENE,
        {{
             //
             "THAT WAS CLOSE! The alien projectile that\ndestroyed Europe passed right by us.\nWe were nearly destroyed!",
             TEX_NULL,
             NULL,
             {0},
             NPC_INTEL_TARI,
             PORTRAIT_ON_RIGHT //
         },
         {
             "And all because we were called to Europe\nto deal with that skirmish. Such\nincredible timing.",
             TEX_NULL,
             NULL,
             {0},
             NPC_INTEL_TARI,
             PORTRAIT_ON_RIGHT //
         },                    //
         {
             "You handled that very well. You will be the\nlead gunner when we ATTACK whoever\nor WHATEVER did this.",
             TEX_NULL,
             NULL,
             {0},
             NPC_INTEL_TARI,
             PORTRAIT_ON_RIGHT},
         {"(I get the feeling I'm looking\nat WHATEVER did this...)",
          TEX_NULL,
          NULL,
          {0},
          NPC_SELF,
          PORTRAIT_NOT_ON_RIGHT}},
        MODE_WALKING,
        DECK_TCOM,
        {{
             IS_ACTIVE,
             TEX_PLAYER,
             "BUDDY",
             NULL,
             FACING_LEFT,
             {10, 5},
             EVENT_SCENE07_BUDDY,
             DECK_DOCK //
         },            //
         {
             IS_ACTIVE,
             TEX_BED,
             "BUNK 7",
             NULL,
             FACING_UP,
             {4, 8},
             EVENT_SCENE07_BED,
             DECK_LiVE // Put on the living quarters deck
         },            //
         {
             IS_ACTIVE,
             TEX_DOOR,
             "TAC-COM",
             NULL,
             FACING_DOWN,
             {9, 3},
             EVENT_NULL,
             DECK_TCOM // put it here for testing, move it back to TCOM
         },
         {
             //
             IS_ACTIVE,
             TEX_PLAYER,
             "LEE",
             NULL,
             FACING_RIGHT,
             {4, 4},
             EVENT_SCENE07_LEE,
             DECK_ENGR //
         },            //
         {
             //
             IS_ACTIVE,
             TEX_PLAYER,
             "SMITH",
             NULL,
             FACING_LEFT,
             {10, 4},
             EVENT_SCENE07_SMITH,
             DECK_TCOM //
         }}            //
    },                 //
    {
        //
        "Scene 08 - after the jump",
        HAS_CUTSCENE,
        {{"God, that sucked. I feel like I've been\nasleep for years.",
          TEX_NULL,
          NULL,
          {0},
          NPC_SELF,
          0},
         {"I better find BUDDY and see how he faired.",
          TEX_NULL,
          NULL,
          {0},
          NPC_SELF,
          0}},
        MODE_WALKING,
        DECK_LiVE,
        {
            {IS_ACTIVE,
             TEX_BED,
             "BUNK 07",
             NULL,
             FACING_UP,
             {4, 8},
             EVENT_NULL,
             DECK_LiVE},
            {
                //
                IS_ACTIVE,
                TEX_PLAYER,
                "SMITH",
                NULL,
                FACING_LEFT,
                {6, 8},
                EVENT_SCENE08_SMITH,
                DECK_LiVE //
            },
            {
                IS_ACTIVE,
                TEX_DOOR,
                "TAC-COM",
                NULL,
                FACING_DOWN,
                {9, 3},
                EVENT_SCENE08_TCOM_DOOR,
                DECK_TCOM // put it here for testing, move it back to TCOM
            },
            {
                //
                IS_ACTIVE,
                TEX_PLAYER,
                "TARI",
                NULL,
                FACING_LEFT,
                {10, 4},
                EVENT_SCENE08_TARI,
                DECK_TCOM //
            },
            {
                //
                IS_ACTIVE,
                TEX_PLAYER,
                "LEE",
                NULL,
                FACING_RIGHT,
                {6, 3},
                EVENT_SCENE08_LEE,
                DECK_ENGR //
            }             //
        }                 //
    },                    //
    {
        //
        "Scene 09 - battle of ECHO 7",
        NO_CUTSCENE,
        {},
        MODE_BATTLE,
        -1,
        {} //
    },     //
    {
        "Scene 10 - after echo 7",
        HAS_CUTSCENE,
        {{"THAT WAS CLOSE! We nearly lost the\nbattle!",
          TEX_NULL,
          NULL,
          {0},
          NPC_INTEL_TARI,
          PORTRAIT_ON_RIGHT},
         {"What?!? There was no battle. That was a\nfarm!",
          TEX_NULL,
          NULL,
          {0},
          NPC_SELF,
          PORTRAIT_NOT_ON_RIGHT},
         {"Repeat that to anybody and you will be\nkilled. It's time to start playing\nalong. For the greater good.",
          TEX_NULL,
          NULL,
          {0},
          NPC_INTEL_TARI,
          PORTRAIT_ON_RIGHT}},
        MODE_WALKING,
        DECK_TCOM,
        {
            {IS_ACTIVE,
             TEX_BED,
             "BUNK 07",
             NULL,
             FACING_UP,
             {4, 8},
             EVENT_NULL,
             DECK_LiVE},
            {
                //
                IS_ACTIVE,
                TEX_PLAYER,
                "SMITH",
                NULL,
                FACING_LEFT,
                {6, 4},
                EVENT_SCENE10_SMITH,
                DECK_ENGR //
            },
            {
                IS_ACTIVE,
                TEX_DOOR,
                "TAC-COM",
                NULL,
                FACING_DOWN,
                {9, 3},
                EVENT_NULL,
                DECK_TCOM // put it here for testing, move it back to TCOM
            },
            {
                //
                IS_ACTIVE,
                TEX_PLAYER,
                "LEE",
                NULL,
                FACING_RIGHT,
                {6, 3},
                EVENT_NULL,
                DECK_ENGR //
            },
            {IS_ACTIVE,
             TEX_CONTROL_PANEL,
             "CONTROL PANEL",
             NULL,
             FACING_DOWN,
             {8, 3},
             EVENT_SCENE10_CONTROL_PANEL,
             DECK_ENGR} //
        }               //
    },                  //
    {
        "Scene 11 - aftermath",
        HAS_CUTSCENE,
        {{"meanwhile on Earth...",
          TEX_NULL,
          NULL,
          {0},
          NPC_NARRATOR,
          PORTRAIT_ON_RIGHT},
         {"THAT WAS CLOSE!\nIt appears the ship's reactor overloaded\nshortly AFTER they broadcast the\nanomaly data to us.",
          TEX_NULL,
          NULL,
          {0},
          NPC_PRES,
          PORTRAIT_NOT_ON_RIGHT},
         {"Captain LASSIC and Officer TARI will\nbe known as heroes...",
          TEX_NULL,
          NULL,
          {0},
          NPC_PRES,
          PORTRAIT_NOT_ON_RIGHT},
         {"Of the new world I will build...",
          TEX_NULL,
          NULL,
          {0},
          NPC_PRES,
          PORTRAIT_NOT_ON_RIGHT},
         {"Thank you forplaying Mutiny on the\nErnest King. The game ended up being about\n1/4 of what I set out to do.\nI dreamed big for this gamejam\nand didn't quite hit my goal.\nBut I had fun :-D",
          TEX_NULL,
          NULL,
          {0},
          NPC_NARRATOR,
          0}}}};

void RunEvent(int eventId)
{
    switch (eventId)
    {
    case EVENT_SCENE01_LEE:
    {
        if (getFlag(GF_TALKED_TO_LEE) == 0)
            GlobalScore += 10;
        AddDialogQuick("I don't believe we met. Welcome aboard. I'm\nChief Engineer LEE.", NPC_ENG_LEE, PORTRAIT_ON_RIGHT);
        AddDialogQuick("Thank you Mr. Lee. I've heard this is a hell\nof a ship. I can't wait to see how\nit performs.", NPC_SELF, PORTRAIT_NOT_ON_RIGHT);
        AddDialogQuick("The Ernset King is the best ship ever built.\nThese engines could run for years\nand barely degrade.", NPC_ENG_LEE, PORTRAIT_ON_RIGHT);
        AddDialogQuick("The engines are charging now to simulate a\nburst shot. I think that means they're\nready for you in the simulator.\nGo up 3 decks to TACCOM.", NPC_ENG_LEE, 1);
        setFlag(GF_TALKED_TO_LEE, 1);
        allowedDecks[DECK_TCOM] = 1;
    }
    break;
    case EVENT_SCENE01_BUDDY:
    {
        if (getFlag(GF_TALKED_TO_BUDDY) == 0)
            GlobalScore += 10;
        AddDialogQuick("BUDDY? I didn't know you were here.\nHow long has it been?", NPC_SELF, PORTRAIT_NOT_ON_RIGHT);
        AddDialogQuick("At least 3 years.\nGreat to see you! Glad we'll\nget to serve together!\n", NPC_BUDDY, PORTRAIT_ON_RIGHT);
        AddDialogQuick("If you're at the guns I'll feel better\nif I end up in surface combat.", NPC_BUDDY, PORTRAIT_ON_RIGHT);
        AddDialogQuick("I'll be in the sim every day.\nI better have a good shot!\nFor now I need some rest.\n", NPC_SELF, PORTRAIT_NOT_ON_RIGHT);
        AddDialogQuick("Of course! Rest up.\nWhen you get a chance go\ndown to engineering too.\nDrive Engineer LEE is a\ncool dude. You'll like him.", NPC_BUDDY, PORTRAIT_ON_RIGHT);
        setFlag(GF_TALKED_TO_BUDDY, 1);
        allowedDecks[DECK_ENGR] = 1;
    }
    break;
    case EVENT_SCENE01_RUIZ:
    {
        if (getFlag(GF_TALKED_TO_RUIZ) == 0)
            GlobalScore += 10;
        AddDialogQuick("Welcome aboard.\nI am Ensign RUIZ.\nRemember...the captain\nis here for the good of\nall of us. Same with\nIntelligence Officer Taris.\nThey know best.", NPC_SHEEP_RUIZ, PORTRAIT_ON_RIGHT);
        AddDialogQuick("(jeez, what's with that guy?)", NPC_SELF, PORTRAIT_NOT_ON_RIGHT);
        setFlag(GF_TALKED_TO_RUIZ, 1);
    }
    break;
    case EVENT_SCENE01_SMITH:
    {
        if (getFlag(GF_TALKED_TO_SMITH) == 0)
            GlobalScore += 10;
        //////////////////////////////////// ////////////////////////////////////
        AddDialogQuick("THAT WAS CLOSE. You were on the last\nshuttle before we depart. Welcome\naboard the S.N.S. Ernest King.", NPC_COMM_SMITH, 1);
        AddDialogQuick("I am your commanding officer.\nMy name is Commaner SMITH.", NPC_COMM_SMITH, 1);
        ////////////////////////////////////  //////////////////////////////////// ////////////////////////////////////
        AddDialogQuick("We'll begin training after you\nget some sleep. You have bunk 7. One\ndeck up. Use the LEFT elevator to go\nup and the righ elevator to go down.", NPC_COMM_SMITH, 1);
        AddDialogQuick("Thank you for greeting me personally.\nI look forward to getting on the\nsim. Bunk 7, one deck up. Got it.", NPC_SELF, 0);
        allowedDecks[DECK_LiVE] = 1;
        setFlag(GF_TALKED_TO_SMITH, 1);
    }
    break;
    case EVENT_SCENE01_BED:;
        {
            fadeOutDialog.text = "First night on the ship is always\n the weirdest...";
            fadeOutDialog.speaker = NPC_NARRATOR;

            fadeInDialog.text = "I wish I was well rested...";
            fadeInDialog.speaker = NPC_NARRATOR;

            fadeInDialogWaiting = 1;
            fadeOutDialogWaiting = 1;

            FadeOutAndIn();
        }
        break;
    case EVENT_SCENE01_TACCOM_DOOR:;
        {
            DialogTextEntry dt = {0};
            dt.speaker = NPC_NARRATOR;
            dt.text = "TACCOM SIM time!";
            AddDialog(dt);
            FadeOutAndChangeScene(SCENE_02_BATTLE_TACCOM_PRACTICE_1);
        }
        break;
    case EVENT_SCENE03_BUDDY:
    {
        AddDialogQuick("THAT WAS CLOSE! I heard you dropped\nit right in the pickle jar!", NPC_BUDDY, 1);
        AddDialogQuick("Uhh, something like that.", NPC_SELF, 0);
        AddDialogQuick("LEE in engineering wanted to ask\nyou something when you were done.", NPC_BUDDY, 1);
        AddDialogQuick("Ok, I'll go check in with him.", NPC_SELF, 0);
        if (getFlag(GF_TALKED_TO_BUDDY) == 0)
            GlobalScore += 10;
        setFlag(GF_TALKED_TO_BUDDY, 1);
        setFlag(GF_ELEVATOR_UP_ENABLED, 1);
        setFlag(GF_ELEVATOR_DOWN_ENABLED, 1);
    }
    break;
    case EVENT_SCENE03_LEE:
    {
        AddDialogQuick("Did they tell you anything about\nlive fire exercises? Because\nthe draw on the core is huge\nand nobody will tell me why.", NPC_ENG_LEE, 1);
        AddDialogQuick("Not that I know of, but if I see\nanything weird I'll let you know.", NPC_SELF, 0);
        AddDialogQuick("Thanks kid! The core is ready for\nyour next practice run I think.", NPC_ENG_LEE, 1);
        if (getFlag(GF_TALKED_TO_LEE) == 0)
            GlobalScore += 10;
        setFlag(GF_TALKED_TO_LEE, 1);
    }
    break;
    case EVENT_SCENE03_TACCOM_DOOR:
        if (getFlag(GF_TALKED_TO_LEE) == 1)
        {
            AddDialogQuick("Welcome back. Let's get started.", NPC_COMM_SMITH, 1);
            FadeOutAndChangeScene(SCENE_04_BATTLE_TACCOM_PRACTICE_2);
        }
        break;
    case EVENT_SCENE05_BUDDY:
    {
        if (getFlag(GF_TALKED_TO_BUDDY) == 0)
        {
            GlobalScore += 10;
            setFlag(GF_TALKED_TO_BUDDY, 1);
        }
        AddDialogQuick("Hey, how'd the sim go?", NPC_BUDDY, 1);
        AddDialogQuick("Something very weird is happening.\nI think we are close to some\nvery bad dealings right now.", NPC_SELF, 0);
        AddDialogQuick("Well, I know we can trust LEE.\nDo you think we can trust SMITH?", NPC_BUDDY, 1);
        AddDialogQuick("I think so. I just watch Officer\nTARI tear him a new one!", NPC_SELF, 0);
        AddDialogQuick("Ok, so we'll work together and keep\nan eye on this...", NPC_BUDDY, 1);
        AddDialogQuick("Yeah, I'll go check with LEE.", NPC_SELF, 0);
    }
    break;
    case EVENT_SCENE05_LEE:
    {
        if (getFlag(GF_TALKED_TO_LEE) == 0)
        {
            GlobalScore += 10;
            setFlag(GF_TALKED_TO_LEE, 1);
        }
        AddDialogQuick("Mr. LEE, have you heard?", NPC_SELF, 0);
        AddDialogQuick("If you mean did I hear that STRATCOM\nsimulated nukin Europe, then YES.\nI heard.", NPC_ENG_LEE, 1);
        AddDialogQuick("I'm going to work with Buddy and\nkeep reporting to SMITH about what I see.", NPC_SELF, 0);
        AddDialogQuick("Good. I've served with SMITH.\nHe's a good man. If I notice anything\nI'll find you.", NPC_ENG_LEE, 1);
        pageText = "This is SMITH, get to TACCOM\nimmediately.";
        pageSend = NPC_COMM_SMITH;

        setFlagCooldown(GF_PAGE_SELF, 10.0f);
    }
    break;
    case EVENT_SCENE05_TACCOM_DOOR:
    {
        if (getFlag(GF_TALKED_TO_SMITH) == 1)
        {
            FadeOutAndChangeScene(SCENE_06_BATTLE_TACCOM_MISSION_1);
        }
    }
    break;
    case EVENT_SCENE05_SMITH:
    {
        if (getFlag(GF_TALKED_TO_SMITH) == 1)
        {
            AddDialogQuick("Get into TACCOM now! We have a battle!", NPC_COMM_SMITH, 1);
        }
        else
        {
            AddDialogQuick("Find your allies. See if they know anything.", NPC_COMM_SMITH, 1);
        }
    }
    break;
    case EVENT_SCENE07_BED:
    {
        if (getFlag(GF_TALKED_TO_BUDDY) == 1 && getFlag(GF_TALKED_TO_SMITH) == 1 && getFlag(GF_TALKED_TO_LEE) == 1)
        {
            AddDialogQuick("(my first stasis jump...)", NPC_SELF, 0);
            AddDialogQuick("(I hope we all end up on the\nenter end...)", NPC_SELF, 0);
            FadeOutAndChangeScene(SCENE_08_WALKING_AFTER_JUMP);
        }
    }
    break;
    case EVENT_SCENE07_BUDDY:
    {
        AddDialogQuick("What happened?!?", NPC_BUDDY, 1);
        AddDialogQuick("I think TARI attacked Europe but\nI don't know why.", NPC_SELF, 0);
        AddDialogQuick("I got word we're shipping out!\nThe Ernest King is going to\njump to some alien world.\nWe're going to attack them.", NPC_BUDDY, 1);
        if (getFlag(GF_TALKED_TO_SMITH) == 1)
        {
            AddDialogQuick("I talked to SMITH and he said\nthey already have vectors to some\nalien planets. Someone nuked Europe and\nblamed it on aliens and now\nwe're going to attack!", NPC_SELF, 0);
        }
        if (getFlag(GF_TALKED_TO_LEE) == 0)
        {
            AddDialogQuick("This can't be good...\nI'm going to see what LEE says.", NPC_SELF, 0);
        }

        if (getFlag(GF_TALKED_TO_BUDDY) == 0)
        {
            setFlag(GF_TALKED_TO_BUDDY, 1);
            GlobalScore += 10;
        }
    }
    break;
    case EVENT_SCENE07_SMITH:
    {
        AddDialogQuick("TARI already told me we're\nheading off to some group of alien\nplanets for revenge. They already have the\nvectors. They've been planning this.", NPC_COMM_SMITH, 1);
        AddDialogQuick("The captain is following orders,\nbut I don't know if he suspects TARI of\nfoul play.", NPC_COMM_SMITH, 1);
        AddDialogQuick("Go find your allies. See what they\nknow.", NPC_COMM_SMITH, 1);
        AddDialogQuick("Aye sir.", NPC_SELF, 0);
        if (getFlag(GF_TALKED_TO_SMITH) == 0)
        {
            GlobalScore += 10;
            setFlag(GF_TALKED_TO_SMITH, 1);
        }
    }
    break;
    case EVENT_SCENE07_LEE:
    {
        AddDialogQuick("They're preparing for a jump.\nIt's going to be the biggest jump the\nErnest King has done. This ship\nis powerful, but it can only do a few\njumps like this without a\nfull rebuild!", NPC_ENG_LEE, 1);
        if (getFlag(GF_TALKED_TO_LEE) == 0)
        {
            pageText = "This is the captain.\nAll crew to bunks for stasis jump.\nJump commencing immediately.";
            pageSend = NPC_CAP_LASSIC;
            GlobalScore += 10;
            setFlag(GF_TALKED_TO_LEE, 1);
            setFlagCooldown(GF_PAGE_SELF, 15.0f);
        }
    }
    break;
    case EVENT_SCENE08_TCOM_DOOR:
    {
        if (getFlag(GF_TALKED_TO_LEE) == 1 && getFlag(GF_TALKED_TO_SMITH) == 1 && getFlag(GF_TALKED_TO_TARI) == 1)
        {
            FadeOutAndChangeScene(SCENE_09_BATTLE_TACCOM_MISSION_2);
        }
    }
    break;
    case EVENT_SCENE08_LEE:
    {
        if (getFlag(GF_TALKED_TO_LEE) == 0)
        {
            GlobalScore += 10;
            setFlag(GF_TALKED_TO_LEE, 1);
        }

        AddDialogQuick("LEE, how are the engines?", NPC_SELF, 0);
        AddDialogQuick("Shot to shit. I expect we can\ndo 2 or 3 more jumps max before\nit's inoperable.", NPC_ENG_LEE, 1);
        AddDialogQuick("Son, did you hear about BUDDY?", NPC_ENG_LEE, 1);
        if (getFlag(GF_TALKED_TO_SMITH) == 1 || getFlag(GF_TALKED_TO_TARI) == 1)
        {
            AddDialogQuick("Yes. I heard. Those bastards killed\nhim you know.", NPC_SELF, 0);
            AddDialogQuick("They must have. Nobody's died\nin a jump in a century!", NPC_ENG_LEE, 1);
        }
        else
        {
            AddDialogQuick("BUDDY?", NPC_SELF, 0);
            AddDialogQuick("He died in the jump. Nobody's died\nin a jump in a century. Watch\nyourself out there.", NPC_ENG_LEE, 1);
        }
    }
    break;
    case EVENT_SCENE08_SMITH:
    {
        if (getFlag(GF_TALKED_TO_SMITH) == 0)
        {
            setFlag(GF_TALKED_TO_SMITH, 1);
            GlobalScore += 10;
        }
        AddDialogQuick("I don't know a good way to tell\nyou this. BUDDY died in the jump.", NPC_COMM_SMITH, 1);
        if (getFlag(GF_TALKED_TO_LEE) == 1)
        {
            AddDialogQuick("I heard sir. And that's horse shit.\nThey killed him", NPC_SELF, 0);
        }
        else
        {
            AddDialogQuick("What?!?! they must have killed him.", NPC_SELF, 0);
        }
        AddDialogQuick("He was apparently poking around\nSTRAT COM. It must be TARI.", NPC_COMM_SMITH, 0);
        if (getFlag(GF_TALKED_TO_LEE) == 0)
        {
            AddDialogQuick("Talk to LEE. See how the engines are.", NPC_COMM_SMITH, 1);
            AddDialogQuick("And when you are done...", NPC_COMM_SMITH, 1);
        }
        AddDialogQuick("I have orders to send you to TARI\nfor a briefing. We're ata planet\ndesignated ECHO 7. You will be\ndoing a bombing run.", NPC_COMM_SMITH, 1);
        AddDialogQuick("Stick to your mission for now.\nI'll try to find more answers.", NPC_COMM_SMITH, 1);
        AddDialogQuick("Aye sir.", NPC_SELF, 0);
    }
    break;
    case EVENT_SCENE08_TARI:
        if (getFlag(GF_TALKED_TO_SMITH) == 1 && getFlag(GF_TALKED_TO_LEE) == 1 && getFlag(GF_TALKED_TO_TARI) == 0)
        {
            AddDialogQuick("Sorry to hear about your friend.\nI talked with him briefly outside\nSTRAT COM right before the jump.\nHe seemed lovely.", NPC_INTEL_TARI, 1);
            AddDialogQuick("You are going to be bombing ECHO 7.\nThe planet has complete cloud cover so\nyou are going sensors only.", NPC_INTEL_TARI, 1);
            AddDialogQuick("Go now.", NPC_INTEL_TARI, 1);
            setFlag(GF_TALKED_TO_TARI, 1);
        }
        break;
    case EVENT_ELEVATOR_DOWN:;
        {
            if (CurrentDeck > DECK_ENGR)
            {
                if (getFlag(GF_ELEVATOR_DOWN_ENABLED) == 1 && (allowedDecks[CurrentDeck - 1] == 1))
                {
                    FadeOutAndChangeDeck(CurrentDeck - 1);
                }
                else
                {
                    AddDialogQuick("Access is not authorized at this time.", NPC_NARRATOR, PORTRAIT_NOT_ON_RIGHT);
                }
            }
        }
        break;
    case EVENT_ELEVATOR_UP:;
        {
            if (CurrentDeck < DECK_BRDG)
            {
                if (getFlag(GF_ELEVATOR_UP_ENABLED) && allowedDecks[CurrentDeck + 1] == 1)
                {
                    FadeOutAndChangeDeck(CurrentDeck + 1);
                }
                else
                {
                    AddDialogQuick("Access is not authorized at this time.", NPC_NARRATOR, PORTRAIT_NOT_ON_RIGHT);
                }
            }
        }
        break;
    case EVENT_SCENE10_SMITH:
        if (getFlag(GF_TALKED_TO_SMITH) == 0)
        {
            AddDialogQuick("While they were running that\nmockery of a mission, I found\nthe truth. TARI and the captain\nare in league with a powerful cabal\non Earth.", NPC_COMM_SMITH, 1);
            AddDialogQuick("They detected an anomaly deep\nin space. One that they believe\nwill give them the power to\nrule the Earth themselves, if they\ncan return detailed readings.", NPC_ENG_LEE, 1);
            AddDialogQuick("So they attacked Earth and\nconcocted this story to get\nauthorization to jump out this far.", NPC_COMM_SMITH, 1);
            AddDialogQuick("How do we stop them?", NPC_SELF, 0);
            AddDialogQuick("I was wrong about the engines.\nThey destroyed them making\nthe jump out here. We can't\nmake it home. But they can\ntransmit the data still and complete\ntheir mission.", NPC_ENG_LEE, 1);
            AddDialogQuick("Unless, we destroy the ship.", NPC_COMM_SMITH, 1);
            AddDialogQuick("...", NPC_SELF, 0);
            AddDialogQuick("Is that the only way?", NPC_SELF, 0);
            AddDialogQuick("Yes.", NPC_ENG_LEE, 1);
            AddDialogQuick("I rigged the panel behind me.\nPress it and the reactor overloads.", NPC_ENG_LEE, 1);
            AddDialogQuick("We are on this ship to protect earth.\nAnd that's what we are going\nto do.", NPC_COMM_SMITH, 1);
            AddDialogQuick("Agreed. Who pushes the button?", NPC_SELF, 0);
            AddDialogQuick("In honor of your lost friend,\nyou should do it.", NPC_COMM_SMITH, 1);
            setFlag(GF_TALKED_TO_SMITH, 1);
        }
        else
        {
            AddDialogQuick("Go on son. It's time.", NPC_COMM_SMITH, 1);
        }
        break;
    case EVENT_SCENE10_CONTROL_PANEL:
        if (getFlag(GF_TALKED_TO_BUDDY) == 0)
        {
            setFlagCooldown(GF_TALKED_TO_BUDDY, 1);
            AddDialogQuick("See ya soon BUDDY...", NPC_SELF, 0);
            isWhiteout = 1;
            whiteoutLevel = 0.0f;
            Shake(100);
            setFlagCooldown(GF_SCENE_OVER, 10.0f);
        }
        break;
    }
}

Texture2D tex_walking_ui_ship_deck_mid;
Texture2D tex_walking_ui_ship_deck_top;
Texture2D tex_common_ui_dpad;

// SOME UI DRAWING STUFF
void UIDrawInsetRectangle(int x, int y, int width, int height, Color bgColor, Color darkAccent, Color lightAccent)
{
    DrawRectangle(x, y, width, height, bgColor);
    DrawRectangle(x, y, 2, height, lightAccent);
    DrawRectangle(x, y + height - 2, width, 2, lightAccent);
    DrawRectangle(x, y, width, 2, darkAccent);
    DrawRectangle(x + width - 2, y, 2, height, darkAccent);
}

Texture2D Textures[100];

// MAIN SCENE FUNCTION
void InitSceneSystem(void)
{
    InitDialog();

    npcNames[NPC_CAP_LASSIC] = "LASSIC";
    npcNames[NPC_COMM_SMITH] = "SMITH";
    npcNames[NPC_ENG_LEE] = "LEE";
    npcNames[NPC_INTEL_TARI] = "TARI";
    npcNames[NPC_SHEEP_RUIZ] = "RUIZ";

    tex_walking_ui_ship_deck_mid = LoadTexture("resources/walking-ui-ship-deck-mid.png");
    tex_walking_ui_ship_deck_top = LoadTexture("resources/walking-ui-ship-deck-top.png");
    tex_common_ui_dpad = LoadTexture("resources/common-ui-dpad.png");

    Textures[TEX_BED] = LoadTexture("resources/bed.png");
    Textures[TEX_PLAYER] = LoadTexture("resources/player.png");
    Textures[TEX_DOOR] = LoadTexture("resources/door.png");
    Textures[TEX_RAYLIB] = LoadTexture("resources/raylib_logo.png");
    Textures[TEX_ENGINE_CORE] = LoadTexture("resources/engine_core.png");
    Textures[TEX_BAY_DOORS] = LoadTexture("resources/bay-doors.png");
    Textures[TEX_SHUTTLE] = LoadTexture("resources/shuttle.png");
    Textures[TEX_SHUTTLE_PAD] = LoadTexture("resources/shuttle-pad.png");
    Textures[TEX_CONTROL_PANEL] = LoadTexture("resources/controlpanel.png");

    CurrentScene = SCENE_01_WALKING_INTRO;

    InitScene();
}

void AddPropsFromDeck(int deck)
{
    for (int i = 0; i < DECK_PROP_MAX; i++)
    {
        if (deckProps[deck][i].isActive)
        {
            if (deckProps[deck][i].tex == NULL && deckProps[deck][i].textureId != TEX_NULL)
            {
                deckProps[deck][i].tex = &Textures[deckProps[deck][i].textureId];
            }
            AddProp(deckProps[deck][i]);
        }
    }
}

void LoadDeckProps()
{
    if (CurrentDeck < 0 || CurrentDeck > 11)
        return;
    // load deck props first. if the scene assigns props to the same place those take precedence.
    printf("AddPropsFromDeck %i\n", CurrentDeck);
    AddPropsFromDeck(CurrentDeck);

    // do the top 8 decks
    if (CurrentDeck > DECK_ENGR)
    {
        printf("AddPropsFromDeck %i\n", DECK_TOP_8);
        AddPropsFromDeck(DECK_TOP_8);
    }

    // do teh bottom 8 decks
    if (CurrentDeck < DECK_BRDG)
    {
        printf("AddPropsFroMDeck %i\n", DECK_BOTTOM_8);
        AddPropsFromDeck(DECK_BOTTOM_8);
    }
}

void LoadSceneProps()
{
    for (int i = 0; i < SCENE_PROP_MAX; i++)
    {
        if (scene[CurrentScene].props[i].isActive && scene[CurrentScene].props[i].deckId == CurrentDeck)
        {
            if (scene[CurrentScene].props[i].tex == NULL && scene[CurrentScene].props[i].textureId != TEX_NULL)
            {
                scene[CurrentScene].props[i].tex = &Textures[scene[CurrentScene].props[i].textureId];
            }
            AddProp(scene[CurrentScene].props[i]);
        }
    }
}

void InitScene(void)
{
    printf("InitScene %i\n", CurrentScene);

    // elevators on by default because now it uses allowedDecks to control access to individual decks based on the state of the scene.
    setFlag(GF_ELEVATOR_UP_ENABLED, 1);
    setFlag(GF_ELEVATOR_DOWN_ENABLED, 1);

    // reset what decks the player is allowed on.
    for (int i = 0; i < 9; i++)
    {
        allowedDecks[i] = 0;
    }

    // set up game flags

    switch (CurrentScene)
    {
    case SCENE_01_WALKING_INTRO:
        allowedDecks[DECK_DOCK] = 1;
        PlayerPos.x = 5 * 32 + Textures[TEX_PLAYER].height / 2;
        PlayerPos.y = 8 * 32 + Textures[TEX_PLAYER].width / 2;
        break;
    case SCENE_03_WALKING_AFTER_PRACTICE_1:
        allowedDecks[DECK_TCOM] = 1;
        allowedDecks[DECK_LiVE] = 1;
        allowedDecks[DECK_DOCK] = 1;
        allowedDecks[DECK_ENGR] = 1;
        PlayerPos.x = 9 * 32 + Textures[TEX_PLAYER].height / 2;
        PlayerPos.y = 4 * 32 + Textures[TEX_PLAYER].width / 2;
        setFlag(GF_ELEVATOR_UP_ENABLED, 0);
        setFlag(GF_ELEVATOR_DOWN_ENABLED, 0);
        break;
    case SCENE_05_WALKING_AFTER_PRACTICE_2:
        allowedDecks[DECK_TCOM] = 1;
        allowedDecks[DECK_LiVE] = 1;
        allowedDecks[DECK_DOCK] = 1;
        allowedDecks[DECK_ENGR] = 1;
        PlayerPos.x = 9 * 32 + Textures[TEX_PLAYER].height / 2;
        PlayerPos.y = 4 * 32 + Textures[TEX_PLAYER].width / 2;
        break;
    case SCENE_07_WALKING_AFTER_BATTLE_1:
        allowedDecks[DECK_TCOM] = 1;
        allowedDecks[DECK_LiVE] = 1;
        allowedDecks[DECK_DOCK] = 1;
        allowedDecks[DECK_ENGR] = 1;
        PlayerPos.x = 9 * 32 + Textures[TEX_PLAYER].height / 2;
        PlayerPos.y = 4 * 32 + Textures[TEX_PLAYER].width / 2;
        break;
    case SCENE_08_WALKING_AFTER_JUMP:
        allowedDecks[DECK_TCOM] = 1;
        allowedDecks[DECK_LiVE] = 1;
        allowedDecks[DECK_DOCK] = 1;
        allowedDecks[DECK_ENGR] = 1;
        PlayerPos.x = 5 * 32 + Textures[TEX_PLAYER].height / 2;
        PlayerPos.y = 8 * 32 + Textures[TEX_PLAYER].width / 2;
        setFlag(GF_SHIP_MOVING, 1);
        break;
    case SCENE_10_WALKING_AFTER_BATTLE_2:
        allowedDecks[DECK_TCOM] = 1;
        allowedDecks[DECK_LiVE] = 1;
        allowedDecks[DECK_DOCK] = 1;
        allowedDecks[DECK_ENGR] = 1;
        PlayerPos.x = 9 * 32 + Textures[TEX_PLAYER].height / 2;
        PlayerPos.y = 4 * 32 + Textures[TEX_PLAYER].width / 2;
        break;
    case SCENE_11_WALKING_BRIDGE_1:
        isWhiteout = 1;
        whiteoutLevel = 1.0f;
        break;
    }

    printf("InitProps\n");
    InitProps();

    CurrentDeck = scene[CurrentScene].startingDeck;

    printf("LoadDeckProps\n");
    LoadDeckProps();

    printf("LoadSceneProps\n");
    LoadSceneProps();

    // if the scene starts with a cutscene add it to the dialog queue.
    printf("Load cutscene dialoge\n");
    if (scene[CurrentScene].hasCutScene)
    {
        for (int i = 0; i < SCENE_CUTSCENE_DIALOG_MAX; i++)
        {
            if (scene[CurrentScene].cutSceneEntries[i].speaker > NPC_NULL)
            {
                AddDialog(scene[CurrentScene].cutSceneEntries[i]);
            }
        }
    }

    printf("Load big stars\n");
    for (int i = 0; i < BIG_STAR_COUNT; i++)
    {
        bigStars[i].x = GetRandomValue(-screenWidth / 2, screenWidth / 2);
        bigStars[i].y = GetRandomValue(-(screenWidth - 128) / 2, (screenWidth - 128) / 2);
        bigStars[i].z = GetRandomValue(1, 4);
        bigStars[i].z = bigStars[i].z * bigStars[i].z;
    }

    printf("FadeIn()\n");
    FadeIn();

    if (scene[CurrentScene].playMode == MODE_BATTLE)
        InitBattle();
}

void ChangeDeck(int newDeck)
{
    printf("ChangeDeck to %i\n", newDeck);
    CurrentDeck = newDeck;

    printf("Call InitProps\n");
    InitProps();

    printf("Call LoadDeckProps\n");
    LoadDeckProps();
    LoadSceneProps();

    printf("Call FadeIn\n");
    FadeIn();

    // Change player position after deck change, always put them by the elevators
    PlayerPos.x = 9 * 32 + Textures[TEX_PLAYER].height / 2;
    PlayerPos.y = 15 * 32 + Textures[TEX_PLAYER].width / 2;
}

void UpdateScene(void)
{
    float delta = GetFrameTime();
    UpdateDialog(delta);
    updateFlags(delta);

    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            PropDef *pd = GetProp(x, y);
            if (pd && pd->pulseRate != 0)
            {
                // PULSING ISN'T WORKING YET
                printf("Update pulse rate\n");
                pd->currentAlpha += pd->pulseRate * delta;
                if (pd->currentAlpha > 1.0)
                {
                    pd->pulseRate = -1 * pd->pulseRate;
                    pd->currentAlpha = 1.0f;
                }
                else if (pd->currentAlpha < 0.5)
                {
                    pd->currentAlpha = 0.5;
                    pd->pulseRate = -1 * pd->pulseRate;
                }
            }
        }
    }

    // dialog has to update every tick in case new dialog is added. process its input here
    // dialog can be open in WALKING or BATTLE and if dialog is open stop updating the game.
    if (IsDialogOpen)
    {
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
        {
            if (IsDialogComplete)
            {
                CloseCurrentDialog();
            }
            else
            {
                SkipDialogAnimation();
            }
        }
    }
    else
    {
        if (isFadingIn)
        {
            curtainFadeLevel -= delta;
            if (curtainFadeLevel < 0)
            {
                curtainFadeLevel = 0;
                isFadingIn = false;
                AfterFadeIn();
            }
            else
            {
                return;
            }
        }
        if (isFadingOut)
        {
            curtainFadeLevel += delta;
            if (curtainFadeLevel > 1.0f)
            {
                curtainFadeLevel = 1.0f;
                isFadingOut = false;
                AfterFadeOut();
            }
            else
            {
                return;
            }
        }

        // the game scene only has to update when the dialog is closed.
        if (scene[CurrentScene].playMode == MODE_WALKING)
        {
            UpdateBigStars(delta);
            UpdateWalkingUI(delta);
            UpdateWalkingMap(delta);
        }
        else if (scene[CurrentScene].playMode == MODE_BATTLE)
        {
            int battleResult = UpdateBattle(delta);
            if (battleResult > -1)
            {
                FadeOutAndChangeScene(battleResult);
            }
        }
    }

    if (getFlag(GF_PAGE_SELF) == 1)
    {
        setFlag(GF_PAGE_SELF, 0);
        AddDialogQuick(pageText, pageSend, 1);
        if (CurrentScene == SCENE_05_WALKING_AFTER_PRACTICE_2)
        {
            setFlag(GF_TALKED_TO_SMITH, 1);
        }
    }

    if (isWhiteout)
    {
        whiteoutLevel += delta;
        if (whiteoutLevel > 1.0f)
            whiteoutLevel = 1.0f;
    }

    if (CurrentScene == SCENE_10_WALKING_AFTER_BATTLE_2 && getFlag(GF_SCENE_OVER) == 1)
    {
        FadeOutAndChangeScene(SCENE_11_WALKING_BRIDGE_1);
    }

}

void DrawScene(void)
{
    ClearBackground(BLACK);

    // draw the scene.
    if (scene[CurrentScene].playMode == MODE_WALKING)
    {
        DrawBigStars();
        DrawWalkingMap();
        DrawWalkingUI();
    }
    else if (scene[CurrentScene].playMode == MODE_BATTLE)
    {
        DrawBattle();
    }

    if (isFadingIn || isFadingOut)
    {
        Color color = ColorAlpha(BLACK, curtainFadeLevel);
        DrawRectangle(0, 0, screenWidth, screenHeight - 128, color);
    }

    if (isWhiteout)
    {
        Color white = ColorAlpha(WHITE, whiteoutLevel);
        DrawRectangle(0, 0, screenWidth, screenHeight, white);
    }

    DrawDialog();

#ifdef _DEBUG
    DrawDebug();
#endif
}

void UnloadScene(void)
{
}

// WALKING UI
void DrawWalkingUI()
{
    DrawRectangle(0, screenHeight - 128, screenWidth, 128, GRAY);
    DrawRectangle(0, screenHeight - 132, screenWidth, 4, LIGHTGRAY);

    UIDrawInsetRectangle(64 - tex_common_ui_dpad.width / 2.0f - 4, screenHeight - 64 - tex_common_ui_dpad.height / 2.0f - 4, tex_common_ui_dpad.width + 8, tex_common_ui_dpad.height + 8, darkerGray, DARKGRAY, LIGHTGRAY);
    DrawTexture(tex_common_ui_dpad, 64 - tex_common_ui_dpad.width / 2.0f, screenHeight - 64 - tex_common_ui_dpad.height / 2.0f, WHITE);

    Texture2D *deckTex;
    Color deckColor;

    UIDrawInsetRectangle(122, 335, 300, 98, BLACK, DARKGRAY, LIGHTGRAY);

    // deck display is 256x61 right now
    // surround for it is 296x94

    for (int i = DECK_ENGR; i <= DECK_BRDG; i++)
    {
        if (i == DECK_BRDG)
        {
            // draw top
            deckTex = &tex_walking_ui_ship_deck_top;
        }
        else
        {
            // draw mid
            deckTex = &tex_walking_ui_ship_deck_mid;
        }

        if (CurrentDeck == i)
        {
            DrawText(deckNames[i], 146 + (i - 1) * 26, 345, font.baseSize, deckColors[i]);
        }

        if (CurrentDeck == i && CurrentDeckHighlighted == 1)
        {
            deckColor = deckColors[i];
        }
        else
        {
            deckColor = ColorAlpha(deckColors[i], 0.25f);
        }

        DrawTexture(*deckTex, 146 + (i - 1) * 26, 361, deckColor);
    }

    UIDrawInsetRectangle(434, 335, 65, 98, RED, transparentWhite, transparentBlack);
    DrawCircleGradient(434 + 65 / 2, 335 + 98 / 2, 65 / 2 - 2, YELLOW, RED);

    DrawText(TextFormat("%04i", GlobalScore), screenWidth - MeasureText("0000", 40) - 5, 5, 40, GREEN);
}

void UpdateWalkingUI(float delta)
{
    CurrentDeckFlashTimeout -= delta;
    if (CurrentDeckFlashTimeout <= 0)
    {
        CurrentDeckFlashTimeout = 0.25f;
        if (CurrentDeckHighlighted == 1)
            CurrentDeckHighlighted = 0;
        else
            CurrentDeckHighlighted = 1;
    }
}

// WALKING MAP
void DrawWalkingMap()
{
    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            int gx = (x * 32) + screenWidth / 2 - PlayerPos.x;
            int gy = (y * 32) + (screenHeight - 128) / 2 - PlayerPos.y;

            // zero tiles should be transparent
            if (map[CurrentDeck][x][y] > 0)
            {
                DrawRectangle(gx, gy, 32, 32, tileColors[map[CurrentDeck][x][y]]);
            }

            if (map[CurrentDeck][x][y] == 2)
            {
                DrawRectangleLines(gx, gy, 32, 32, transparentBlack);
            }

            if (x > 0 && x < mapWidth - 1 && y > 0 && y < mapHeight - 1)
            {
                // walls
                if (map[CurrentDeck][x][y] == 1)
                {
                    // this first one is a special case for criss cross single rows of tiles.
                    if (map[CurrentDeck][x][y - 1] == 1 && map[CurrentDeck][x + 1][y] == 1 && map[CurrentDeck][x][y + 1] == 1 && map[CurrentDeck][x - 1][y] == 1 && map[CurrentDeck][x + 1][y - 1] > 1 && map[CurrentDeck][x + 1][y + 1] > 1 && map[CurrentDeck][x - 1][y + 1] > 1 && map[CurrentDeck][x - 1][y - 1] > 1)
                    {
                        DrawLine(gx + 16, gy, gx + 16, gy + 32, WHITE);
                        DrawLine(gx, gy + 16, gx + 32, gy + 16, WHITE);
                        DrawLine(gx, gy, gx + 32, gy + 32, WHITE);
                        DrawLine(gx, gy + 32, gx + 32, gy, WHITE);
                    }
                    else if (map[CurrentDeck][x][y - 1] == 1 && map[CurrentDeck][x][y + 1] == 1 && (map[CurrentDeck][x + 1][y] > 1 || map[currentScreen][x - 1][y] > 1))
                    {
                        DrawLine(gx + 16, gy, gx + 16, gy + 32, WHITE);
                    }
                    else if (map[CurrentDeck][x - 1][y] == 1 && map[CurrentDeck][x + 1][y] == 1 && (map[CurrentDeck][x][y + 1] > 1 || map[CurrentDeck][x][y - 1] > 1))
                    {
                        DrawLine(gx, gy + 16, gx + 32, gy + 16, WHITE);
                    }
                    else if (map[CurrentDeck][x][y + 1] == 1 && map[CurrentDeck][x + 1][y] == 1 && map[CurrentDeck][x + 1][y + 1] > 1)
                    {
                        DrawLine(gx + 16, gy + 32, gx + 16, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 32, gy + 16, WHITE);
                        DrawLine(gx + 32, gy + 32, gx + 16, gy + 16, WHITE);
                    }
                    else if (map[CurrentDeck][x - 1][y] == 1 && map[CurrentDeck][x][y - 1] == 1 && map[CurrentDeck][x + 1][y + 1] > 1)
                    {
                        DrawLine(gx, gy + 16, gx + 16, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 16, gy, WHITE);
                        DrawLine(gx + 32, gy + 32, gx + 16, gy + 16, WHITE);
                    }
                    else if (map[CurrentDeck][x][y - 1] == 1 && map[CurrentDeck][x + 1][y] == 1 && map[CurrentDeck][x + 1][y - 1] > 1)
                    {
                        DrawLine(gx + 16, gy, gx + 16, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 32, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 32, gy, WHITE);
                    }
                    else if (map[CurrentDeck][x - 1][y] == 1 && map[CurrentDeck][x][y + 1] == 1 && map[CurrentDeck][x + 1][y - 1] > 1)
                    {
                        DrawLine(gx, gy + 16, gx + 16, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 16, gy + 32, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 32, gy, WHITE);
                    }
                    else if (map[CurrentDeck][x - 1][y] == 1 && map[CurrentDeck][x][y - 1] == 1 && map[CurrentDeck][x - 1][y - 1] > 1)
                    {
                        DrawLine(gx, gy + 16, gx + 16, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 16, gy, WHITE);
                        DrawLine(gx + 16, gy + 16, gx, gy, WHITE);
                    }
                    else if (map[CurrentDeck][x][y + 1] == 1 && map[CurrentDeck][x + 1][y] == 1 && map[CurrentDeck][x - 1][y - 1] > 1)
                    {
                        DrawLine(gx + 16, gy + 32, gx + 16, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 32, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx, gy, WHITE);
                    }
                    else if (map[CurrentDeck][x - 1][y] == 1 && map[CurrentDeck][x][y + 1] == 1 && map[CurrentDeck][x - 1][y + 1] > 1)
                    {
                        DrawLine(gx, gy + 16, gx + 16, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 16, gy + 32, WHITE);
                        DrawLine(gx, gy + 32, gx + 16, gy + 16, WHITE);
                    }
                    else if (map[CurrentDeck][x][y - 1] == 1 && map[CurrentDeck][x + 1][y] == 1 && map[CurrentDeck][x - 1][y + 1] > 1)
                    {
                        DrawLine(gx + 16, gy, gx + 16, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 32, gy + 16, WHITE);
                        DrawLine(gx, gy + 32, gx + 16, gy + 16, WHITE);
                    }
                    // ENDCAPS
                    else if (map[CurrentDeck][x][y + 1] == 1 && map[CurrentDeck][x][y - 1] > 1)
                    {
                        DrawLine(gx, gy, gx + 16, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 32, gy, WHITE);
                        DrawLine(gx + 16, gy + 32, gx + 16, gy + 16, WHITE);
                    }
                    else if (map[CurrentDeck][x][y - 1] == 1 && map[CurrentDeck][x][y + 1] > 1)
                    {
                        DrawLine(gx, gy + 32, gx + 16, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 32, gy + 32, WHITE);
                        DrawLine(gx + 16, gy, gx + 16, gy + 16, WHITE);
                    }
                    else if (map[CurrentDeck][x - 1][y] == 1 && map[CurrentDeck][x + 1][y] > 1)
                    {
                        DrawLine(gx + 32, gy, gx + 16, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 32, gy + 32, WHITE);
                        DrawLine(gx, gy + 16, gx + 16, gy + 16, WHITE);
                    }
                    else if (map[CurrentDeck][x + 1][y] == 1 && map[CurrentDeck][x - 1][y] > 1)
                    {
                        DrawLine(gx, gy, gx + 16, gy + 16, WHITE);
                        DrawLine(gx + 16, gy + 16, gx, gy + 32, WHITE);
                        DrawLine(gx + 16, gy + 16, gx + 32, gy + 16, WHITE);
                    }
                }
                else if (map[CurrentDeck][x][y] == 0)
                {
                    if (map[CurrentDeck][x][y + 1] == 1 && map[CurrentDeck][x + 1][y] == 1)
                    {
                        DrawTriangle((Vector2){gx + 32, gy}, (Vector2){gx, gy + 32}, (Vector2){gx + 32, gy + 32}, tileColors[1]);
                    }
                    else if (map[CurrentDeck][x - 1][y] == 1 && map[CurrentDeck][x][y + 1] == 1)
                    {
                        DrawTriangle((Vector2){gx, gy}, (Vector2){gx, gy + 32}, (Vector2){gx + 32, gy + 32}, tileColors[1]);
                    }
                    else if (map[CurrentDeck][x][y - 1] == 1 && map[CurrentDeck][x - 1][y] == 1)
                    {
                        DrawTriangle((Vector2){gx + 32, gy}, (Vector2){gx, gy}, (Vector2){gx, gy + 32}, tileColors[1]);
                    }
                    else if (map[CurrentDeck][x][y - 1] == 1 && map[CurrentDeck][x + 1][y] == 1)
                    {
                        DrawTriangle((Vector2){gx, gy}, (Vector2){gx + 32, gy + 32}, (Vector2){gx + 32, gy}, tileColors[1]);
                    }
                }
            }
        }
    }

    if (getFlag(GF_ON_NPC_SQUARE))
    {
        int px = (int)PlayerPos.x / 32;
        int py = (int)PlayerPos.y / 32;
        DrawRectangleLines(px * 32 + screenWidth / 2 - PlayerPos.x, py * 32 + (screenHeight - 128) / 2 - PlayerPos.y, 32, 32, GREEN);
        DrawRectangleLines(px * 32 + screenWidth / 2 - PlayerPos.x - 2, py * 32 + (screenHeight - 128) / 2 - PlayerPos.y - 2, 36, 36, GREEN);
        DrawText("INTERACT", px * 32 + screenWidth / 2 - PlayerPos.x - 10, (py + 1) * 32 + (screenHeight - 128) / 2 - PlayerPos.y + 3, font.baseSize, DARKGREEN);
        DrawText("INTERACT", px * 32 + screenWidth / 2 - PlayerPos.x - 11, (py + 1) * 32 + (screenHeight - 128) / 2 - PlayerPos.y + 2, font.baseSize, GREEN);
    }

    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            PropDef *prop = GetProp(x, y);
            if (prop != NULL)
            {
                Color color = WHITE;
                if (prop->pulseRate != 0)
                {
                    color = ColorAlpha(WHITE, prop->currentAlpha);
                }

                if (prop->label != NO_LABEL)
                {
                    DrawText(prop->label, x * 32 + screenWidth / 2 - PlayerPos.x, y * 32 + (screenHeight - 128) / 2 - PlayerPos.y - 8, font.baseSize, BLUE);
                }
                DrawTexturePro(*prop->tex, (Rectangle){0, 0, prop->tex->width, prop->tex->height}, (Rectangle){x * 32 + screenWidth / 2 - PlayerPos.x + prop->tex->width / 2, y * 32 + (screenHeight - 128) / 2 - PlayerPos.y + prop->tex->height / 2, prop->tex->width, prop->tex->height}, (Vector2){prop->tex->width / 2, prop->tex->height / 2}, prop->rotation, color);
            }
        }
    }

    //DrawTexturePro(Textures[TEX_PLAYER], (Rectangle){0, 0, Textures[TEX_PLAYER].width, Textures[TEX_PLAYER].height}, (Rectangle){PlayerPos.x, PlayerPos.y, Textures[TEX_PLAYER].width, Textures[TEX_PLAYER].height}, (Vector2){Textures[TEX_PLAYER].width / 2.0f, Textures[TEX_PLAYER].height / 2.0f},PlayerDirection, WHITE);
    DrawTexturePro(Textures[TEX_PLAYER], (Rectangle){0, 0, Textures[TEX_PLAYER].width, Textures[TEX_PLAYER].height}, (Rectangle){screenWidth / 2, (screenHeight - 128) / 2, Textures[TEX_PLAYER].width, Textures[TEX_PLAYER].height}, (Vector2){Textures[TEX_PLAYER].width / 2, Textures[TEX_PLAYER].height / 2}, PlayerDirection, WHITE);
}

void UpdateWalkingMap(float delta)
{
    Vector2 newPos = {PlayerPos.x, PlayerPos.y};

    if (IsKeyDown(KEY_D) || isRightTouched())
    {
        newPos.x += delta * playerVel;
        PlayerDirection = FACING_RIGHT;
    }
    else if (IsKeyDown(KEY_S) || isDownTouched())
    {
        newPos.y += delta * playerVel;
        PlayerDirection = FACING_DOWN;
    }
    else if (IsKeyDown(KEY_A) || isLeftTouched())
    {
        newPos.x -= delta * playerVel;
        PlayerDirection = FACING_LEFT;
    }
    else if (IsKeyDown(KEY_W) || isUpTouched())
    {
        newPos.y -= delta * playerVel;
        PlayerDirection = FACING_UP;
    }

    if (PlayerDirection == FACING_RIGHT)
    {
        // check top right.
        int x = (int)(newPos.x + Textures[TEX_PLAYER].width / 2.0f) / 32;
        int y = (int)(newPos.y - Textures[TEX_PLAYER].height / 3.0f) / 32;
        if (map[CurrentDeck][x][y] == 1)
        {
            newPos.x = (x)*32 - Textures[TEX_PLAYER].width / 2.0f;
        }

        // check bottom right.
        x = (int)(newPos.x + Textures[TEX_PLAYER].width / 2.0f) / 32;
        y = (int)(newPos.y + Textures[TEX_PLAYER].height / 3.0f) / 32;
        if (map[CurrentDeck][x][y] == 1)
        {
            newPos.x = (x)*32 - Textures[TEX_PLAYER].width / 2.0f;
        }
    }
    else if (PlayerDirection == FACING_LEFT)
    {
        // check top left.
        int x = (int)(newPos.x - Textures[TEX_PLAYER].width / 2.0f) / 32;
        int y = (int)(newPos.y - Textures[TEX_PLAYER].height / 3.0f) / 32;
        if (map[CurrentDeck][x][y] == 1)
        {
            newPos.x = (x + 1) * 32 + Textures[TEX_PLAYER].width / 2.0f;
        }

        // check bottom left.
        x = (int)(newPos.x - Textures[TEX_PLAYER].width / 2.0f) / 32;
        y = (int)(newPos.y + Textures[TEX_PLAYER].height / 3.0f) / 32;
        if (map[CurrentDeck][x][y] == 1)
        {
            newPos.x = (x + 1) * 32 + Textures[TEX_PLAYER].width / 2.0f;
        }
    }
    else if (PlayerDirection == FACING_UP)
    {
        // check top left
        int x = (int)(newPos.x - Textures[TEX_PLAYER].width / 3.0f) / 32;
        int y = (int)(newPos.y - Textures[TEX_PLAYER].height / 2.0f) / 32;
        if (map[CurrentDeck][x][y] == 1)
        {
            newPos.y = (y + 1) * 32 + Textures[TEX_PLAYER].height / 2.0f;
        }

        // check top right
        x = (int)(newPos.x + Textures[TEX_PLAYER].width / 3.0f) / 32;
        y = (int)(newPos.y - Textures[TEX_PLAYER].height / 2.0f) / 32;
        if (map[CurrentDeck][x][y] == 1)
        {
            newPos.y = (y + 1) * 32 + Textures[TEX_PLAYER].height / 2.0f;
        }
    }
    else if (PlayerDirection == FACING_DOWN)
    {
        // check bottom left
        int x = (int)(newPos.x - Textures[TEX_PLAYER].width / 3.0f) / 32;
        int y = (int)(newPos.y + Textures[TEX_PLAYER].height / 2.0f) / 32;
        if (map[CurrentDeck][x][y] == 1)
        {
            newPos.y = (y)*32 - Textures[TEX_PLAYER].height / 2.0f;
        }

        // check bottom right
        x = (int)(newPos.x + Textures[TEX_PLAYER].width / 3.0f) / 32;
        y = (int)(newPos.y + Textures[TEX_PLAYER].height / 2.0f) / 32;
        if (map[CurrentDeck][x][y] == 1)
        {
            newPos.y = (y)*32 - Textures[TEX_PLAYER].height / 2.0f;
        }
    }

    PlayerPos.x = newPos.x;
    PlayerPos.y = newPos.y;

    PlayerColBox.x = PlayerPos.x - Textures[TEX_PLAYER].width / 2.0f;
    PlayerColBox.y = PlayerPos.y - Textures[TEX_PLAYER].height / 2.0f;
    PlayerColBox.width = Textures[TEX_PLAYER].width;
    PlayerColBox.height = Textures[TEX_PLAYER].height;

    // use the center of the player to find the single grid square it is in and check props there.
    int newGridX = (int)PlayerPos.x / 32;
    int newGridY = (int)PlayerPos.y / 32;

    PropDef *prop = GetProp(newGridX, newGridY);
    if (prop != NULL)
    {
        setFlag(GF_ON_NPC_SQUARE, 1);
    }
    else
    {
        setFlag(GF_ON_NPC_SQUARE, 0);
    }

    if (IsKeyPressed(KEY_ENTER) || isFirePressed())
    {
        if (prop != NULL)
        {
            if (prop->eventId > 0 && prop->isEventLive)
            {
                RunEvent(prop->eventId);
                // leave events live and use gameflags to prevent multiple hits
                // gameflags are more efficient to reactivate than gameprops.
                //prop->isEventLive = 0;
            }
        }
    }
}

void UpdateBigStars(float delta)
{
    if (!getFlag(GF_SHIP_MOVING))
        return;
    for (int i = 0; i < BIG_STAR_COUNT; i++)
    {
        bigStars[i].x = bigStars[i].x * (1 - ((17 - bigStars[i].z) / 16) * delta);
        bigStars[i].y = bigStars[i].y * (1 - ((17 - bigStars[i].z) / 16) * delta);

        if (bigStars[i].x > -25 && bigStars[i].x < 25 && bigStars[i].y > -25 && bigStars[i].y < 25)
        {
            int place = GetRandomValue(0, 3);

            switch (place)
            {
            case 0:
                bigStars[i].x = GetRandomValue(-screenWidth / 2, screenWidth / 2);
                bigStars[i].y = -(screenHeight - 128) / 2 - 10;
                break;
            case 1:
                bigStars[i].x = screenWidth / 2 + 10;
                bigStars[i].y = GetRandomValue((screenHeight - 128) / -2, (screenHeight - 128) / 2);
                break;
            case 2:
                bigStars[i].x = GetRandomValue(-screenWidth / 2, screenWidth / 2);
                bigStars[i].y = (screenHeight - 128) / 2 + 10;
                break;
            case 3:
                bigStars[i].x = screenWidth / -2 - 10;
                bigStars[i].y = GetRandomValue((screenHeight - 128) / -2, (screenHeight - 128) / 2);
                break;
            }
        }
    }
}

void DrawBigStars(void)
{
    for (int i = 0; i < BIG_STAR_COUNT; i++)
    {
        DrawCircle(bigStars[i].x + screenWidth / 2, bigStars[i].y + (screenHeight - 128) / 2, 0.1 * ((1 / bigStars[i].z) * 10 + 5), WHITE);
    }
}

// DEBUG STUFF
void DrawDebug()
{
    switch (scene[CurrentScene].playMode)
    {
    case MODE_WALKING:
        modeText = "WALKING";
        break;
    case MODE_BATTLE:
        modeText = "BATTLE";
        break;
    default:
        modeText = "UNKNOWN";
        break;
    }

    DrawText(scene[CurrentScene].name, 10, 10, 20, RED);
    DrawText(modeText, 10, 30, 20, RED);
}

// FADING ROUTINES
void FadeIn()
{
    isFadingIn = true;
    isFadingOut = false;
    curtainFadeLevel = 1.0f;
}

void FadeOut()
{
    isFadingIn = false;
    isFadingOut = true;
    curtainFadeLevel = 0.0f;
}

void FadeOutAndIn()
{
    FadeOut();
    fadeBackInAutomatically = 1;
}

void FadeOutAndChangeDeck(int newDeck)
{
    FadeOut();
    fadeOutDeckChange = newDeck;
}

void FadeOutAndChangeScene(int newScene)
{
    fadeOutSceneChange = newScene;
    FadeOut();
}

// events

void AfterFadeOut()
{
    if (fadeOutDialogWaiting)
    {
        AddDialog(fadeOutDialog);
        fadeOutDialogWaiting = 0;
        if (fadeBackInAutomatically)
        {
            FadeIn();
            fadeBackInAutomatically = 0;
        }
    }

    if (fadeOutDeckChange > -1)
    {
        ChangeDeck(fadeOutDeckChange);
        fadeOutDeckChange = -1;
    }

    if (fadeOutSceneChange > -1)
    {
        CurrentScene = fadeOutSceneChange;
        InitScene();
        fadeOutSceneChange = -1;
    }
}

void AfterFadeIn()
{
    if (fadeInDialogWaiting)
    {
        AddDialog(fadeInDialog);
        fadeInDialogWaiting = 0;
    }
}