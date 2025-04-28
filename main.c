#include "raylib.h"
#include <stdio.h>

typedef struct {
  // Variables
  double kwhPerSecond;
  double kwhPerClick;

  // Generator Variables
  int activeGenerator;

  // Battery Variables
  int activeBattery;
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

  // App Variables
  InitWindow(screenWidth, screenHeight, title);
  SetTargetFPS(targetFps);

  // Screens
  typedef enum { SCREEN_MAIN, SCREEN_MAINMENU, SCREEN_SHORTCUTS } GameScreen;
  GameScreen currentScreen = SCREEN_MAIN;

  // Functional Variables
  static float dayTimer = 0.0f;
  const double dayMinute = 900;   // 15 minut den = 900sec
  const double nightMinute = 540; // 9 minut noc = 540sec
  float dayPercentage = 0.0f;
  bool isSunlight = true;

  // Default Variables
  GameData data = {0};

  // Color Pallete
  Color bgColor = BLACK;
  Color neonGreen = {57, 255, 20, 255};

  // Application Run
  while (!WindowShouldClose()) {

    BeginDrawing();

    Vector2 mousePoint = GetMousePosition();
    bool isHovering = false;
    bool isClicked = false;

    if (currentScreen == SCREEN_MAIN) {
      ClearBackground(bgColor);

      // MAIN Variables
      const int blockHeight = 75;
      Rectangle crankButton = {12, 12 + blockHeight, (screenWidth / 3 - 24),
                               blockHeight};

      // Draw Basic Outlines
      DrawRectangleLines(6, 6, screenWidth - 12, screenHeight - 12, neonGreen);
      DrawRectangleLines(6, 6, screenWidth / 3 - 12, screenHeight - 12,
                         neonGreen);
      DrawRectangleLines(6, 6, (screenWidth / 3) * 2 - 12, screenHeight - 12,
                         neonGreen);
      DrawRectangleLines(6, 6, screenWidth / 3 - 12, blockHeight + 3,
                         neonGreen);

      isHovering = CheckCollisionPointRec(mousePoint, crankButton);
      isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
      DrawRectangleRec(crankButton, isHovering ? GRAY : BLACK);
      DrawText("Click to rotate the crank.",
               (screenWidth / 6) -
                   MeasureText("Click to rotate the crank.", 20) / 2,
               (12 + blockHeight) + (blockHeight / 2) - 10, 20, neonGreen);

#pragma region Sunlight function
      char dayTimerText[30];
      snprintf(dayTimerText, sizeof(dayTimerText), "Sunlight %: %0.1f",
               dayPercentage);
      DrawText(dayTimerText,
               (screenWidth / 6) - (MeasureText(dayTimerText, 20) / 2),
               blockHeight / 2 - 10, 20, neonGreen);

      // Draw If Its Day or Night
      if (isSunlight) {
        DrawText("DAY", (screenWidth / 6) - MeasureText("DAY", 20) / 2,
                 blockHeight / 2 + 24, 20, neonGreen);
      } else {
        DrawText("NIGHT", (screenWidth / 6) - MeasureText("NIGHT", 20) / 2,
                 blockHeight / 2 + 24, 20, neonGreen);
      }

      // SunLight Calculation
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
#pragma endregion
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