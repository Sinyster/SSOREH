#include "raylib.h"
#include <stdio.h>

typedef struct {
  double money;
  float sunlight;
} GameData;

typedef struct {
  char name[64];
} Battery;

typedef struct {
  char name[64];
} Generator;

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
    char moneyText[30];
    snprintf(moneyText, sizeof(moneyText), "$ %0.1f", data.money);
    DrawText(moneyText,
             screenWidth / 2 - MeasureText(moneyText, HeaderFont) / 2,
             screenHeight - PanelHeight + PanelHeight / 2 - HeaderFont / 2,
             HeaderFont, HeaderColor);
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