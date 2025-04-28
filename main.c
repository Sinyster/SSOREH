#include "raylib.h"
#include <stdio.h>

typedef struct {
  // Variables
  double kwh;
} GameData;

void saveGame(GameData *data);
void loadGame(GameData *data);

typedef struct {
  int hours;
  int minutes;
} TimeData;

void convertTime(TimeData *time);

int main(void) {
  // Const Variables
  const int screenWidth = 1280;
  const int screenHeight = 720;
  const int targetFps = 60;
  char title[] = "SSOREH";

  // Color Palette

  // App Variables
  InitWindow(screenWidth, screenHeight, title);
  SetTargetFPS(targetFps);

  // Screens
  typedef enum { SCREEN_MAIN, SCREEN_MAINMENU, SCREEN_SHORTCUTS } GameScreen;
  GameScreen currentScreen = SCREEN_MAIN;

  // Functional Variables
  static float dayTimer = 0.0f;
  const double dayMinute = 10;  // 15 minut den = 900sec
  const double nightMinute = 5; // 9 minut noc = 540sec
  float dayPercentage = 0.0f;
  bool isSunlight = true;

  // Default Variables
  GameData data = {0};

  // Application Run
  while (!WindowShouldClose()) {
    Color bgColor = {66, 69, 73, 255};
    Color neonGreen = {57, 255, 20, 255};

    BeginDrawing();

    if (currentScreen == SCREEN_MAIN) {
      ClearBackground(bgColor);
      DrawRectangleLines(6, 6, screenWidth - 12, screenHeight - 12, neonGreen);

#pragma region Sunlight function
      char dayTimerText[30];
      snprintf(dayTimerText, sizeof(dayTimerText), "Sunlight %: %0.1f",
               dayPercentage);
      DrawText(dayTimerText,
               (screenWidth / 6) - (MeasureText(dayTimerText, 20) / 2), 12, 20,
               neonGreen);

      if (isSunlight) {
        dayTimer += GetFrameTime();
        if (dayTimer < dayMinute / 2) {
          dayPercentage += 100.0f / (dayMinute / 2 * targetFps);
          if (dayPercentage > 100.0f)
            dayPercentage = 100.0f;
        } else if (dayTimer > dayMinute / 2) {
          dayPercentage -= 100.0f / (dayMinute / 2 * targetFps);
          if (dayPercentage < 0.1f) {
            dayPercentage = 0.1f;
            dayTimer = 0.0f;
            isSunlight = false;
          }
        }
      } else {
        dayTimer += GetFrameTime();
        if (dayTimer > nightMinute) {
          isSunlight = true;
          dayTimer = 0.0f;
        }
      }
    }
#pragma endregion

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

void convertTime(TimeData *time) {
  while (time->minutes % 60 == 0) {
    time->hours += 1;
  }
}