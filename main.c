#include "raylib.h"
#include "variables.h"
#include <stdio.h>
#include <string.h>

// Functions
// Functions: Render
void RenderUpperPanel(Color bg);
void RenderLowerPanel(Color bg);
void RenderPopUp(Color bg, Rectangle Button, Vector2 MousePoint);
void RenderBatteryPlayScreen();
void RenderGeneratorPlayScreen(Vector2 MousePoint);

// Functions: Render: Division
void DivideIntoThree();

// Functions: Texts
void RenderUpperPanelTexts(GameScreen CurrentScreen, Color Active,
                           Color Inactive);

// Functions: Enable
void EnableGameButton(Vector2 VectorPointer);

// Functions: Calculations
void CalculateBatteryPercentage();
void CalculateSunlight();

// Define stuff
void DefineBatteries(Battery *bat);
void DefineGenerators(Generator *gen);

int main(void) {
  // Base Vars - Windows
  InitWindow(ScreenWidth, ScreenHeight, Title);
  SetTargetFPS(TargetFps);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BackgroundWhite);

    Vector2 MousePoint = GetMousePosition();

    // Game Calculations
    CalculateBatteryPercentage();
    CalculateSunlight();

    // Define Batteries
    DefineBatteries(&bat);
    DefineGenerators(&gen);

    // Upper Panel
    RenderUpperPanel(PanelBackground);

    // Functionality of Game Button
    EnableGameButton(MousePoint);

    // Functionality of Upgrade Button
    Rectangle BtnUpgrade = {ScreenWidth / NumOfUPT, 0, ScreenWidth / NumOfUPT,
                            PanelHeight};
    isHovering = CheckCollisionPointRec(MousePoint, BtnUpgrade);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    DrawRectangleRec(BtnUpgrade, isHovering ? GRAY : PanelBackground);
    if (isClicked && !isUpgPopUpShowed) {
      isUpgPopUpShowed = true;
    } else if (isClicked && isUpgPopUpShowed) {
      isUpgPopUpShowed = false;
    }

    switch (CurrentScreen) {
    case SCREEN_PLAY:

      // Layer 1: Lower Panel
      RenderLowerPanel(PanelBackground);

      // Layer 2: Texts
      RenderBatteryPlayScreen();
      Rectangle GenBtn = {ScreenWidth / 3, PanelHeight + Spacing,
                          ScreenWidth / 3 - 4,
                          ScreenHeight - PanelHeight * 2 - Spacing * 2};
      isHovering = CheckCollisionPointRec(MousePoint, GenBtn);
      isClicked = isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
      DrawRectangleRec(GenBtn, isHovering ? LIGHTGRAY : BackgroundWhite);

      if (isClicked) {
        if (Data.ActiveGenerator == 0) {
          bat.actualCapacity += gen.genPerClick;
        } else {
          isGenerating = !isGenerating;
        }
      }
      RenderGeneratorPlayScreen(MousePoint);

      // Layer 4: Division
      DivideIntoThree();
      break;
    case SCREEN_UPGRADE:
      // Layer 1: Lower Panel
      RenderLowerPanel(PanelBackground);
      break;
    case SCREEN_STATS:
      break;
    case SCREEN_SETTINGS:
      break;
    case SCREEN_MENU:
      break;
    default:
      break;
    }

    // It's down here cause of render layering
    // Render Upgrade Popup
    if (isUpgPopUpShowed) {
      RenderPopUp(PanelBackground, BtnUpgrade, MousePoint);
    }

    // If clicked anywhere else then PopUp, close.
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        !CheckCollisionPointRec(MousePoint, BtnUpgrade)) {
      isUpgPopUpShowed = false;
    }

    // Upper Panel: Texts (Here cause of layering objects)
    RenderUpperPanelTexts(CurrentScreen, BLACK, DARKGRAY);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}

// RENDER
// Function for Rendering Background of Upper panel
void RenderUpperPanel(Color bg) {
  DrawRectangle(0, 0, ScreenWidth, PanelHeight, bg);
  return;
}

