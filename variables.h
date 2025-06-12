#ifndef VAR_H
#define VAR_H

#include "raylib.h"
#include <string.h>

typedef struct {
  // Player
  double money;
  int ActiveBattery;
  int ActiveGenerator;
  int ActiveMachine;

  int voltageBat;

  // Sunlight
  float sunlight;
  float DayTimer;
} GameData;

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
  char name[32];
  double maxCapacity;
  double actualCapacity;
  double maxInput;
  double price;

  char NextName[32];
  double NextMaxCap;
  double NextMaxInput;

  float percentage;
} Battery;

typedef struct {
  char name[32];
  double genPerSec;
  double genPerClick;
  char Special[32];
  double price;

  char NextName[32];
  double NextGen;
  char NextSpecial[32];

  int numOfSolarPanels;
  int hasExtra;

  float gasoline;
  int needGas;
} Generator;

typedef struct {
  char name[32];
  double drain;
  double output;
  double price;

  char NextName[32];
  double NextDrain;
  double NextOutput;
} Machines;

typedef struct {
  GameData data;
  Battery battery;
  Generator generator;
  Machines machines;
} SaveData;

// Window Variables
const float ScreenWidth = 1280.0f;
const float ScreenHeight = 720.0f;
const int TargetFps = 60;
const char Title[8] = "SS-OREH";

// Setting Default Values
GameScreen CurrentScreen = SCREEN_PLAY;
UpgradeScreen UpgScreen = UPG_BAT;

GameData Data = {0};
Battery bat = {0};
Generator gen = {0};
Machines mac = {0};

// Panel Variables
const float PanelHeight = 40.0f;
const float Spacing = 10.0f;

// Button
Rectangle SellBtn = {ScreenWidth / 3 * 2, PanelHeight + Spacing,
                     ScreenWidth / 3,
                     ScreenHeight - PanelHeight * 2 - Spacing * 2};

// Color Palletes
// Light Mode
Color LightBackground = WHITE;
Color LightPanelBackground = LIGHTGRAY;
Color LightFontInactive = DARKGRAY;
Color LightFontActive = BLACK;
Color LightHover = GRAY;

Color activeBackground;
Color activePanelBackground;
Color activeFontInactive;
Color activeFontActive;
Color activeHover;

// Font Variables
const float FontSizeHeader = 32.0f;
const float FontSizeText = 22.5f;

// Titles for Upper Panel
const char UpperPanelTitles[][24] = {"GAME", "UPGRADES", "STATISTICS",
                                     "SETTINGS", "MENU"};
int NumOfUPT = sizeof(UpperPanelTitles) / sizeof(UpperPanelTitles[0]);

// PopUp Labels
// PopUp Labels: Upgrades
const char UpgradePopupTitles[][24] = {"Batteries", "Generators", "Machines"};
int NumOfUPUT = sizeof(UpgradePopupTitles) / sizeof(UpgradePopupTitles[0]);

// Function Variables
bool isHovering = false;
bool isClicked = false;
bool isDarkmode = false;

bool isGameButtonAlowed = false;
bool isUpgPopUpShowed = false;

bool isDay = true;

bool isGenerating = false;
bool isSelling = false;

// Game Variables
float input = 0.0f;
float priceOfAnotherPanel = 0.0f;

// Game Variables: Sunlight
const int DayMinutes = 960.0f;
const int NightMinutes = 480.0f;

// Game Variables: Input
float InputTimer = 0.0f;
float InputUsedThisSecond = 0.0f;

// Game Variables: Selling
float ActualOutput = 0.0f;

// Texts
char GeneratorInfoText[2048] = {0};

#endif