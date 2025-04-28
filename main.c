#include "raylib.h"
#include <stdio.h>
#include <string.h>

typedef struct {
  // Variables
  double whPerSecond;
  double whPerClick;

  // Generator Variables
  int activeGenerator;

  // Battery Variables
  int activeBattery;
  double batteryCharge;
} GameData;

// Battery Variables
typedef struct {
  char name[30];
  double capacity;
  double maxChargeRate;
  double defectRate;

  double chargeInPercent;
} Battery;

void setBatteryVariables(Battery *battery, int selected);

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
  data.whPerClick = 0.5;

  Battery battery = {0};
  setBatteryVariables(&battery, 1);

  // Color Pallete
  Color bgColor = BLACK;
  Color neonGreen = {57, 255, 20, 255};
  Color warning = {255, 10, 10, 255};

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
      Rectangle crankButton = {12, blockHeight + 16, (screenWidth / 3 - 24),
                               blockHeight * 2};

      // Draw Basic Outlines
      DrawRectangleLines(6, 6, screenWidth - 12, screenHeight - 12, neonGreen);
      DrawRectangleLines(6, 6, screenWidth / 3 - 12, screenHeight - 12,
                         neonGreen);
      DrawRectangleLines(6, 6, (screenWidth / 3) * 2 - 12, screenHeight - 12,
                         neonGreen);
      DrawRectangleLines(6, 6, screenWidth / 3 - 12, blockHeight + 3,
                         neonGreen);
      DrawRectangleLines(6, 6, screenWidth / 3 - 12, screenHeight / 3,
                         neonGreen);
      DrawRectangleLines(6, 6, screenWidth / 3 - 12, (screenHeight / 3) * 2,
                         neonGreen);

      isHovering = CheckCollisionPointRec(mousePoint, crankButton);
      isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
      DrawRectangleRec(crankButton, isHovering ? GRAY : BLACK);
      DrawText("Click to rotate the crank.",
               (screenWidth / 6) -
                   MeasureText("Click to rotate the crank.", 20) / 2,
               (12 + blockHeight) + blockHeight - 10, 20, neonGreen);

      if (isClicked || IsKeyDown(KEY_C)) {
        if (battery.capacity > data.batteryCharge) {
          data.batteryCharge += data.whPerClick;
        }
      }

      char whPerClickText[30];
      snprintf(whPerClickText, sizeof(whPerClickText), "%0.2f Wh per click",
               data.whPerClick);
      DrawText(whPerClickText,
               screenWidth / 6 - MeasureText(whPerClickText, 20) / 2,
               (12 + blockHeight) + blockHeight + 10, 20, neonGreen);

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
#pragma region Battery
      setBatteryVariables(&battery, 1);

      // Formatting variables for text output.
      char batCapacityText[30];
      char batMaxChargeText[30];
      char batChargeText[30];
      char batChargePercentText[30];

      snprintf(batCapacityText, sizeof(batCapacityText),
               "Max. Capacity: %0.0f Wh", battery.capacity);
      snprintf(batMaxChargeText, sizeof(batMaxChargeText),
               "Max. Charge Rate: %0.2f Wh/s", battery.maxChargeRate);
      snprintf(batChargeText, sizeof(batChargeText), "Current Charge: %0.2f Wh",
               data.batteryCharge);
      snprintf(batChargePercentText, sizeof(batChargePercentText),
               "Current Charge in %: %0.2f", battery.chargeInPercent);

      // Draw Battery Info
      DrawText("Battery: ", 10, screenHeight / 3 + 10, 20, neonGreen);
      DrawText(battery.name, 10 + MeasureText("Battery: ", 20),
               screenHeight / 3 + 10, 20, neonGreen);
      DrawText(batCapacityText, 10, screenHeight / 3 + 50, 20, neonGreen);
      DrawText(batMaxChargeText, 10, screenHeight / 3 + 70, 20, neonGreen);
      DrawText(batChargeText, 10, screenHeight / 3 + 110, 20, neonGreen);
      if (battery.chargeInPercent > 75.0) {
        DrawText(batChargePercentText, 10, screenHeight / 3 + 130, 20, warning);
      } else {
        DrawText(batChargePercentText, 10, screenHeight / 3 + 130, 20,
                 neonGreen);
      }

      // Calculations ============================
      // Percentage
      battery.chargeInPercent = 100.0 * (data.batteryCharge / battery.capacity);

#pragma endregion
#pragma region Upgrades
      // Battery Upgrade

#pragma endregion
    }

    EndDrawing();
  }
  CloseWindow();

  return 0;
}

void setBatteryVariables(Battery *battery, int selected) {
  switch (selected) {
  case 1:
    // Basic Battery (on start)
    strcpy(battery->name, "Lithium-Ion Battery");
    battery->capacity = 1000.0;
    battery->maxChargeRate = 1.0;
    battery->defectRate = 5.0; // per 1Wh
    break;
  }
}