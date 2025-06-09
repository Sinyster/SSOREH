#ifndef VAR_H
#define VAR_H

#include "raylib.h"
#include <string.h>

// Struct for Game Screens
typedef enum {
  SCREEN_PLAY,
  SCREEN_UPGRADE,
  SCREEN_STATS,
  SCREEN_SETTINGS,
  SCREEN_MENU
} GameScreen;

// Game Screens: Struct for different Upgrade Screens
typedef enum { UPG_BAT, UPG_GEN, UPG_MAC } UpgradeScreen;

// Struct for Batteries
typedef struct {
  char name[16];
  double maxCapacity;
  double actualCapacity;
  double maxOutput;
  double maxInput;

  float percentage;
} Battery;

// Window Variables
const float ScreenWidth = 1280.0f;
const float ScreenHeight = 720.0f;
const int TargetFps = 60;
const char Title[8] = "SS-OREH";

// Setting Default Values
GameScreen CurrentScreen = SCREEN_PLAY;
UpgradeScreen UpgScreen = UPG_BAT;

Battery bat = {0};

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

// PopUp Labels
// PopUp Labels: Upgrades
const char UpgradePopupTitles[][24] = {"Batteries", "Generators", "Machines"};
int NumOfUPUT = sizeof(UpgradePopupTitles) / sizeof(UpgradePopupTitles[0]);

// Function Variables
bool isGameButtonAlowed = false;
bool isHovering = false;
bool isClicked = false;
bool isUpgPopUpShowed = false;

// Game Varaibles
int activeBattery = 0;
#endif