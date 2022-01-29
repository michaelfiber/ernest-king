#include <string.h>
#include <stdio.h>
#include "raylib.h"
#include "screens.h"
#include "scene.h"
#include "dialog.h"

#define PORTRAIT_ON_RIGHT 1
#define PORTRAIT_NOT_ON_RIGHT 0

int IsDialogOpen = 0;
int IsDialogComplete = 0;

static int dialogJustClosed = 0;
static float dialogLetterTimeout = 0.0f;
static int currentDialogLetter = 0;
static int loaded = 0;

DialogTextEntry dialogQueue[32];
static int dialogQueueSize = 0;
Color dialogBackground = BLACK;
Texture2D textures[10] = {0};

void InitDialog()
{
    if (loaded == 0)
    {
        printf("LOADED DIALOG PORTRAITS!\n");
        textures[NPC_BUDDY] = LoadTexture("resources/buddy.png");
        textures[NPC_CAP_LASSIC] = LoadTexture("resources/lassic.png");
        textures[NPC_COMM_SMITH] = LoadTexture("resources/smith.png");
        textures[NPC_SELF] = LoadTexture("resources/self.png");
        textures[NPC_ENG_LEE] = LoadTexture("resources/lee.png");
        textures[NPC_INTEL_TARI] = LoadTexture("resources/tari.png");
        textures[NPC_PRES] = LoadTexture("resources/pres.png");
        loaded = 1;
    }
    dialogLetterTimeout = 0;
    currentDialogLetter = 0;
    IsDialogComplete = false;
    IsDialogOpen = false;
    dialogQueueSize = 0;
    dialogBackground = ColorAlpha(BLACK, 0.85f);

    for (int i = 0; i < 32; i++)
    {
        dialogQueue[i].speaker = NPC_NULL;
    }
}

void AddDialog(DialogTextEntry entry)
{
    dialogQueue[dialogQueueSize].speaker = entry.speaker;
    dialogQueue[dialogQueueSize].portraitOnRight = entry.portraitOnRight;
    dialogQueue[dialogQueueSize].text = entry.text;
    dialogQueue[dialogQueueSize].texture = entry.texture;

    if (entry.text != NULL)
    {
        dialogQueue[dialogQueueSize].contentSize = MeasureTextEx(font, entry.text, font.baseSize * DIALOG_FONT_SIZE_MULTIPLIER, 1);
        // the textsize seems to be off by ~1 character width
        dialogQueue[dialogQueueSize].contentSize.x += 28;
    }
    else if (entry.texture)
    {

        dialogQueue[dialogQueueSize].contentSize.x = entry.texture->width;
        dialogQueue[dialogQueueSize].contentSize.y = entry.texture->height;
    }

    dialogQueueSize++;

    printf("DIALOG QUEUE SIZE %i\n", dialogQueueSize);

    if (dialogQueueSize > 32)
    {
        printf("DIALOG QUEUE SIZE IS TOO BIG! %i\n", dialogQueueSize);
    }
}

void AddDialogQuick(char *text, int speaker, int portraitOnRight)
{
    DialogTextEntry dt = {0};
    dt.text = text;
    dt.speaker = speaker;
    dt.portraitOnRight = portraitOnRight;
    AddDialog(dt);
}

void UpdateDialog(float delta)
{
    if (dialogQueue[0].speaker > NPC_NULL)
    {
        IsDialogOpen = true;
        if (!IsDialogComplete)
        {
            if (dialogQueue[0].text != NULL)
            {
                if (dialogLetterTimeout <= 0)
                {
                    dialogLetterTimeout = DIALOG_LETTER_TIMEOUT;
                    currentDialogLetter++;
                }

                if (currentDialogLetter > strlen(dialogQueue[0].text))
                {
                    IsDialogComplete = true;
                    currentDialogLetter = strlen(dialogQueue[0].text);
                }
                dialogLetterTimeout -= delta;
            }
        }
    }
    else
    {
        IsDialogOpen = false;
    }
    dialogJustClosed = false;
}

void SkipDialogAnimation()
{
    if (dialogQueue[0].text != NULL)
    {
        currentDialogLetter = strlen(dialogQueue[0].text);
    }
}

