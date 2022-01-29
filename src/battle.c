#include <stdio.h>
#include "battle.h"
#include "dialog.h"
#include "scene.h"
#include "screens.h"
#include "game_flags.h"

#define FLICKER_TIMEOUT 0.02f
#define MAX_CLOUDS 10
#define MAX_TARGETS 20
#define MAX_BULLETS 10

void DrawUI();
void DrawMarker(Vector2 pos, int targetType, int size);
void DrawTargeter();
void DrawClouds();
void DrawGround();
void DrawTargets();
void Fire();
void updateOffset();

static float blastRadius = 30.0f;
static float altitude = 5.0f;
static float groundSpeed = 10.0f;
static float cloudSpeed = 1.0f;
static int currentGround = GROUND_NULL;
static float worldZoom = 2.0f;
static int shotCounter = 0;
static int enemiesHit = 0;
static int friendliesHit = 0;
static int controlsActive = 0;
static float whiteoutLevel = 0.0f;

// number from 1 to 5 indicating how dense it is.
static int cloudIntensity = 5;

static float targeterSpeed = 20.0f;

static int currentBattle = -1;
static int nextScene = -1;
static int isUsingBattleTimer = 0;
static float battleTimer = -1;
static Vector3 targeterPos = {0};
static Vector2 offset = {0};
static float flickerTimer = 0.0f;
static int flickerOff = 0;
static int firstFire = 0;
static Vector3 clouds[MAX_CLOUDS] = {0};
static Vector2 worldImageOffset = {0};
static Vector4 targets[MAX_TARGETS] = {0};
static Texture2D dpad;
static Texture2D crater;
static Color UiPanelColor;
static Color UiDarkRedColor;
static Color UiDarkerRedColor;
static Color TargeterRedColor;
static Color TargeterRedTransparent;
static Color TargeterGreenColor;
static Color TargeterGreenTransparent;
static Color TargeterYellowColor;
static Color TargeterYellowTransparent;
static Color CloudWhite;
static Color transparentBlack = {
    0, 0, 0, 64};
static Color transparentWhite = {
    255, 255, 255, 64};

static BulletDef bullets[MAX_BULLETS] = {0};

static BattleDef battles[] = {
    {
        //
        "Practice Mission 01 - European Sim",
        GROUND_EUROPE,
        CLOUD_PARTLY //
    },               //
    {
        //
        "Practice Mission 02 - Island Sim",
        GROUND_ISLAND,
        CLOUD_NONE //
    },             //
    {
        //
        "Mission 01 - Norhtern Europe Close Support",
        GROUND_EUROPE,
        CLOUD_MOSTLY //
    },               //
    {
        //
        "Mission 02 - ECHO 7 Strike",
        GROUND_ECHO7,
        CLOUD_COMPLETE_COVER //
    },                       //
    {
        //
        "Mission 03 - OSCAR 2 Strike",
        GROUND_OSCAR2,
        CLOUD_TINY_WINDOWS //
    }                      //

};

static Texture2D textures[5];

static int contentLoaded = 0;
static int isCutsceneActive = 0;