// Function for Rendering Background of Lower panel
void RenderLowerPanel(Color bg) {
  // Draw Background as Layer 1
  DrawRectangle(0, ScreenHeight - PanelHeight, ScreenWidth, PanelHeight, bg);

  char buffer[128];
  float x = 0.0f;
  float y = ScreenHeight - PanelHeight / 2 - FontSizeHeader / 2;

  // Text as Layer 2
  // Render Money
  snprintf(buffer, sizeof(buffer), "%0.2f$", Data.money);
  x = ScreenWidth / 2.0 - (float)MeasureText(buffer, FontSizeText) / 2;
  DrawText(buffer, x, y, FontSizeHeader, BLACK);

  // Battery Capacity in %
  snprintf(buffer, sizeof(buffer), "Battery %: %0.2f", bat.percentage);
  x = (ScreenWidth / 5 / 2) - (float)MeasureText(buffer, FontSizeText) / 2;
  y = ScreenHeight - PanelHeight / 2 - FontSizeText / 2;
  DrawText(buffer, x, y, FontSizeText, DARKGRAY);

  // Sunlight in %
  snprintf(buffer, sizeof(buffer), "Sunlight %: %0.2f", Data.sunlight);
  x = ScreenWidth - (ScreenWidth / NumOfUPT / 2) -
      (float)MeasureText(buffer, FontSizeText) / 2;
  DrawText(buffer, x, y, FontSizeText, DARKGRAY);

  // Electricity Input

  // Electricity Output
  return;
}

// Function for rendering Popup
void RenderPopUp(Color bg, Rectangle Button, Vector2 MousePoint) {
  if (isUpgPopUpShowed) {
    // First Render Background as Layer 1
    Rectangle popup = {Button.x, Button.y + PanelHeight, Button.width,
                       PanelHeight * NumOfUPUT};
    DrawRectangleRec(popup, bg);

    // Second Render are Buttons as Layer 2
    Rectangle Batteries = {popup.x, popup.y, popup.width, PanelHeight};
    isHovering = CheckCollisionPointRec(MousePoint, Batteries);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    DrawRectangleRec(Batteries, isHovering ? GRAY : PanelBackground);

    if (isClicked) {
      UpgScreen = UPG_BAT;
      CurrentScreen = SCREEN_UPGRADE;
      isGameButtonAlowed = true;
      isUpgPopUpShowed = false;
    }

    Rectangle Generators = {Batteries.x, Batteries.y + PanelHeight, popup.width,
                            PanelHeight};
    isHovering = CheckCollisionPointRec(MousePoint, Generators);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    DrawRectangleRec(Generators, isHovering ? GRAY : PanelBackground);

    if (isClicked) {
      UpgScreen = UPG_GEN;
      CurrentScreen = SCREEN_UPGRADE;
      isGameButtonAlowed = true;
      isUpgPopUpShowed = false;
    }

    Rectangle Machines = {Generators.x, Generators.y + PanelHeight, popup.width,
                          PanelHeight};
    isHovering = CheckCollisionPointRec(MousePoint, Machines);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    DrawRectangleRec(Machines, isHovering ? GRAY : PanelBackground);

    if (isClicked) {
      UpgScreen = UPG_MAC;
      CurrentScreen = SCREEN_UPGRADE;
      isGameButtonAlowed = true;
      isUpgPopUpShowed = false;
    }

    // Third Render are Titles as Layer 3
    char buffer[128];
    for (int i = 0; i < NumOfUPUT; i++) {
      snprintf(buffer, sizeof(buffer), "%s", UpgradePopupTitles[i]);

      float x = Button.x + Spacing;
      float y = (Button.y + PanelHeight) * (i + 1) + Spacing;

      DrawText(buffer, x, y, FontSizeText, DARKGRAY);
    }
  }
  return;
}

// Function for drawing 3 division lines.
void DivideIntoThree() {
  for (int i = 0; i < 3; i++) {
    DrawLine(ScreenWidth / 3 * i - 1, PanelHeight + Spacing,
             ScreenWidth / 3 * i - 1, ScreenHeight - PanelHeight - Spacing,
             BLACK);
  }
  return;
}

// Function for rendering battery info on screen
void RenderBatteryPlayScreen() {
  // Buffer Variable for formatting other vars
  char buffer[128];

  float x = Spacing;
  float y = PanelHeight * 2;

  Color color = DARKGRAY;

  // Drawing Main Label + UnderLine

  snprintf(buffer, sizeof(buffer), "Battery: %s", bat.name);
  DrawText(buffer, x, y, FontSizeText, BLACK);

  // Actual Capacity
  y += FontSizeText;

  snprintf(buffer, sizeof(buffer), "Capacity: %0.2f Wh", bat.actualCapacity);
  DrawText(buffer, x, y, FontSizeText, color);

  // Capacity in %
  y += FontSizeText;
  snprintf(buffer, sizeof(buffer), "Capacity %: %0.2f", bat.percentage);
  DrawText(buffer, x, y, FontSizeText, color);

  // Max Capacity
  y += FontSizeText;

  snprintf(buffer, sizeof(buffer), "Max Capacity: %0.2f Wh", bat.maxCapacity);
  DrawText(buffer, x, y, FontSizeText, color);
  return;
}

