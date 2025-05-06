#include "functions.h"
#include "raylib.h"

// #include <stdio.h>

typedef struct {
  double money;
  float sunlight;
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

  // Day/Night Variables
  const int dayMinutes = 10;
  const int nightMinutes = 5;
  float sunlight = 0.0f;
  bool isDay = true;

  // Setting Screens
  typedef enum { SCREEN_MAIN } GameScreen;
  GameScreen currentScreen = SCREEN_MAIN;

  // Timer Variables
  float dayTimer = 0.0f;

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

  // Functional Variables
  bool isHovering = false;
  bool isClicked = false;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(bg1);

    Vector2 mousePoint = GetMousePosition();

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
#pragma region Lower Panel

#pragma region SCREEN_MAIN
    if (currentScreen = SCREEN_MAIN) {
    }

    EndDrawing();
  }
  CloseWindow();
  return 0;
}