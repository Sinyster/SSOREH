#include "raylib.h"
#include <stdio.h>

typedef struct {
  // Variables
  double kwh;
} GameData;

void saveGame(GameData *data);
void loadGame(GameData *data);

int main(void) {
  // Const Variables
  const int screenWidth = 1280;
  const int screenHeight = 720;
  const int targetFps = 60;
  char title[] = "SSOREH";

  // Color Palette
  Color bgColor = {66, 69, 73};

  // App Variables
  InitWindow(screenWidth, screenHeight, title);
  SetTargetFPS(targetFps);

  // Screens
  typedef enum { SCREEN_MAIN, SCREEN_MAINMENU, SCREEN_SHORTCUTS } GameScreen;
  GameScreen currentScreen = SCREEN_MAINMENU;

  // Default Variables
  GameData data = {0};

  // Application Run
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(bgColor);

    if (currentScreen == SCREEN_MAINMENU) {
    } else if (currentScreen == SCREEN_MAIN) {
    } else if (currentScreen == SCREEN_SHORTCUTS) {
    }

    EndDrawing();
  }
  CloseWindow();

  return 0;
}
void saveGame(GameData *data) {
  FILE *file = fopen("savedata.txt", "wb");
  if (file != NULL) {
    fwrite(data, sizeof(GameData), 1, file);
    fclose(file);
  }
}

void loadGame(GameData *data) {
  FILE *file = fopen("savedata.txt", "rb");
  if (file != NULL) {
    fread(data, sizeof(GameData), 1, file);
    fclose(file);
  }
}