void RenderGeneratorPlayScreen(Vector2 MousePoint) {
  char buffer[128];

  if (Data.ActiveGenerator == 0) {
    snprintf(buffer, sizeof(buffer), "Click Anywhere!");
    DrawText(buffer,
             ScreenWidth / 2 - (float)MeasureText(buffer, FontSizeHeader) / 2,
             ScreenHeight - PanelHeight * 3, FontSizeHeader, DARKGRAY);
  }

  float x = ScreenWidth / 3 + Spacing;
  float y = PanelHeight * 2;

  // Generator Name
  snprintf(buffer, sizeof(buffer), "Generator: %s", gen.name);
  DrawText(buffer, x, y, FontSizeText, BLACK);

  return;
}

// TEXTS
// Function for Rendering Upper Panel Button Texts - Even Active one
void RenderUpperPanelTexts(GameScreen CurrentScreen, Color Active,
                           Color Inactive) {
  char buffer[128];
  for (int i = 0; i < NumOfUPT; i++) {
    const char *title = UpperPanelTitles[i];
    if (i == CurrentScreen) {
      snprintf(buffer, sizeof(buffer), "*%s*", title);
    } else {
      snprintf(buffer, sizeof(buffer), "%s", title);
    }

    float textWidth = MeasureText(buffer, FontSizeHeader);
    float x = (ScreenWidth / NumOfUPT) * (i + 0.5f) - textWidth / 2;
    float y = PanelHeight / 2 - FontSizeHeader / 2;

    DrawText(buffer, x, y, FontSizeHeader,
             (i == CurrentScreen) ? Active : Inactive);
  }
  return;
}

// OTHER
// Function for Game Button
void EnableGameButton(Vector2 VectorPointer) {
  if (isGameButtonAlowed) {
    Rectangle GameBtn = {0, 0, ScreenWidth / NumOfUPT, PanelHeight};
    isHovering = CheckCollisionPointRec(VectorPointer, GameBtn);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    DrawRectangleRec(GameBtn, isHovering ? GRAY : PanelBackground);

    if (isClicked && CurrentScreen != SCREEN_PLAY) {
      isGameButtonAlowed = false;
      CurrentScreen = SCREEN_PLAY;
    }
  }
  return;
}

// Simple Function for calculating Battery Capacity in %
void CalculateBatteryPercentage() {
  bat.percentage = 100.0f * (bat.actualCapacity / bat.maxCapacity);
  return;
}

// sunlight Function
void CalculateSunlight() {
  Data.DayTimer += GetFrameTime();
  if (isDay) {
    if (Data.DayTimer < (float)DayMinutes / 2) {
      Data.sunlight += 100.0f / ((float)DayMinutes / 2 * TargetFps);
      if (Data.sunlight > 100.0f) {
        Data.sunlight = 100.0f;
      }
    } else if (Data.DayTimer > (float)DayMinutes / 2) {
      Data.sunlight -= 100.0f / ((float)DayMinutes / 2 * TargetFps);
      if (Data.sunlight < 0.1f) {
        Data.sunlight = 0.0f;
        Data.DayTimer = 0.0f;
        isDay = false;
      }
    }
  } else {
    Data.DayTimer += GetFrameTime();
    if (Data.DayTimer > (float)NightMinutes) {
      isDay = true;
      Data.DayTimer = 0.0f;
    }
  }
  return;
}

// Define Stuff
// Define Batteries
void DefineBatteries(Battery *bat) {
  int battery = Data.ActiveBattery;
  switch (battery) {
  case 0:
    // Base Battery
    strcpy(bat->name, "Lithium Battery");
    bat->maxCapacity = 1000.0; // Wh
    bat->maxInput = 0.5;       // Wh
    bat->maxOutput = 0.5;      // Wh
    break;
  case 1:
    break;
  }
  return;
}

// Define Generators
void DefineGenerators(Generator *gen) {
  int generator = Data.ActiveGenerator;
  switch (generator) {
  case 0:
    strcpy(gen->name, "Handcrank");
    gen->genPerClick = 0.1f; // Wh
    break;
  }
  return;
}