void InitBattle()
{
  isCutsceneActive = 0;

  if (contentLoaded == 0)
  {
    // load everything maybe?
    printf("Load all battle asets\n");
    dpad = LoadTexture("resources/common-ui-dpad.png");
    crater = LoadTexture("resources/crater.png");
    UiPanelColor = (Color){25, 25, 25, 255};
    UiDarkRedColor = (Color){128, 0, 0, 255};
    UiDarkerRedColor = (Color){64, 0, 0, 255};

    TargeterRedColor = RED;
    TargeterRedTransparent = ColorAlpha(RED, 0.5f);

    TargeterGreenColor = (Color){100, 255, 100, 255};
    TargeterGreenTransparent = ColorAlpha(TargeterGreenColor, 0.5f);

    TargeterYellowColor = YELLOW;
    TargeterYellowTransparent = ColorAlpha(YELLOW, 0.5f);

    CloudWhite = ColorAlpha(WHITE, 0.8f);

    textures[GROUND_ISLAND] = LoadTexture("resources/nantucket.png");
    textures[GROUND_EUROPE] = LoadTexture("resources/europe.png");
    textures[GROUND_CITY] = LoadTexture("resources/city.png");
    textures[GROUND_EUROPE_DESTROYED] = LoadTexture("resources/europe-destroyed.png");
    textures[GROUND_ECHO7] = LoadTexture("resources/echo7-ground.png");

    contentLoaded = 1;
  }

  switch (CurrentScene)
  {
  case SCENE_02_BATTLE_TACCOM_PRACTICE_1:
    currentBattle = B1_PRACTICE_01;
    break;
  case SCENE_04_BATTLE_TACCOM_PRACTICE_2:
    currentBattle = B2_PRACTICE_02;
    break;
  case SCENE_06_BATTLE_TACCOM_MISSION_1:
    currentBattle = B3_MISSION_01;
    break;
  case SCENE_09_BATTLE_TACCOM_MISSION_2:
    currentBattle = B4_MISSION_02;
    break;
  case SCENE_14_BATTLE_TACCOM_MISSION_3:
    currentBattle = B5_MISSION_03;
    break;
  }

  battleTimer = 0;
  shotCounter = 0;

  for (int i = 0; i < MAX_TARGETS; i++)
  {
    targets[i].z = 0;
  }

  // mission specific init logic.
  switch (currentBattle)
  {
  case B1_PRACTICE_01:
    isUsingBattleTimer = false;
    nextScene = SCENE_03_WALKING_AFTER_PRACTICE_1;
    targeterPos.z = 30; // z is size of the targeter.
    groundSpeed = 1.0f;
    blastRadius = 30.0f;
    cloudSpeed = 1.0f;
    cloudIntensity = 1;
    currentGround = GROUND_ISLAND;
    worldZoom = 1.0f;
    controlsActive = 1;

    targets[0].x = -70;
    targets[0].y = 30;
    targets[0].z = 30;
    targets[0].w = TARGET_ENEMY;

    targets[1].x = -40;
    targets[1].y = 50;
    targets[1].z = 25;
    targets[1].w = TARGET_ENEMY;

    targets[2].x = 0;
    targets[2].y = 40;
    targets[2].z = 10;
    targets[2].w = TARGET_FRIENDLY;

    AddDialogQuick("YELLOW targets are enemy. GREEN are\nfriendly. In this sim the ship is barely\nmoving. This is easy mode.", NPC_COMM_SMITH, 1);
    break;
  case B2_PRACTICE_02:
    isUsingBattleTimer = false;
    nextScene = SCENE_05_WALKING_AFTER_PRACTICE_2;
    targeterPos.z = 40;
    groundSpeed = 5.0f;
    blastRadius = 30.0f;
    cloudSpeed = 1.0f;
    cloudIntensity = 2;
    currentGround = GROUND_EUROPE;
    worldZoom = 1.0f;
    setFlagCooldown(GF_BATTLE_INTRO_OVER, 3.0f);
    AddDialogQuick("We will be operating in Europe today.\nThe terrain will move by faster.\nGet your shots off quick but\nDON'T hit non-targets!\n", NPC_COMM_SMITH, 1);
    for (int i = 0; i < 5; i++)
    {
      targets[i].x = GetRandomValue(-150, 0);
      targets[i].y = GetRandomValue(-50, 50);
      targets[i].z = GetRandomValue(15, 50);
      targets[i].w = TARGET_ENEMY;
    }
    for (int i = 5; i < MAX_BULLETS; i++)
    {
      targets[i].x = GetRandomValue(10, 150);
      targets[i].y = GetRandomValue(-50, 50);
      targets[i].z = GetRandomValue(15, 50);
      targets[i].w = TARGET_FRIENDLY;
    }
    controlsActive = 0;
    break;
  case B3_MISSION_01:
    targeterPos.z = 40;
    groundSpeed = 5.0f;
    blastRadius = 30.0f;
    cloudSpeed = 5.0f;
    cloudIntensity = 3;
    currentGround = GROUND_EUROPE;
    worldZoom = 1.0f;
    AddDialogQuick("We've been called to actual battle.\nThere's a skirmish in Europe and you\nare going to stop it.", NPC_COMM_SMITH, 1);
    AddDialogQuick("I haven't heard anything about a\nconflict in Europe...", NPC_SELF, 0);
    AddDialogQuick("Me neither. But we both have to\nfollow our orders.\nGet ready...", NPC_COMM_SMITH, 1);
    setFlagCooldown(GF_REACT_TO_GROUND_TARGET, 3.0f);
    targets[0].x = -50;
    targets[0].y = 0;
    targets[0].z = 40;
    targets[0].w = TARGET_ENEMY;
    nextScene = SCENE_07_WALKING_AFTER_BATTLE_1;
    controlsActive = 1;
    break;
  case B4_MISSION_02:
    nextScene = SCENE_10_WALKING_AFTER_BATTLE_2;
    targeterPos.z = 40;
    groundSpeed = 1.0f;
    cloudSpeed = 10.0f;
    cloudIntensity = 5;
    currentGround = GROUND_ECHO7;
    worldZoom = 1.0f;
    targets[0].x = -115;
    targets[0].y = -25;
    targets[0].z = 50;
    targets[0].w = TARGET_ENEMY;
    controlsActive = 1;
    setFlagCooldown(GF_REACT_TO_GROUND_TARGET, 5.0f);
    break;
  case B5_MISSION_03:
    nextScene = SCENE_15_WALKING_AFTER_BATTLE_3;
    break;
  default:
    nextScene = -1;
    break;
  }

  firstFire = 0;
  targeterPos.x = 0;
  targeterPos.y = 0;

  if (currentGround > GROUND_NULL)
  {
    worldImageOffset.x = screenWidth / -2 - (textures[currentGround].width - screenWidth); // screenWidth - textures[currentGround].width * worldZoom;
    worldImageOffset.y = textures[currentGround].height / -2;
  }
  else
  {
    worldImageOffset.x = 0;
    worldImageOffset.y = 0;
  }

  // TODO - MAKE THE CLOUDS CHANGE BY MAP
  for (int i = 0; i < (int)(cloudIntensity / 5.0f * MAX_CLOUDS); i++)
  {
    int min = 5;
    int max = 10;

    int rand = GetRandomValue(0, 10);

    if (rand < 2)
    {
      min = 80;
      max = 100;
    }
    else if (rand < 5)
    {
      min = 50;
      max = 60;
    }

    clouds[i].x = GetRandomValue(0, screenWidth);
    clouds[i].y = GetRandomValue(0, screenHeight - 178);
    clouds[i].z = GetRandomValue(min, max);
  }

  // clear the bullet list
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    bullets[i].isActive = 0;
    bullets[i].isCrater = 0;
  }

  updateOffset();

  printf("Init battle %i\n", currentBattle);
  printf("Current ground is %i\n", currentGround);
}

