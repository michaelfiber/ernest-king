#ifndef GAME_FLAGS_H
#define GAME_FLAGS_H

#define GAMEFLAG_MAX 100

typedef enum GameFlags
{
    GF_NULL = 0,
    GF_ON_NPC_SQUARE,
    GF_ELEVATOR_UP_ENABLED,
    GF_ELEVATOR_DOWN_ENABLED,
    GF_TALKED_TO_BUDDY,
    GF_TALKED_TO_RUIZ,
    GF_TALKED_TO_LEE,
    GF_TALKED_TO_TARI,
    GF_HAS_SLEPT,
    GF_SHIP_MOVING,
    GF_FIRST_FIRE,
    GF_SCENE_OVER,
    GF_BATTLE_INTRO_OVER,
    GF_BATTLE_ZOOMED_OUT,
    GF_TALKED_TO_SMITH,
    GF_SHOW_CUTSCENE,
    GF_SHOW_EXPLOSION,
    GF_PAGE_SELF,
    GF_REACT_TO_GROUND_TARGET
} GameFlags;

// returns 1 if the flag is set or 0 if it is not. the actual flag data stored can also be 2 meaning that a cooldown is happening.
int getFlag(int flag);

// set the flag to 0 for false or 1 for true. values greater than 1 will be converted to one. values less than zero will be converted to zero.
void setFlag(int flag, int value);

// sets the flag to true after the cooldown period. cooldown should pause during dialog.
void setFlagCooldown(int flag, float cooldown);

// updates the status of flags. should not run during dialogs.
void updateFlags(float delta);

// zero out all flags and reset all cooldowns
void zeroFlags();

#endif