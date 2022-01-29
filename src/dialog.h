#include "raylib.h"

#ifndef DIALOG_H
#define DIALOG_H

extern int IsDialogOpen;
extern int IsDialogComplete;

#define SCENE_CUTSCENE_DIALOG_MAX 32
#define DIALOG_LETTER_TIMEOUT 0.05f
#define DIALOG_FONT_SIZE_MULTIPLIER 2
#define DIALOG_FONT_SPACING 2

typedef struct DialogTextEntry
{
    char *text;
    int texture_id;
    Texture2D *texture;
    Vector2 contentSize;
    int speaker;
    int portraitOnRight;
} DialogTextEntry;

void InitDialog();
void AddDialog(DialogTextEntry entry);
void AddDialogQuick(char *text, int speaker, int portraitOnRight);
void UpdateDialog(float delta);
void SkipDialogAnimation(void);
void CloseCurrentDialog(void);
void DrawDialog(void);

#endif // DIALOG_H