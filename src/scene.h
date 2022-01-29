#include "raylib.h"
#include "dialog.h"
#include "props.h"

#ifndef SCENE_H
#define SCENE_H

#define DECK_PROP_MAX 20
#define SCENE_PROP_MAX 20

#define IS_ACTIVE 1
#define IS_NOT_ACTIVE 0
#define HAS_CUTSCENE 1
#define NO_CUTSCENE 0
#define MOVING_STARFIELD 1
#define NO_MOVING_STARFIELD 0
#define SLEEPING_ALLOWED 1
#define NO_SLEEPING_ALLOWED 0
#define PORTRAIT_ON_RIGHT 1
#define PORTRAIT_NOT_ON_RIGHT 0
#define NO_LABEL NULL

extern int GlobalScore;
extern int CurrentScene;

extern Rectangle tUp;
extern Rectangle tLeft;
extern Rectangle tRight;
extern Rectangle tDown;
extern Rectangle tFire;

int isLeftTouched();
int isRightTouched();
int isUpTouched();
int isDownTouched();
int isFirePressed();

typedef enum DeckIds
{
    DECK_NULL = 0,
    DECK_ENGR,
    DECK_DOCK,
    DECK_LiVE,
    DECK_TCOM,
    DECK_SCOM,
    DECK_OFCR,
    DECK_MED,
    DECK_iNTL,
    DECK_BRDG,
    DECK_TOP_8,   // All the decks except the bottom - good for down elevator placement
    DECK_BOTTOM_8 // All the decks except the top - good for up elevator placement
} DeckIds;

typedef enum EventTypes
{
    EVENT_NULL = 0,
    EVENT_ELEVATOR_UP,
    EVENT_ELEVATOR_DOWN,
    EVENT_SCENE01_BED,
    EVENT_SCENE01_TACCOM_DOOR,
    EVENT_SCENE01_BUDDY,
    EVENT_SCENE01_RUIZ,
    EVENT_SCENE01_LEE,
    EVENT_SCENE01_SMITH,
    EVENT_SCENE03_BUDDY,
    EVENT_SCENE03_LEE,
    EVENT_SCENE03_TACCOM_DOOR,
    EVENT_SCENE05_BUDDY,
    EVENT_SCENE05_LEE,
    EVENT_SCENE05_TACCOM_DOOR,
    EVENT_SCENE05_SMITH,
    EVENT_SCENE07_BUDDY,
    EVENT_SCENE07_BED,
    EVENT_SCENE07_LEE,
    EVENT_SCENE07_SMITH,
    EVENT_SCENE08_TCOM_DOOR,
    EVENT_SCENE08_SMITH,
    EVENT_SCENE08_LEE,
    EVENT_SCENE08_TARI,
    EVENT_SCENE10_SMITH,
    EVENT_SCENE10_CONTROL_PANEL
} EventTypes;

// these are textures that are loaded into the map.
typedef enum TextureIds
{
    TEX_NULL = 0,
    TEX_BED,
    TEX_PLAYER,
    TEX_DOOR,
    TEX_RAYLIB,
    TEX_ENGINE_CORE,
    TEX_BAY_DOORS,
    TEX_SHUTTLE,
    TEX_SHUTTLE_PAD,
    TEX_CONTROL_PANEL
} TextureIds;

typedef enum PlayerFacing
{
    FACING_UP = 0,
    FACING_RIGHT = 90,
    FACING_DOWN = 180,
    FACING_LEFT = 270
} PlayerFacing;

typedef enum PlayMode
{
    MODE_NULL = 0,
    MODE_WALKING,
    MODE_BATTLE
} PlayMode;

typedef enum BattleLocations
{
    LOC_NULL = 0,
    LOC_EARTH_EUROPE,
    LOC_EARTH_JUNGLE,
    LOC_ECHO7,
    LOC_OSCAR2
} BattleLocations;

typedef enum NPCs
{
    NPC_NULL = 0,
    NPC_SELF,
    NPC_NARRATOR,
    NPC_BUDDY,
    NPC_SHEEP_RUIZ,
    NPC_ENG_LEE,
    NPC_COMM_SMITH,
    NPC_CAP_LASSIC,
    NPC_INTEL_TARI,
    NPC_PRES
} NPCs;

typedef enum Touchables
{
    TOUCH_NULL = 0,
    TOUCH_BED,
    TOUCH_DOOR_BEDROOM,
    TOUCH_DOOR_TACCOM,
    TOUCH_DOOR_STRATCOM,
    TOUCH_DOOR_ELEVATOR_UP,
    TOUCH_DOOR_ELEVATOR_DOWN,
    TOUCH_CONSOLE_BRIDGE_SYSTEM,
    TOUCH_CONSOLE_BRIDGE_NAVIGATION
} Touchables;

typedef enum Scenes
{
    SCENE_01_WALKING_INTRO = 0,
    SCENE_02_BATTLE_TACCOM_PRACTICE_1,
    SCENE_03_WALKING_AFTER_PRACTICE_1,
    SCENE_04_BATTLE_TACCOM_PRACTICE_2,
    SCENE_05_WALKING_AFTER_PRACTICE_2,
    SCENE_06_BATTLE_TACCOM_MISSION_1,
    SCENE_07_WALKING_AFTER_BATTLE_1,
    SCENE_08_WALKING_AFTER_JUMP,
    SCENE_09_BATTLE_TACCOM_MISSION_2,
    SCENE_10_WALKING_AFTER_BATTLE_2,
    SCENE_11_WALKING_BRIDGE_1,
    SCENE_12_WALKING_AFTER_BRIDGE_1,
    SCENE_13_WALKING_AFTER_JUMP_2,
    SCENE_14_BATTLE_TACCOM_MISSION_3,
    SCENE_15_WALKING_AFTER_BATTLE_3,
    SCENE_16_WALKING_AFTER_CONFINEMENT,
    SCENE_17_WALKING_AFTER_JUMP_3,
    SCENE_18_WALKING_BRIDGE_MUTINY,
    SCENE_19_WALKING_OVERLOAD_THE_REACTOR,
    SCENE_20_GOD_AMONGST_MEN
} Scenes;

typedef enum SceneEvents
{
    EVT_NULL = 0,
    EVT_SLEEP, // touch bed, go to sleep, optionally go to new scene after.
    EVT_TACCOM_ENTER

} SceneEvents;

typedef struct Scene
{
    char *name;
    short hasCutScene;
    DialogTextEntry cutSceneEntries[SCENE_CUTSCENE_DIALOG_MAX];
    int playMode;
    int startingDeck;
    PropDef props[SCENE_PROP_MAX];
} Scene;

typedef struct SceneEvent
{
    int eventType;
    int touchable; // the touchable item that will trigger this event, 0 for nothing

} SceneEvent;

void UIDrawInsetRectangle(int x, int y, int width, int height, Color bgColor, Color darkAccent, Color lightAccent);

#endif // DIALOG_H