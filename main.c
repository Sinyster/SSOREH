#include "raylib.h"
#include <stdio.h>
#include <string.h>

typedef struct {
  double Money;

  int UnlockedLevelBattery;
  int UnlockedLevelGenerator;
  // int UnlockedLevelStuff;
  int UnlockedStageBattery;
  int UnlockedStageGenerator;
  // int UnlockedStageStuff;
} GameData;

typedef struct {
  // Namings
  char Name[64];
  char Next[64];

  // Variables
  double Capacity;
  double Charge;
  double ChargeInPercent;
  double Price;
  double NextPrice;
  double MaxInput;
} Battery;

typedef struct {
  // Namings
  char Name[64];
  char Next[64];

  // Variables
  double GeneratesPerClick;
  double GeneratesPerSecond;
  double Efficiency;
} Generator;

typedef struct {
  // Namings
  char Name[64];
  char Next[64];

  // Variables
  double DrainSecond;
  double GeneratesSecond;
} Stuff;

void setBattery(Battery *bat, int unlockedLvl, int stage);
void setGenerator(Generator *gen, int unlockedLvl, int stage);
void setStuff(Stuff *stuff, int unlockedLvl, int stage);

int main(void) {

  // Setting up Window
  const float screenWidth = 1280.0f;
  const float screenHeight = 720.0f;
  char windowTitle[] = "SS-OREH";
  int targetFps = 60;

  InitWindow(screenWidth, screenHeight, windowTitle);
  SetTargetFPS(targetFps);
  // SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  // SetExitKey(KEY_NULL);

  // Day/Night Variables
  const int dayMinutes = 10;
  const int nightMinutes = 5;
  float sunlight = 0.0f;
  bool isDay = true;

  // Setting Screens
  typedef enum {
    SCREEN_MAIN,
    SCREEN_UPGRADES,
    SCREEN_STATISTICS,
    SCREEN_SETTINGS,
    SCREEN_MENU
  } GameScreen;
  GameScreen currentScreen = SCREEN_MAIN;

  // Defaults
  GameData data = {0};
  Battery bat = {0};
  Generator gen = {0};

  // Timer Variables
  float dayTimer = 0.0f;

  // Colors
  Color bg1 = WHITE;
  Color bg2 = LIGHTGRAY;
  Color LinesColor = GRAY;
  Color HoverOver = GRAY;

  Color HeaderColor = DARKGRAY;
  Color TextColor = GRAY;

  // Panel Variables
  const float PanelHeight = 40.0f;
  // Button Variables
  const float ButtonWidth = 250.0f;
  const float ButtonHeight = 75.0f;
  // Font Variables
  const int HeaderFont = 30;
  const int textFont = 22;

  // Functional Variables
  bool isHovering = false;
  bool isClicked = false;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(bg1);

    Vector2 mousePoint = GetMousePosition();

    setBattery(&bat, data.UnlockedLevelBattery, data.UnlockedStageBattery);
    setGenerator(&gen, data.UnlockedLevelGenerator,
                 data.UnlockedStageGenerator);

#pragma region Upper Panel
    DrawRectangle(0, 0, screenWidth, PanelHeight, bg2);
    DrawLine(screenWidth / 5, PanelHeight * 0.1, screenWidth / 5,
             PanelHeight * 0.9, LinesColor);
    DrawLine(screenWidth / 5 * 2, PanelHeight * 0.1, screenWidth / 5 * 2,
             PanelHeight * 0.9, LinesColor);
    DrawLine(screenWidth / 5 * 3, PanelHeight * 0.1, screenWidth / 5 * 3,
             PanelHeight * 0.9, LinesColor);
    DrawLine(screenWidth / 5 * 4, PanelHeight * 0.1, screenWidth / 5 * 4,
             PanelHeight * 0.9, LinesColor);

    // Game Button
    Rectangle gameButton = {5, PanelHeight * 0.1, screenWidth / 5 - 10,
                            PanelHeight * 0.8};
    isHovering = CheckCollisionPointRec(mousePoint, gameButton);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    DrawRectangleRec(gameButton, isHovering ? HoverOver : bg2);

    DrawText("Game", screenWidth / 5 / 2 - MeasureText("Game", HeaderFont) / 2,
             PanelHeight / 2 - HeaderFont / 2, HeaderFont, HeaderColor);

    if (isClicked && currentScreen != SCREEN_MAIN) {
      currentScreen = SCREEN_MAIN;
    }

    // Upgrades Button
    Rectangle upgButton = {screenWidth / 5 + 5, PanelHeight * 0.1,
                           screenWidth / 5 - 10, PanelHeight * 0.8};
    isHovering = CheckCollisionPointRec(mousePoint, upgButton);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    DrawRectangleRec(upgButton, isHovering ? HoverOver : bg2);

    DrawText("Upgrades",
             screenWidth / 5 * 2 - screenWidth / 5 / 2 -
                 MeasureText("Upgrades", HeaderFont) / 2,
             PanelHeight / 2 - HeaderFont / 2, HeaderFont, HeaderColor);

    if (isClicked && currentScreen != SCREEN_UPGRADES) {
      currentScreen = SCREEN_UPGRADES;
    }

    // Stats Button
    Rectangle statButton = {screenWidth / 5 * 2 + 5, PanelHeight * 0.1,
                            screenWidth / 5 - 10, PanelHeight * 0.8};
    isHovering = CheckCollisionPointRec(mousePoint, statButton);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    DrawRectangleRec(statButton, isHovering ? HoverOver : bg2);

    DrawText("Statistics",
             screenWidth / 5 * 3 - screenWidth / 5 / 2 -
                 MeasureText("Statistics", HeaderFont) / 2,
             PanelHeight / 2 - HeaderFont / 2, HeaderFont, HeaderColor);

    if (isClicked && currentScreen != SCREEN_STATISTICS) {
      currentScreen = SCREEN_STATISTICS;
    }

    // Setting Button
    Rectangle settingButton = {screenWidth / 5 * 3 + 5, PanelHeight * 0.1,
                               screenWidth / 5 - 10, PanelHeight * 0.8};
    isHovering = CheckCollisionPointRec(mousePoint, settingButton);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    DrawRectangleRec(settingButton, isHovering ? HoverOver : bg2);

    DrawText("Settings",
             screenWidth / 5 * 4 - screenWidth / 5 / 2 -
                 MeasureText("Settings", HeaderFont) / 2,
             PanelHeight / 2 - HeaderFont / 2, HeaderFont, HeaderColor);

    if (isClicked && currentScreen != SCREEN_SETTINGS) {
      currentScreen = SCREEN_SETTINGS;
    }

    // MENU

    Rectangle menuButton = {screenWidth - screenWidth / 5 + 5,
                            PanelHeight * 0.1, screenWidth / 5 - 10,
                            PanelHeight * 0.8};
    isHovering = CheckCollisionPointRec(mousePoint, menuButton);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    DrawRectangleRec(menuButton, isHovering ? HoverOver : bg2);

    DrawText("Menu",
             screenWidth - screenWidth / 5 / 2 -
                 MeasureText("Menu", HeaderFont) / 2,
             PanelHeight / 2 - HeaderFont / 2, HeaderFont, HeaderColor);

    if (isClicked && currentScreen != SCREEN_MENU) {
      currentScreen = SCREEN_MENU;
    }
#pragma endregion

#pragma region Lower Panel
    // Basic Drawing
    DrawRectangle(0, screenHeight - PanelHeight, screenWidth, PanelHeight, bg2);
    DrawLine(screenWidth / 5, screenHeight - PanelHeight + PanelHeight * 0.1,
             screenWidth / 5, screenHeight - PanelHeight + PanelHeight * 0.9,
             LinesColor);
    DrawLine(screenWidth / 5 * 2,
             screenHeight - PanelHeight + PanelHeight * 0.1,
             screenWidth / 5 * 2,
             screenHeight - PanelHeight + PanelHeight * 0.9, LinesColor);
    DrawLine(screenWidth / 5 * 3,
             screenHeight - PanelHeight + PanelHeight * 0.1,
             screenWidth / 5 * 3,
             screenHeight - PanelHeight + PanelHeight * 0.9, LinesColor);
    DrawLine(screenWidth / 5 * 4,
             screenHeight - PanelHeight + PanelHeight * 0.1,
             screenWidth / 5 * 4,
             screenHeight - PanelHeight + PanelHeight * 0.9, LinesColor);

    // Drawing Money
    char MoneyText[30];
    snprintf(MoneyText, sizeof(MoneyText), "$ %0.1f", data.Money);
    DrawText(MoneyText,
             screenWidth / 2 - MeasureText(MoneyText, HeaderFont) / 2,
             screenHeight - PanelHeight + PanelHeight / 2 - HeaderFont / 2,
             HeaderFont, HeaderColor);

    // Drawing Charge of Battery in %
    char ChargeInPercentText[30];
    snprintf(ChargeInPercentText, sizeof(ChargeInPercentText),
             "Battery %: %0.1f", bat.ChargeInPercent);
    DrawText(
        ChargeInPercentText,
        screenWidth / 5 / 2 - MeasureText(ChargeInPercentText, textFont) / 2,
        screenHeight - PanelHeight / 2 - textFont / 2, textFont, HeaderColor);
#pragma endregion

#pragma region SCREEN_MAIN
    if (currentScreen == SCREEN_MAIN) {
      // Editing Upper Panel
      DrawText("*Game*",
               screenWidth / 5 / 2 - MeasureText("*Game*", HeaderFont) / 2,
               PanelHeight / 2 - HeaderFont / 2, HeaderFont, HeaderColor);

      // Dividing screen
      DrawLine(screenWidth / 3, PanelHeight + PanelHeight * 0.1,
               screenWidth / 3, screenHeight - PanelHeight - PanelHeight * 0.2,
               LinesColor);
      DrawLine(screenWidth / 3 * 2, PanelHeight + PanelHeight * 0.1,
               screenWidth / 3 * 2,
               screenHeight - PanelHeight - PanelHeight * 0.2, LinesColor);

      // RENDERING
      // Battery
      DrawText(bat.Name, 10, PanelHeight * 2, textFont, HeaderColor);
      DrawLine(10, PanelHeight * 2 + textFont,
               10 + MeasureText(bat.Name, textFont), PanelHeight * 2 + textFont,
               HeaderColor);

      // MAX Capacity
      char CapacityText[30];
      snprintf(CapacityText, sizeof(CapacityText), "Max. Capacity: %0.1f Wh",
               bat.Capacity);
      DrawText(CapacityText, 10, PanelHeight * 3, textFont, TextColor);

      // MAX Input Per Second
      char MaxPSText[30];
      snprintf(MaxPSText, sizeof(MaxPSText), "Max. Input: %0.1f/s",
               bat.MaxInput);
      DrawText(MaxPSText, 10, PanelHeight * 3 + HeaderFont, textFont,
               TextColor);

      // Actual Charge
      char ChargeText[30];
      snprintf(ChargeText, sizeof(ChargeText), "Charge: %0.1f", bat.Charge);
      DrawText(ChargeText, 10, PanelHeight * 3 + HeaderFont * 2, textFont,
               TextColor);

      // Charge in %
      bat.ChargeInPercent = 100.0f * (bat.Charge / bat.Capacity);
      snprintf(ChargeInPercentText, sizeof(ChargeInPercentText),
               "Charge in %: %0.1f", bat.ChargeInPercent);
      DrawText(ChargeInPercentText, 10, PanelHeight * 3 + HeaderFont * 3,
               textFont, TextColor);

      // Generator

      // Generations
      // HandCrank
      if (data.UnlockedLevelGenerator == 0) {
        Rectangle click = {screenWidth / 3 + 5, PanelHeight + 5,
                           screenWidth / 3 - 10,
                           screenHeight - PanelHeight * 2 - 10};
        isHovering = CheckCollisionPointRec(mousePoint, click);
        isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        DrawRectangleRec(click, isHovering ? LIGHTGRAY : bg1);

        if (isClicked) {
          if (bat.Charge > bat.Capacity) {
            bat.Charge = bat.Capacity;
          } else {
            bat.Charge += gen.GeneratesPerClick;
          }
        }

        DrawText(gen.Name, screenWidth / 3 + 10, PanelHeight * 2, textFont,
                 HeaderColor);
        DrawLine(screenWidth / 3 + 10, PanelHeight * 2 + textFont,
                 screenWidth / 3 + 10 + MeasureText(gen.Name, textFont),
                 PanelHeight * 2 + textFont, HeaderColor);
        char WhPerClickText[30];
        snprintf(WhPerClickText, sizeof(WhPerClickText), "Wh per Click: %0.1f",
                 gen.GeneratesPerClick);
        DrawText(WhPerClickText, screenWidth / 3 + 10, PanelHeight * 3,
                 textFont, TextColor);

        DrawText("Click Anywhere!",
                 screenWidth / 2 -
                     MeasureText("Click Anywhere!", HeaderFont) / 2,
                 screenHeight - PanelHeight * 2, HeaderFont,
                 isHovering ? GRAY : LIGHTGRAY);
      }

#pragma region Sunlight
      if (isDay) {
        dayTimer += GetFrameTime();
        if (dayTimer < dayMinutes / 2) {
          sunlight += 100.0f / (dayMinutes / 2 * targetFps);
          if (sunlight > 100.0f)
            sunlight = 100.0f;
        } else if (dayTimer > dayMinutes / 2) {
          sunlight -= 100.0f / (dayMinutes / 2 * targetFps);
          if (sunlight < 0.1f) {
            sunlight = 0.1f;
            dayTimer = 0.0f;
            isDay = false;
          }
        }
      } else {
        dayTimer += GetFrameTime();
        if (dayTimer > nightMinutes) {
          isDay = true;
          dayTimer = 0.0f;
        }
      }
#pragma endregion
#pragma endregion

#pragma region SCREEN_UPGR
    } else if (currentScreen == SCREEN_UPGRADES) {
      // Editing Upper Panel
      DrawText("*Upgrades*",
               screenWidth / 5 * 2 - screenWidth / 5 / 2 -
                   MeasureText("*Upgrades*", HeaderFont) / 2,
               PanelHeight / 2 - HeaderFont / 2, HeaderFont, HeaderColor);

      // Dividing screen
      DrawLine(screenWidth / 3, PanelHeight + PanelHeight * 0.1,
               screenWidth / 3, screenHeight - PanelHeight - PanelHeight * 0.2,
               LinesColor);
      DrawLine(screenWidth / 3 * 2, PanelHeight + PanelHeight * 0.1,
               screenWidth / 3 * 2,
               screenHeight - PanelHeight - PanelHeight * 0.2, LinesColor);
#pragma endregion

#pragma region SCREEN_STATS
    } else if (currentScreen == SCREEN_STATISTICS) {
      DrawText("*Statistics*",
               screenWidth / 5 * 3 - screenWidth / 5 / 2 -
                   MeasureText("*Statistics*", HeaderFont) / 2,
               PanelHeight / 2 - HeaderFont / 2, HeaderFont, HeaderColor);
#pragma endregion

#pragma region SCREEN_STGS
    } else if (currentScreen == SCREEN_SETTINGS) {
      DrawText("*Settings*",
               screenWidth / 5 * 4 - screenWidth / 5 / 2 -
                   MeasureText("*Settings*", HeaderFont) / 2,
               PanelHeight / 2 - HeaderFont / 2, HeaderFont, HeaderColor);
#pragma endregion

#pragma region SCREEN_MENU
    } else if (currentScreen == SCREEN_MENU) {
      DrawText("*Menu*",
               screenWidth - screenWidth / 5 / 2 -
                   MeasureText("*Menu*", HeaderFont) / 2,
               PanelHeight / 2 - HeaderFont / 2, HeaderFont, HeaderColor);
    }
#pragma endregion

#pragma endregion

    EndDrawing();
  }
  CloseWindow();
  return 0;
}

// Setting Batteries
void setBattery(Battery *bat, int unlockedLvl, int stage) {
  switch (stage) {
  case 0:
    if (unlockedLvl == 0) {
      strcpy(bat->Name, "Lithium-Ion Battery");
      bat->Capacity = 1000.0;
      bat->MaxInput = 1.0;

      // Next One
      strcpy(bat->Next, "Basic Energy Upgrade");
      bat->NextPrice = 5.0;
    }
    break;
  }
}

// Setting Generators
void setGenerator(Generator *gen, int unlockedLvl, int stage) {
  switch (stage) {
  case 0:
    if (unlockedLvl == 0) {
      strcpy(gen->Name, "Hand Crank");
      gen->GeneratesPerClick = 0.5;
    }
    break;
  }
}

void setStuff(Stuff *stuff, int unlockedLvl, int stage) {}