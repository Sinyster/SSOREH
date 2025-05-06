#include "raylib.h"
#include <stdio.h>

typedef struct {
  double money;
} GameData;

int main(void) {
  // Setting up Window
  const float screenWidth = 1280.0f;
  const float screenHeight = 720.0f;
  char windowTitle[] = "SS-OREH";
  int targetFps = 60;

  InitWindow(screenWidth, screenHeight, windowTitle);
  SetTargetFPS(targetFps);
  // SetExitKey(KEY_NULL);

  typedef enum { SCREEN_MAIN } GameScreen;
  GameScreen currentScreen = SCREEN_MAIN;

  // Colors
  Color bg1 = WHITE;
  Color bg2 = LIGHTGRAY;
  Color LinesColor = GRAY;
  Color HoverOver = GRAY;

  Color HeaderColor = DARKGRAY;
  Color TextColor = LIGHTGRAY;

  // Panel Variables
  const float PanelHeight = 40.0f;
  // Button Variables
  const float ButtonWidth = 250.0f;
  const float ButtonHeight = 75.0f;
  // Font Variables
  const int HeaderFont = 30;
  const int textFont = 22;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(bg1);
#pragma region SCREEN_MAIN
    if (currentScreen = SCREEN_MAIN) {
    }

    EndDrawing();
  }
  CloseWindow();
  return 0;
}