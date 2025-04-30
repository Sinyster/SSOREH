#include "raylib.h"
#include <stdio.h>
#include <string.h>

/*
  Trying to make save files as small as possible.
*/

typedef struct {
  double money;
  int timeOfDay;

  // Battery
  double batteryCharge;
  int activeBattery;

  // Generator
  int activeGenerator;

  // Stuff
  int activeStuff
} GameData;

typedef struct {
  char name[30];
  double capacity;
  double health;
  double price;
  double maxChargePerSecond;
  float percentage;
} Battery;

typedef struct {
  char name[30];
  double whPerClick;
  double whPerSecond;
} Generator;

typedef struct {
  char name[30];
  double drainPerSecond;
  double moneyPerSecond;
} Stuff;

void setGenerator(Generator *gen, int selected);
void setBattery(Battery *bat, int selected);
void setStuff(Stuff *stuff, int selected);

int main(void) {
  // App Variables
  const double screenWidth = 1280.0;
  const double screenHeight = 720.0;
  const int targetFps = 60;
  const char title[] = "SS-OREH";

  // App Init
  InitWindow(screenWidth, screenHeight, title);
  SetTargetFPS(targetFps);
  // SetExitKey(KEY_NULL);

  // Scenes
  typedef enum { SCENE_MAIN } GameScenes;
  GameScenes currentScene = SCENE_MAIN;

  // Color Pallete
  Color bgColor = WHITE;
  Color bgColor2 = LIGHTGRAY;
  Color hoverBg = GRAY;
  Color headerColor = DARKGRAY;
  Color textColor = GRAY;
  Color linesColor = GRAY;

  // DEFAULT VARIABLES
  GameData data = {0};
  data.activeBattery = 1;
  data.activeGenerator = 1;
  data.activeStuff = 1;

  Battery bat = {0};
  setBattery(&bat, data.activeBattery);

  Generator gen = {0};
  setGenerator(&gen, data.activeGenerator);

  Stuff stuff = {0};
  setStuff(&stuff, data.activeStuff);

  // Panel Variables
  const int PanelHeight = 35;
  const int headerFontSize = 22;
  const int textFontSize = 20;

  const int buttonWidth = 250;
  const int buttonHeight = 75;

  // Game Variables;
  bool isHovering = false;
  bool isClicked = false;
  bool isSelling = false;

  static float dayTimer = 0.0f;
  bool isDay = true;
  float sunlight = 0.0f;
  const int dayMinute = 900;
  const int nightMinute = 540;

  while (!WindowShouldClose()) {
    // Base Functions for All Scenes
    Vector2 mousePoint = GetMousePosition();
    BeginDrawing();
    ClearBackground(bgColor);

    if (data.batteryCharge <= 0.0) {
      data.batteryCharge = 0.0;
    }

#pragma region Upper Panel
    DrawRectangle(0, 0, screenWidth, PanelHeight, bgColor2);
    DrawLine(0, PanelHeight, screenWidth, PanelHeight, linesColor);

    // First Part
    DrawLine(screenWidth / 5, PanelHeight * 0.1, screenWidth / 5, PanelHeight,
             linesColor);
    Rectangle mainSceneButton = {3, PanelHeight * 0.1, screenWidth / 5 - 6,
                                 PanelHeight * 0.8};
    isHovering = CheckCollisionPointRec(mousePoint, mainSceneButton);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    DrawRectangleRec(mainSceneButton, isHovering ? hoverBg : bgColor2);
    DrawText("Game",
             (screenWidth / 5) / 2 - MeasureText("Game", headerFontSize) / 2,
             PanelHeight / 2 - headerFontSize / 2, headerFontSize, headerColor);

    if (currentScene != SCENE_MAIN && isClicked) {
      currentScene = SCENE_MAIN;
    }
    // Second Part
    // Third Part
    // Forth Part
    // Fifth Part

#pragma endregion
#pragma region Lower Panel
    DrawRectangle(0, screenHeight - PanelHeight, screenWidth, PanelHeight,
                  bgColor2);
    DrawLine(0, screenHeight - PanelHeight, screenWidth,
             screenHeight - PanelHeight, linesColor);

    DrawLine(screenWidth / 5, (screenHeight - PanelHeight) + PanelHeight * 0.1,
             screenWidth / 5, (screenHeight - PanelHeight) + PanelHeight * 0.9,
             linesColor);
    DrawLine(screenWidth / 5 * 2,
             (screenHeight - PanelHeight) + PanelHeight * 0.1,
             screenWidth / 5 * 2,
             (screenHeight - PanelHeight) + PanelHeight * 0.9, linesColor);
    DrawLine(screenWidth / 5 * 3,
             (screenHeight - PanelHeight) + PanelHeight * 0.1,
             screenWidth / 5 * 3,
             (screenHeight - PanelHeight) + PanelHeight * 0.9, linesColor);
    // Drawing Money
    char moneyText[30];
    snprintf(moneyText, sizeof(moneyText), "$ %0.2f", data.money);
    DrawText(moneyText,
             (screenWidth / 5) / 2 - MeasureText(moneyText, headerFontSize) / 2,
             screenHeight - PanelHeight / 2 - headerFontSize / 2,
             headerFontSize, headerColor);

    // Drawing Sunlight %
    char sunlightText[30];
    snprintf(sunlightText, sizeof(sunlightText), "Sunlight %: %0.1f", sunlight);
    DrawText(sunlightText,
             (screenWidth / 5 * 2) - (screenWidth / 5 / 2) -
                 MeasureText(sunlightText, headerFontSize) / 2,
             screenHeight - PanelHeight / 2 - headerFontSize / 2,
             headerFontSize, headerColor);

    // Battery Charge
    char percentageText[30];
    snprintf(percentageText, sizeof(percentageText), "Charge in %: %0.2f",
             bat.percentage);
    DrawText(percentageText,
             screenWidth / 2 - MeasureText(percentageText, headerFontSize) / 2,
             screenHeight - PanelHeight / 2 - headerFontSize / 2,
             headerFontSize, headerColor);
#pragma endregion

    // ===========================================================================================================

    if (currentScene == SCENE_MAIN) {
      // Editing Upper Panel
      DrawLine((screenWidth / 5) / 2 - MeasureText("Game", headerFontSize),
               PanelHeight * 0.9,
               (screenWidth / 5) / 2 + MeasureText("Game", headerFontSize),
               PanelHeight * 0.9, linesColor);

      // RENDER OF MAIN GAME PANEL

      DrawLine(screenWidth / 3, PanelHeight + PanelHeight * 0.1,
               screenWidth / 3, screenHeight - PanelHeight - PanelHeight * 0.1,
               linesColor);

      DrawLine(screenWidth / 3 * 2, PanelHeight + PanelHeight * 0.1,
               screenWidth / 3 * 2,
               screenHeight - PanelHeight - PanelHeight * 0.1, linesColor);

      // FIRST COLUMN
      // =======================================================================================================================================
      DrawText("Battery: ", PanelHeight, PanelHeight * 2, headerFontSize,
               textColor);

      DrawText(bat.name, PanelHeight + MeasureText("Battery: ", headerFontSize),
               PanelHeight * 2, headerFontSize, textColor);

      // Capacity
      char capacityText[30];
      snprintf(capacityText, sizeof(capacityText), "Max. Capacity: %0.0f Wh",
               bat.capacity);
      DrawText(capacityText, PanelHeight, PanelHeight * 3, textFontSize,
               textColor);

      // Max Charge
      char maxChargeText[30];
      snprintf(maxChargeText, sizeof(maxChargeText), "Max. Charge: %0.1f Wh/s",
               bat.maxChargePerSecond);
      DrawText(maxChargeText, PanelHeight, PanelHeight * 3 + textFontSize,
               textFontSize, textColor);

      // Actual Charge
      char chargeText[30];
      snprintf(chargeText, sizeof(chargeText), "Actual Charge: %0.1f Wh",
               data.batteryCharge);
      DrawText(chargeText, PanelHeight, PanelHeight * 4 + textFontSize,
               textFontSize, textColor);
      DrawText(percentageText, PanelHeight, PanelHeight * 4 + textFontSize * 2,
               textFontSize, textColor);

      // SECOND COLUMN
      // =======================================================================================================================================
      DrawText("Source: ", screenWidth / 3 + PanelHeight, PanelHeight * 2,
               headerFontSize, textColor);
      DrawText(gen.name,
               screenWidth / 3 + PanelHeight +
                   MeasureText("Source: ", headerFontSize),
               PanelHeight * 2, headerFontSize, textColor);
      char genPerClick[30];
      snprintf(genPerClick, sizeof(genPerClick),
               "Generates per Click: %0.1f Wh", gen.whPerClick);
      DrawText(genPerClick, screenWidth / 3 + PanelHeight, PanelHeight * 3,
               textFontSize, textColor);

      // THIRD COLUMN
      // =======================================================================================================================================
      DrawText("Powering: ", screenWidth / 3 * 2 + PanelHeight, PanelHeight * 2,
               headerFontSize, textColor);
      DrawText(stuff.name,
               screenWidth / 3 * 2 + PanelHeight +
                   MeasureText("Powering: ", headerFontSize),
               PanelHeight * 2, headerFontSize, textColor);

      char drainText[30];
      snprintf(drainText, sizeof(drainText), "Drains: %0.1f Wh/s",
               stuff.drainPerSecond);
      DrawText(drainText, screenWidth / 3 * 2 + PanelHeight, PanelHeight * 3,
               textFontSize, textColor);

      char gensText[30];
      snprintf(gensText, sizeof(gensText), "Gains: %0.1f $/s",
               stuff.moneyPerSecond);
      DrawText(gensText, screenWidth / 3 * 2 + PanelHeight,
               PanelHeight * 3 + textFontSize, textFontSize, textColor);

#pragma region Generation
      if (strcmp(gen.name, "Hand Crank") == 0) {
        Rectangle handCrank = {screenWidth / 2 - buttonWidth / 2,
                               screenHeight / 2, buttonWidth, buttonHeight};
        isHovering = CheckCollisionPointRec(mousePoint, handCrank);
        isClicked = isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        DrawRectangleRec(handCrank, isHovering ? GRAY : LIGHTGRAY);
        DrawText("Rotate the Crank",
                 screenWidth / 2 -
                     MeasureText("Rotate the Crank", headerFontSize) / 2,
                 screenHeight / 2 - headerFontSize / 2 + buttonHeight / 2,
                 headerFontSize, headerColor);
        if (isClicked) {
          data.batteryCharge += gen.whPerClick;
        }
      }

#pragma endregion

#pragma region Selling
      // Rendering button
      Rectangle sellButton = {screenWidth / 6 * 5 - buttonWidth / 2,
                              screenHeight / 2, buttonWidth, buttonHeight};
      isHovering = CheckCollisionPointRec(mousePoint, sellButton);
      isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

      DrawRectangleRec(sellButton, isHovering ? GRAY : LIGHTGRAY);

      if (isClicked) {
        isSelling = isSelling ? false : true;
      }

      // TURN ON/OFF TEXT
      if (data.batteryCharge > 0.0) {
        if (isSelling) {
          DrawText("Turn Off",
                   (screenWidth / 6 * 5) -
                       MeasureText("Turn Off", headerFontSize) / 2,
                   screenHeight / 2 + buttonHeight / 2 - headerFontSize / 2,
                   headerFontSize, headerColor);
        } else {
          DrawText("Turn On",
                   (screenWidth / 6 * 5) -
                       MeasureText("Turn On", headerFontSize) / 2,
                   screenHeight / 2 + buttonHeight / 2 - headerFontSize / 2,
                   headerFontSize, headerColor);
        }
      }

      // Sell Function
      if (isSelling) {
        if (data.batteryCharge > 0.0) {
          data.batteryCharge -= stuff.drainPerSecond / targetFps;
          data.money += stuff.moneyPerSecond / targetFps;
        } else {
          isSelling = false;
        }
      }

#pragma endregion

#pragma region Battery Calc

      // Battery Percentage
      bat.percentage = 100.0f * (data.batteryCharge / bat.capacity);

#pragma endregion

#pragma region Sunlight Func
      // Calculation
      if (isDay) {
        dayTimer += GetFrameTime();
        if (dayTimer < dayMinute / 2) {
          sunlight += 100.0f / (dayMinute / 2 * targetFps);
          if (sunlight > 100.0f)
            sunlight = 100.0f;
        } else if (dayTimer > dayMinute / 2) {
          sunlight -= 100.0f / (dayMinute / 2 * targetFps);
          if (sunlight < 0.1f) {
            sunlight = 0.1f;
            dayTimer = 0.0f;
            isDay = false;
          }
        }
      } else {
        dayTimer += GetFrameTime();
        if (dayTimer > nightMinute) {
          isDay = true;
          dayTimer = 0.0f;
        }
      }

#pragma endregion
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}

void setGenerator(Generator *gen, int selected) {
  if (selected == 1) { // Hand Crank
    strcpy(gen->name, "Hand Crank");
    gen->whPerClick = 0.5;
  }
}
void setBattery(Battery *bat, int selected) {
  if (selected == 1) { // Basic Battery from Start
    strcpy(bat->name, "Lithium-Ion Battery");
    bat->capacity = 1000.0;
    bat->maxChargePerSecond = 1.0;
  }
}
void setStuff(Stuff *stuff, int selected) {
  if (selected == 1) { // Light Bulb
    strcpy(stuff->name, "Light Bulb");
    stuff->drainPerSecond = 0.2; // Wh
    stuff->moneyPerSecond = 0.1; // Wh
  }
}