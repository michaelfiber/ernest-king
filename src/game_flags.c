#include "game_flags.h"

static int flags[GAMEFLAG_MAX] = {0};
static float cooldowns[GAMEFLAG_MAX] = {0};

int getFlag(int flag)
{
  if (flag < 0 || flag >= GAMEFLAG_MAX)
    return 0;
  return flags[flag] == 1;
}

void setFlag(int flag, int val)
{
  if (flag < 0 || flag >= GAMEFLAG_MAX)
    return;

  if (val <= 0)
    flags[flag] = 0;
  else
    flags[flag] = 1;
}

void setFlagCooldown(int flag, float cooldown)
{
  flags[flag] = 2;
  cooldowns[flag] = cooldown;
}

void updateFlags(float delta)
{
  for (int i = 0; i < GAMEFLAG_MAX; i++)
  {
    if (flags[i] == 2)
    {
      cooldowns[i] -= delta;
      if (cooldowns[i] < 0.0f)
      {
        cooldowns[i] = 0;
        flags[i] = 1;
      }
    }
  }
}

void zeroFlags()
{
  for (int i = 0; i < GAMEFLAG_MAX; i++)
  {
    flags[i] = 0;
    cooldowns[i] = 0;
  }
}