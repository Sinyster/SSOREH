#include "raylib.h"

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
  Color headerColor = {};
  Color textColor = {};
  Color linesColor = {};

  while (!WindowShouldClose()) {
    // Base Functions for All Scenes
    BeginDrawing();
    ClearBackground(bgColor);

    


    EndDrawing();
  }
  CloseWindow();
  return 0;
}