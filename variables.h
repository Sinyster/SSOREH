#ifndef VAR_H
#define VAR_H

#include "raylib.h"

typedef enum {
  SCREEN_PLAY,
  SCREEN_UPGRADE,
  SCREEN_STATS,
  SCREEN_SETTINGS,
  SCREEN_MENU
} GameScreen;

// Window Variables
const float ScreenWidth = 1280.0f;
const float ScreenHeight = 720.0f;
const int TargetFps = 60;
const char Title[8] = "SS-OREH";

// Panel Variables
const float PanelHeight = 40.0f;
const float Spacing = 10.0f;

// Defining Colors
Color BackgroundWhite = WHITE;
Color PanelBackground = LIGHTGRAY;

// Font Variables
const float FontSizeHeader = 32.0f;
const float FontSizeText = 26.0f;

// Titles for Upper Panel
const char UpperPanelTitles[][24] = {"GAME", "UPGRADES", "STATISTICS",
                                     "SETTINGS", "MENU"};
int NumOfUPT = sizeof(UpperPanelTitles) / sizeof(UpperPanelTitles[0]);

// Pop-Up Variables
const float PopUpWidth = 0.0f;
const float PopUpHeight = 0.0f;

// Function Variables
bool isGameButtonAlowed = false;
bool isHovering = false;
bool isClicked = false;

#endif