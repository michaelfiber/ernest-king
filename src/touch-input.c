#include "scene.h"
#include "raylib.h"

int isLeftTouched()
{
    return IsMouseButtonDown(0) && CheckCollisionPointRec(GetMousePosition(), tLeft);
}

int isRightTouched()
{
    return IsMouseButtonDown(0) && CheckCollisionPointRec(GetMousePosition(), tRight);
}

int isUpTouched()
{
    return IsMouseButtonDown(0) && CheckCollisionPointRec(GetMousePosition(), tUp);
}

int isDownTouched()
{
    return IsMouseButtonDown(0) && CheckCollisionPointRec(GetMousePosition(), tDown);
}

int isFirePressed()
{
    return IsMouseButtonPressed(0) && CheckCollisionPointRec(GetMousePosition(), tFire);
}