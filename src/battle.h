#include "raylib.h"

#ifndef BATTLE_H
#define BATTLE_H

typedef enum Grounds
{
  GROUND_NULL = -1,
  GROUND_EUROPE = 0,
  GROUND_EUROPE_DESTROYED,
  GROUND_CITY,
  GROUND_ISLAND,
  GROUND_ECHO7,
  GROUND_OSCAR2
} Grounds;

typedef enum Clouds
{
  CLOUD_NONE = 0,
  CLOUD_PARTLY,
  CLOUD_MOSTLY,
  CLOUD_TINY_WINDOWS,
  CLOUD_COMPLETE_COVER
} Clouds;

typedef enum TargetTypes
{
  TARGET_ENEMY = 0,
  TARGET_FRIENDLY,
  TARGET_SCHOOL,
  TARGET_FACTORY
} TargetTypes;

typedef enum Battles
{
  B1_PRACTICE_01 = 0,
  B2_PRACTICE_02,
  B3_MISSION_01,
  B4_MISSION_02,
  B5_MISSION_03
} Battles;

typedef struct BattleDef
{
  char *name;
  int groundLayer;
  int cloudLevel;
  float scrollSpeed;
} BattleDef;

typedef struct BulletDef
{
  int isActive;
  float x;
  float y;
  float z;
  float explosionCountdown;
  float blastRadius;
  int isCrater;
} BulletDef;



void InitBattle();

// returns -1 if it's still going or a scene number to change to a new scene.
int UpdateBattle(float delta);

void DrawBattle(void);

#endif