int updateBattleEndConditions(float delta)
{
  if (isUsingBattleTimer)
  {
    battleTimer -= delta;
  }

  if (isUsingBattleTimer && battleTimer < 0 && battleTimer > -10.0f)
  {
    AddDialogQuick("You ran out of time!", NPC_NARRATOR, 0);
    battleTimer = -11.0f;
    setFlagCooldown(GF_SCENE_OVER, 5);
  }

  if (shotCounter == MAX_BULLETS && battleTimer > -10.0f)
  {
    int foundActiveBullet = 0;
    for (int i = 0; i < MAX_BULLETS; i++)
    {
      if (bullets[i].explosionCountdown > 0 || bullets[i].z > 0)
      {
        foundActiveBullet = 1;
        break;
      }
    }
    if (foundActiveBullet == 0)
    {
      if (currentBattle == B4_MISSION_02)
      {
        AddDialogQuick("That was a letdown", NPC_INTEL_TARI, 1);
      }
      else
      {
        AddDialogQuick("You ran out of ammo!", NPC_NARRATOR, 0);
      }
      setFlagCooldown(GF_SCENE_OVER, 5);
      battleTimer = -11.0f;
    }
  }

  int foundActive = 0;
  for (int i = 0; i < MAX_TARGETS; i++)
  {
    if (targets[i].w == TARGET_ENEMY && targets[i].z > 0)
    {
      foundActive = 1;
    }
  }

  if (!foundActive && battleTimer > -10.0f)
  {
    if (friendliesHit > 0)
    {
      AddDialogQuick("Next time, just hit the enemies...", NPC_COMM_SMITH, 1);
    }
    else
    {
      if (currentBattle == B4_MISSION_02)
      {
        AddDialogQuick("Mission accomplished.", NPC_INTEL_TARI, 1);
      }
      else
      {
        AddDialogQuick("Good job! You got close to\nfriendlies without hitting any!", NPC_COMM_SMITH, 1);
      }
    }
    if (currentBattle == B2_PRACTICE_02)
    {
      controlsActive = 0;
      currentGround = GROUND_EUROPE;
      targets[0].z = 30.0f;
      isCutsceneActive = 1;
      setFlagCooldown(GF_SHOW_CUTSCENE, 2.0f);
    }
    else
    {
      setFlagCooldown(GF_SCENE_OVER, 5);
      battleTimer = -11.0f;
    }
  }

  if (getFlag(GF_SCENE_OVER) == 1)
  {
    return 1;
  }

  return 0;
}

