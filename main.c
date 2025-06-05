#include "raylib.h"
#include "variables.h"

// Functions
void RenderUpperPanel(Color bg);
void RenderLowerPanel(Color bg);

int main(void) {
  InitWindow(ScreenWidth, ScreenHeight, Title);
  SetTargetFPS(TargetFps);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BackgroundWhite);

    // Upper Panel
    RenderUpperPanel(PanelBackground);

    // Lower Panel
    RenderLowerPanel(PanelBackground);

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