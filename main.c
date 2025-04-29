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

  // Generator
} GameData;

typedef struct {
  char name[30];
  double capacity;
  double health;
  double price;
  double charge;
  double maxChargePerSecond;
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
  Battery bat = {0};
  Generator gen = {0};
  Stuff stuff = {0};

  // Panel Variables
  const int PanelHeight = 35;
  const int headerFontSize = 20;

  // Game Variables;
  bool isHovering = false;
  bool isClicked = false;

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

    // Drawing Money
    char moneyText[30];
    snprintf(moneyText, sizeof(moneyText), "$ %0.2f", data.money);
    DrawLine(screenWidth / 5, (screenHeight - PanelHeight) + PanelHeight * 0.1,
             screenWidth / 5, (screenHeight - PanelHeight) + PanelHeight * 0.9,
             linesColor);
    DrawLine(screenWidth / 5 * 2,
             (screenHeight - PanelHeight) + PanelHeight * 0.1,
             screenWidth / 5 * 2,
             (screenHeight - PanelHeight) + PanelHeight * 0.9, linesColor);
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
        

      // SECOND COLUMN

#pragma region Sunlight Function
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
    gen->whPerClick = 1.0;
  }
}
void setBattery(Battery *bat, int selected) {
  if (selected == 1) { // Basic Battery from Start
    strcpy(bat->name, "Lithium-Ion Battery");
    bat->capacity = 1000.0;
    bat->maxChargePerSecond = 1.0;
  }
}
void setStuff(Stuff *stuff, int selected) {}