void updateOffset()
{
  offset.x = screenWidth / 2.0f;
  offset.y = (screenHeight - 178) / 2.0f + 50;
}

void updateTargeter(float delta)
{
  // move the test targeter.
  for (int i = 0; i < MAX_TARGETS; i++)
  {
    if (targets[i].z > 0)
    {
      targets[i].x += groundSpeed * delta;
      if (targets[i].x > screenWidth + targets[i].z)
        targets[i].z = 0;
    }
  }

  if (IsKeyDown(KEY_W) || isUpTouched())
  {
    targeterPos.y -= targeterSpeed * delta;
  }
  if (IsKeyDown(KEY_S) || isDownTouched())
  {
    targeterPos.y += targeterSpeed * delta;
  }
  if (IsKeyDown(KEY_A) || isLeftTouched())
  {
    targeterPos.x -= targeterSpeed * delta;
  }
  if (IsKeyDown(KEY_D) || isRightTouched())
  {
    targeterPos.x += targeterSpeed * delta;
  }
}

void updateFlicker(float delta)
{
  flickerTimer -= delta;
  if (flickerTimer < 0)
  {
    flickerTimer = FLICKER_TIMEOUT;
    if (flickerOff == 1)
      flickerOff = 0;
    else
      flickerOff = 1;
  }
}

void updateClouds(float delta)
{
  for (int i = 0; i < MAX_CLOUDS; i++)
  {
    if (clouds[i].z > 0)
    {
      clouds[i].x += clouds[i].z * groundSpeed * delta;
      if (clouds[i].x > screenWidth + 100)
      {
        clouds[i].x = -100;
        clouds[i].y = GetRandomValue(0, screenHeight - 128);
      }
    }
  }
}

void updateBullets(float delta)
{
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].explosionCountdown > 0)
    {
      bullets[i].explosionCountdown -= delta;
      if (bullets[i].explosionCountdown < 0)
      {
        bullets[i].explosionCountdown = 0;
      }
    }

    if (bullets[i].z > 0)
    {
      bullets[i].x += groundSpeed * delta;
      bullets[i].z -= delta;
      if (bullets[i].z < 0 && bullets[i].isCrater == 0)
      {
        bullets[i].explosionCountdown = 3.0f;
        bullets[i].z = 0;
        bullets[i].isCrater = 1;
        for (int j = 0; j < MAX_TARGETS; j++)
        {
          if (targets[j].z > 0)
          {
            if (CheckCollisionCircleRec((Vector2){bullets[i].x, bullets[i].y}, blastRadius, (Rectangle){targets[j].x, targets[j].y, targets[j].z, targets[j].z}))
            {
              if (targets[j].w == TARGET_ENEMY)
              {
                enemiesHit++;
                GlobalScore += 100;
                targets[j].z = 0;
              }
              else
              {
                friendliesHit++;
                targets[j].z = 0;
              }
            }
          }
        }
      }
    }
  }
}

