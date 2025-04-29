#include "raylib.h"
#include <stdio.h>

typedef struct {
  double money;
} GameData;
typedef struct {
} Battery;
typedef struct {
} Generator;

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
  Battery bat = {};
  Generator gen = {};

  // Panel Variables
  const int PanelHeight = 35;
  const int headerFontSize = 20;

  // Game Variables;
  bool isHovering = false;
  bool isClicked = false;

  while (!WindowShouldClose()) {
    // Base Functions for All Scenes
    Vector2 mousePoint = GetMousePosition();
    BeginDrawing();
    ClearBackground(bgColor);

#pragma region Upper Panel
    DrawRectangle(0, 0, screenWidth, PanelHeight, bgColor2);

    // First Part
    DrawLine(screenWidth / 5, PanelHeight * 0.1, screenWidth / 5,
             PanelHeight * 0.9, linesColor);
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
    if (currentScene == SCENE_MAIN) {
#pragma region Lower Panel
      DrawRectangle(0, screenHeight - PanelHeight, screenWidth, PanelHeight,
                    bgColor2);
      // Drawing Money
      char moneyText[30];
      snprintf(moneyText, sizeof(moneyText), "$ %0.2f", data.money);
      DrawLine(screenWidth / 5,
               (screenHeight - PanelHeight) + PanelHeight * 0.1,
               screenWidth / 5,
               (screenHeight - PanelHeight) + PanelHeight * 0.9, linesColor);
      DrawText(moneyText,
               (screenWidth / 5) / 2 -
                   MeasureText(moneyText, headerFontSize) / 2,
               screenHeight - PanelHeight / 2 - headerFontSize / 2,
               headerFontSize, headerColor);
#pragma endregion
    }

    EndDrawing();
  }
  CloseWindow();
  return 0;
}