void CloseCurrentDialog()
{
    if (dialogJustClosed == 1)
    {
        return;
    }

    for (int i = 0; i < dialogQueueSize - 1; i++)
    {
        dialogQueue[i].portraitOnRight = dialogQueue[i + 1].portraitOnRight;
        dialogQueue[i].speaker = dialogQueue[i + 1].speaker;
        dialogQueue[i].text = dialogQueue[i + 1].text;
        dialogQueue[i].contentSize = dialogQueue[i + 1].contentSize;
    }
    dialogQueue[dialogQueueSize - 1].speaker = NPC_NULL;
    dialogQueueSize--;

    currentDialogLetter = 0;
    dialogLetterTimeout = DIALOG_LETTER_TIMEOUT;
    IsDialogComplete = false;

    dialogJustClosed = 1;
}

void DrawDialog()
{
    if (dialogQueue[0].speaker > NPC_NULL)
    {
        char *name;
        int hasPicture = 0;

        switch (dialogQueue[0].speaker)
        {
        case NPC_SELF:
            name = "You";
            hasPicture = 1;
            break;
        case NPC_BUDDY:
            name = "Buddy";
            hasPicture = 1;
            break;
        case NPC_COMM_SMITH:
            name = "Commander Smith";
            hasPicture = 1;
            break;
        case NPC_SHEEP_RUIZ:
            name = "Ruiz";
            break;
        case NPC_ENG_LEE:
            name = "Engineer Lee";
            hasPicture = 1;
            break;
        case NPC_INTEL_TARI:
            name = "Intelligence Officer Tari";
            hasPicture = 1;
            break;
        case NPC_CAP_LASSIC:
            name = "Captain Lassic";
            hasPicture = 1;
            break;
        case NPC_PRES:
            name = "Evil President of Mega Corp";
            hasPicture = 1;
            break;
        default:
            name = "NEEDS NAME";
            break;
        }

        Vector2 nameSize = MeasureTextEx(font, name, font.baseSize * DIALOG_FONT_SIZE_MULTIPLIER, DIALOG_FONT_SPACING);

        if (dialogQueue[0].text != NULL)
        {
            DrawRectangle(20, 20, screenWidth - 40, screenHeight - 40, dialogBackground);
            DrawRectangleLines(20, 20, screenWidth - 40, screenHeight - 40, GREEN);
            DrawRectangleLines(22, 22, screenWidth - 44, screenHeight - 44, LIME);
            DrawRectangleLines(18, 18, screenWidth - 36, screenHeight - 36, LIME);

            Vector2 pos = {40, 180};

            if (dialogQueue[0].speaker != NPC_NARRATOR)
            {
                if (dialogQueue[0].portraitOnRight)
                {
                    DrawRectangleLines(screenWidth - 168, 40, 128, 128, GREEN);
                    if (hasPicture == 1)
                    {
                        DrawTextureEx(textures[dialogQueue[0].speaker], (Vector2) {screenWidth - 168, 40 }, 0.0f, 4.0f, WHITE);
                    }
                    DrawTextEx(font, name, (Vector2){screenWidth - 188 - nameSize.x, 40}, font.baseSize * DIALOG_FONT_SIZE_MULTIPLIER, DIALOG_FONT_SPACING, YELLOW);
                }
                else
                {
                    DrawRectangleLines(40, 40, 128, 128, GREEN);
                    if (hasPicture == 1)
                    {
                        DrawTextureEx(textures[dialogQueue[0].speaker], (Vector2) {40, 40 }, 0.0f, 4.0f, WHITE);
                    }
                    DrawTextEx(font, name, (Vector2){188, 40}, font.baseSize * DIALOG_FONT_SIZE_MULTIPLIER, DIALOG_FONT_SPACING, YELLOW);
                }
            }
            DrawTextEx(font, TextFormat("%.*s", currentDialogLetter, dialogQueue[0].text), pos, font.baseSize * DIALOG_FONT_SIZE_MULTIPLIER, DIALOG_FONT_SPACING, GREEN);
        }
        else if (dialogQueue[0].texture)
        {
            printf("TODO: MAKE IT SO DIALOG PICTURES SHOW\n");
        }
    }
}