int UpdateBattle(float delta)
{
  //if (IsKeyPressed(KEY_Z))
  //{
  //  for (int i = 0; i < MAX_TARGETS; i++)
  //  {
  //    if (targets[i].w == TARGET_ENEMY)
  //    {
  //      targets[i].z = 0.0f;
  //      enemiesHit++;
  //    }
  //  }
  //}

  if (updateBattleEndConditions(delta) > 0)
  {
    return nextScene;
  }

  if (controlsActive == 1)
  {
    updateOffset();

    if (controlsActive == 1)
    {
      updateTargeter(delta);
      updateFlicker(delta);
    }
    updateClouds(delta);

    worldImageOffset.x += groundSpeed * delta;

    if (IsKeyPressed(KEY_ENTER) || isFirePressed())
    {
      if (currentBattle == B1_PRACTICE_01)
      {
        if (firstFire == 0)
        {
          firstFire = 1;
          setFlagCooldown(GF_FIRST_FIRE, 2.0f);
        }
      }
      Fire();
    }

    if (currentBattle == B1_PRACTICE_01)
    {
      int flagFirstFire = getFlag(GF_FIRST_FIRE);
      if (flagFirstFire)
      {
        AddDialogQuick("Holy shit, that's quite a kick!", NPC_SELF, PORTRAIT_NOT_ON_RIGHT);
        setFlag(GF_FIRST_FIRE, 0);
      }
    }

    updateBullets(delta);
  }

  if (currentBattle == B2_PRACTICE_02)
  {
    if (isCutsceneActive == 0)
    {
      if (getFlag(GF_BATTLE_INTRO_OVER) == 1 && controlsActive == 0)
      {
        AddDialogQuick("Zooming in to target city now, get ready!", NPC_COMM_SMITH, 1);
        controlsActive = 1;
        currentGround = GROUND_CITY;
        updateOffset();
      }
    }
    else if (isCutsceneActive == 1 && getFlag(GF_SHOW_CUTSCENE) == 1)
    {
      setFlag(GF_SHOW_CUTSCENE, 0);
      AddDialogQuick("WARNING\nWARNING\nWARNING\nSTRATEGIC COMMAND SIM ACTIVE", NPC_NARRATOR, 0);
      AddDialogQuick("What the...", NPC_COMM_SMITH, 1);
      setFlagCooldown(GF_SHOW_EXPLOSION, 4.0f);
    }
    else if (isCutsceneActive == 1 && getFlag(GF_SHOW_EXPLOSION) == 1)
    {
      whiteoutLevel = 1.0f;
      currentGround = GROUND_EUROPE_DESTROYED;
      setFlag(GF_SHOW_EXPLOSION, 0);
      setFlagCooldown(GF_SCENE_OVER, 10.0f);
    }
  }

  if (currentBattle == B3_MISSION_01)
  {
    if (getFlag(GF_REACT_TO_GROUND_TARGET) == 1)
    {
      setFlag(GF_REACT_TO_GROUND_TARGET, 0);
      AddDialogQuick("What is this? There's only 1 target?", NPC_SELF, 0);
      AddDialogQuick("Hold for orders, something is wrong...", NPC_COMM_SMITH, 1);
      currentGround = GROUND_EUROPE;
      updateOffset();
      setFlagCooldown(GF_SHOW_CUTSCENE, 5.0f);
    }

    if (getFlag(GF_SHOW_CUTSCENE) == 1)
    {
      setFlag(GF_SHOW_CUTSCENE, 0);
      Shake(100);
      setFlagCooldown(GF_SHOW_EXPLOSION, 5.0f);
    }

    if (getFlag(GF_SHOW_EXPLOSION) == 1)
    {
      setFlag(GF_SHOW_EXPLOSION, 0);
      whiteoutLevel = 1.0f;
      currentGround = GROUND_EUROPE_DESTROYED;
      updateOffset();
      setFlagCooldown(GF_SCENE_OVER, 10.0f);
    }
  }

  if (currentBattle == B4_MISSION_02)
  {
    if (getFlag(GF_REACT_TO_GROUND_TARGET) == 1 && getFlag(GF_BATTLE_INTRO_OVER) == 0)
    {
      AddDialogQuick("Is that a farm? Is that what we came\nto bomb?", NPC_SELF, 0);
      AddDialogQuick("No chatter. Finish your mission.", NPC_INTEL_TARI, 1);
      setFlag(GF_BATTLE_INTRO_OVER, 1);
    }
  }

  if (whiteoutLevel > 0.0f)
  {
    whiteoutLevel -= delta * 0.3;
    if (whiteoutLevel < 0.0f)
      whiteoutLevel = 0.0f;
  }

  return -1;
}

void Fire()
{
  // no firing on b3.
  if (currentBattle == B3_MISSION_01)
    return;

  if (currentBattle == B4_MISSION_02 && getFlag(GF_BATTLE_INTRO_OVER) == 0)
    return;

  int bulletFound = 0;
  for (int i = 0; i < 10; i++)
  {
    if (bullets[i].isActive == 0)
    {
      printf("Activating bullet %i\n", i);
      bullets[i].x = targeterPos.x;
      bullets[i].y = targeterPos.y;
      bullets[i].z = altitude;
      bullets[i].isActive = 1;
      bulletFound = 1;
      break;
    }
  }

  if (bulletFound == 1)
  {
    Shake(100);
    shotCounter++;
  }
}

void DrawBullets()
{
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].isActive == 1)
    {
      if (bullets[i].isCrater == 1)
      {
        //DrawTexture(crater, bullets[i].x + offset.x - crater.width / 2, bullets[i].y + offset.y - crater.height / 2, WHITE);
        DrawTexturePro(crater, (Rectangle){0, 0, crater.width, crater.height}, (Rectangle){bullets[i].x + offset.x - blastRadius, bullets[i].y + offset.y - blastRadius, blastRadius * 2, blastRadius * 2}, (Vector2){0, 0}, 0, WHITE);
      }
    }
  }

  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].isActive == 1)
    {
      if (bullets[i].explosionCountdown > 0)
      {
        DrawCircle(bullets[i].x + offset.x, bullets[i].y + offset.y, bullets[i].explosionCountdown * bullets[i].explosionCountdown * bullets[i].explosionCountdown, YELLOW);
      }
    }
  }
}

