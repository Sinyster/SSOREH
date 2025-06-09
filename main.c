#include "raylib.h"
#include "variables.h"
#include <stdio.h>

// Functions
// Functions: Render
void RenderUpperPanel(Color bg);
void RenderLowerPanel(Color bg);
void RenderPopUp(Color bg, Rectangle Button, Vector2 MousePoint);

// Functions: Texts
void RenderUpperPanelTexts(GameScreen CurrentScreen, Color Active,
                           Color Inactive);

// Functions: Enable
void EnableGameButton(Vector2 VectorPointer);
int main(void) {
  // Base Vars - Windows
  InitWindow(ScreenWidth, ScreenHeight, Title);
  SetTargetFPS(TargetFps);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BackgroundWhite);

    Vector2 MousePoint = GetMousePosition();

    // Upper Panel
    RenderUpperPanel(PanelBackground);

    // Lower Panel
    RenderLowerPanel(PanelBackground);

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

// Function for Rendering Background of Upper panel
void RenderUpperPanel(Color bg) {
  DrawRectangle(0, 0, ScreenWidth, PanelHeight, bg);
  return;
}

// Function for Rendering Background of Lower panel
void RenderLowerPanel(Color bg) {
  DrawRectangle(0, ScreenHeight - PanelHeight, ScreenWidth, PanelHeight, bg);
  return;
}

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