void DrawBattle()
{
  DrawGround();
  if (controlsActive == 1)
  {
    DrawBullets();
    DrawClouds();
    DrawTargets();
    DrawTargeter();
  }
  DrawUI();
  DrawText(TextFormat("%04i", GlobalScore), screenWidth - MeasureText("0000", 40) - 5, 5, 40, GREEN);
  if (whiteoutLevel > 0.0f)
  {
    Color whiteout = ColorAlpha(WHITE, whiteoutLevel);
    DrawRectangle(0, 0, screenWidth, screenHeight, whiteout);
  }
}

void DrawUI()
{
  // bottom bar
  DrawRectangle(-100, screenHeight - 128, screenWidth + 200, 128, UiPanelColor);
  DrawRectangle(-100, screenHeight - 132, screenWidth + 200, 4, DARKGRAY);

  UIDrawInsetRectangle(64 - dpad.width / 2.0f - 4, screenHeight - 64 - dpad.height / 2.0f - 4, dpad.width + 8, dpad.height + 8, UiPanelColor, UiDarkerRedColor, UiDarkRedColor);
  DrawTexture(dpad, 64 - dpad.width / 2.0f, screenHeight - 64 - dpad.height / 2.0f, WHITE);

  UIDrawInsetRectangle(434, 335, 65, 98, RED, transparentWhite, transparentBlack);
  DrawCircleGradient(434 + 65 / 2, 335 + 98 / 2, 65 / 2 - 2, YELLOW, RED);

  // top bar
  DrawRectangle(-100, 0, screenWidth + 200, 50, UiPanelColor);
  DrawRectangle(-100, 50, screenWidth + 200, 4, DARKGRAY);
}

void DrawMarker(Vector2 pos, int targetType, int size)
{
  if (flickerOff == 1)
    return;

  Color light;
  Color dark;

  switch (targetType)
  {
  case TARGET_FACTORY:
  case TARGET_ENEMY:
    light = TargeterYellowTransparent;
    dark = TargeterYellowColor;
    break;
  case TARGET_FRIENDLY:
  case TARGET_SCHOOL:
    light = TargeterGreenTransparent;
    dark = TargeterGreenColor;
    break;
  }

  DrawRectangleLines(pos.x - (size / 2) + offset.x, pos.y - (size / 2) + offset.y, size, size, dark);
  DrawPixel(pos.x + offset.x, pos.y + offset.y, dark);
}

void DrawTargeter()
{
  Color red;

  if (flickerOff == 1)
    red = TargeterRedColor;
  else
    red = TargeterRedTransparent;

  for (int i = 0; i < 2; i++)
  {
    DrawCircleLines(targeterPos.x + offset.x, targeterPos.y + offset.y, targeterPos.z / 2 + i * 2, red);
  }

  DrawLine(targeterPos.x + offset.x, targeterPos.y - (targeterPos.z / 1.5) + offset.y, targeterPos.x + offset.x, targeterPos.y + (targeterPos.z / 1.5) + offset.y, red);
  DrawLine(targeterPos.x - (targeterPos.z / 1.5) + offset.x, targeterPos.y + offset.y, targeterPos.x + (targeterPos.z / 1.5) + offset.x, targeterPos.y + offset.y, red);
}

void DrawClouds()
{
  for (int i = 0; i < MAX_CLOUDS; i++)
  {
    if (clouds[i].z > 0)
    {
      DrawCircle(clouds[i].x, clouds[i].y, clouds[i].z, CloudWhite);
    }
  }
}

void DrawGround()
{
  DrawTexturePro(textures[currentGround], (Rectangle){0, 0, textures[currentGround].width, textures[currentGround].height}, (Rectangle){worldImageOffset.x + offset.x, worldImageOffset.y + offset.y, textures[currentGround].width * worldZoom, textures[currentGround].height * worldZoom}, (Vector2){0, 0}, 0.0f, WHITE);
}

void DrawTargets()
{
  for (int i = 0; i < MAX_TARGETS; i++)
  {
    if (targets[i].z > 0)
    {
      DrawMarker((Vector2){targets[i].x, targets[i].y}, (int)targets[i].w, (int)targets[i].z);
    }
  }
}