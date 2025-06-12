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
void RenderGeneratorPlayScreen();
void RenderMachinePlayScreen();

void TextBox(Rectangle bounds, const char *text, const char *Title, Font font,
             float fontSize, float spacing, Color textColor, Color borderColor,
             Color bgColor);

// Functions: Render: Division
void DivideIntoThree();

// Functions: Render Upgrade Screens
void RenderBatteryUpgradeScreen(Rectangle rec1, Rectangle rec2, Rectangle rec3);
void RenderGeneratorUpgradeScreen(Rectangle MainRec, Rectangle ExtraRec);
void RenderMachineUpgradeScreen();

// Functions: Texts
void RenderUpperPanelTexts(GameScreen CurrentScreen, Color Active,
                           Color Inactive);
void WrapTextToFit(const char *text, char *output, Font font, float fontSize,
                   float spacing, float maxWidth);
void SetTexts();

// Functions: Enable
void EnableGameButton(Vector2 VectorPointer);

// Functions: Calculations
void CalculateBatteryPercentage();
void CalculateSunlight();
void GeneratingElectricity();
void GeneratingMoney();

// Functions: Define stuff
void DefineBatteries(Battery *bat);
void DefineGenerators(Generator *gen);
void DefineMachines(Machines *mac);

int main(void) {
  // Define Colors
  if (!isDarkmode) {
    // Lightmode
    activeBackground = LightBackground;
    activePanelBackground = LightPanelBackground;
    activeHover = LightHover;
    activeFontActive = LightFontActive;
    activeFontInactive = LightFontInactive;
  } else {
    // Darkmode
  }

  // Base Vars - Windows
  InitWindow(ScreenWidth, ScreenHeight, Title);
  SetTargetFPS(TargetFps);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(activeBackground);

    Vector2 MousePoint = GetMousePosition();

    // Game Calculations
    CalculateBatteryPercentage();

    // Define Batteries
    DefineBatteries(&bat);
    DefineGenerators(&gen);
    DefineMachines(&mac);

    // Upper Panel
    RenderUpperPanel(activePanelBackground);

    // Functionality of Game Button
    EnableGameButton(MousePoint);

    // Functionality of Upgrade Button
    Rectangle BtnUpgrade = {ScreenWidth / NumOfUPT, 0, ScreenWidth / NumOfUPT,
                            PanelHeight};
    isHovering = CheckCollisionPointRec(MousePoint, BtnUpgrade);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    DrawRectangleRec(BtnUpgrade,
                     isHovering ? activeHover : activePanelBackground);
    if (isClicked && !isUpgPopUpShowed) {
      isUpgPopUpShowed = true;
    } else if (isClicked && isUpgPopUpShowed) {
      isUpgPopUpShowed = false;
    }

    switch (CurrentScreen) {
    case SCREEN_PLAY:
      // Calculations
      CalculateSunlight();

      // Generating Function
      Rectangle GenBtn = {ScreenWidth / 3, PanelHeight + Spacing,
                          ScreenWidth / 3 - 4,
                          ScreenHeight - PanelHeight * 2 - Spacing * 2};
      isHovering = CheckCollisionPointRec(MousePoint, GenBtn);
      isClicked = isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
      DrawRectangleRec(GenBtn,
                       isHovering ? activePanelBackground : activeBackground);

      InputTimer += GetFrameTime();
      if (InputTimer >= 1.0f) {
        InputUsedThisSecond = 0.0f;
        InputTimer = 0.0f;
        input = 0.0f;
      }

      if (isClicked) {
        if (Data.ActiveGenerator == 0) {
          float toAdd = gen.genPerClick;

          if (InputUsedThisSecond + toAdd <= bat.maxInput) {
            bat.actualCapacity += toAdd;
            InputUsedThisSecond += toAdd;
          } else {
            float remaining = bat.maxInput - InputUsedThisSecond;
            if (remaining > 0.0f) {
              bat.actualCapacity += remaining;
              InputUsedThisSecond += remaining;
            }
          }
          input = InputUsedThisSecond;

          if (bat.actualCapacity >= bat.maxCapacity) {
            bat.actualCapacity = bat.maxCapacity;
          }

        } else {
          isGenerating = !isGenerating;
        }
      }

      if (isGenerating) {
        GeneratingElectricity();
      }

      // Selling Function
      isHovering = CheckCollisionPointRec(MousePoint, SellBtn);
      isClicked = isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
      DrawRectangleRec(SellBtn,
                       isHovering ? activePanelBackground : activeBackground);

      if (isClicked && !isSelling && bat.actualCapacity > 0.0) {
        isSelling = true;
      } else if (isClicked && isSelling) {
        isSelling = false;
      }

      if (isSelling) {
        GeneratingMoney();
        ActualOutput = mac.drain;
        DrawText("Turn Off",
                 SellBtn.x + SellBtn.width / 2 -
                     (float)MeasureText("Turn Off", FontSizeHeader) / 2,
                 ScreenHeight - PanelHeight * 3, FontSizeHeader,
                 activeFontInactive);
      } else {
        ActualOutput = 0.0f;
        DrawText("Turn On",
                 SellBtn.x + SellBtn.width / 2 -
                     (float)MeasureText("Turn On", FontSizeHeader) / 2,
                 ScreenHeight - PanelHeight * 3, FontSizeHeader,
                 activeFontInactive);
      }

      // Layer 1: Lower Panel
      RenderLowerPanel(activePanelBackground);

      // Layer 2: Texts
      RenderBatteryPlayScreen();
      RenderGeneratorPlayScreen();
      RenderMachinePlayScreen();

      // Layer 4: Division
      DivideIntoThree();
      break;
    case SCREEN_UPGRADE:
      // Layer 1: Lower Panel
      RenderLowerPanel(activePanelBackground);

      switch (UpgScreen) {
      case UPG_BAT:
        // Functionality
        // Low Voltage Upgrades
        Rectangle LowVoltageUnlock = {
            Spacing, PanelHeight * 3 + Spacing, ScreenWidth / 3 - Spacing * 2,
            ScreenHeight - PanelHeight * 4 - Spacing * 2};

        if (Data.voltageBat == 0) {
          isHovering = CheckCollisionPointRec(MousePoint, LowVoltageUnlock);
          isClicked = isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
          DrawRectangleRec(LowVoltageUnlock, isHovering ? activePanelBackground
                                                        : activeBackground);
          if (isClicked && Data.money >= bat.price) {
            Data.money -= bat.price;
            Data.ActiveBattery += 1;
            // Function for increasing Voltage Level
          }
        }

        // Medium Voltage Upgrades
        Rectangle MediumVoltageUnlock = {
            ScreenWidth / 3 + Spacing, PanelHeight * 3 + Spacing,
            ScreenWidth / 3 - Spacing * 2,
            ScreenHeight - PanelHeight * 4 - Spacing * 2};

        if (Data.voltageBat == 1) {
          isHovering = CheckCollisionPointRec(MousePoint, MediumVoltageUnlock);
          isClicked = isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
          DrawRectangleRec(MediumVoltageUnlock, isHovering
                                                    ? activePanelBackground
                                                    : activeBackground);
          if (isClicked && Data.money >= bat.price) {
            Data.money -= bat.price;
            Data.ActiveBattery += 1;
            // Function for increasing Voltage Level
          }
        }

        // High Voltage Upgrades
        Rectangle HighVoltageUnlock = {
            ScreenWidth - ScreenWidth / 3, PanelHeight * 3 + Spacing,
            ScreenWidth / 3 - Spacing * 2,
            ScreenHeight - PanelHeight * 4 - Spacing * 2};

        if (Data.voltageBat == 2) {
          isHovering = CheckCollisionPointRec(MousePoint, HighVoltageUnlock);
          isClicked = isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
          DrawRectangleRec(HighVoltageUnlock, isHovering ? activePanelBackground
                                                         : activeBackground);
          if (isClicked && Data.money >= bat.price) {
            Data.money -= bat.price;
            Data.ActiveBattery += 1;
            // Function for increasing Voltage Level
          }
        }

        // Layer 1: Texts
        RenderBatteryUpgradeScreen(LowVoltageUnlock, MediumVoltageUnlock,
                                   HighVoltageUnlock);
        break;
      case UPG_GEN:
        // Functionality
        // Main Generator Upgrade Button
        Rectangle MainGeneratorUpgrade = {Spacing, PanelHeight * 3 + Spacing,
                                          ScreenWidth / 3 - Spacing * 2,
                                          PanelHeight * 3};
        isHovering = CheckCollisionPointRec(MousePoint, MainGeneratorUpgrade);
        isClicked = isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        DrawRectangleRec(MainGeneratorUpgrade,
                         isHovering ? activePanelBackground : activeBackground);
        // if(isClicked)

        Rectangle ExtraGeneratorUpgrade = {
            Spacing,
            MainGeneratorUpgrade.y + MainGeneratorUpgrade.height +
                PanelHeight * 2,
            ScreenWidth / 3 - Spacing * 2,
            ScreenHeight - MainGeneratorUpgrade.height - PanelHeight * 6 -
                Spacing * 2};
        isHovering = CheckCollisionPointRec(MousePoint, ExtraGeneratorUpgrade);
        isClicked = isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        DrawRectangleRec(ExtraGeneratorUpgrade,
                         isHovering ? activePanelBackground : activeBackground);

        // Layer 2: Texts
        RenderGeneratorUpgradeScreen(MainGeneratorUpgrade,
                                     ExtraGeneratorUpgrade);
        break;
      case UPG_MAC:
        break;
      }
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
      RenderPopUp(activePanelBackground, BtnUpgrade, MousePoint);
    }

    // If clicked anywhere else then PopUp, close.
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        !CheckCollisionPointRec(MousePoint, BtnUpgrade)) {
      isUpgPopUpShowed = false;
    }

    // Upper Panel: Texts (Here cause of layering objects)
    RenderUpperPanelTexts(CurrentScreen, activeFontActive, activeFontInactive);

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
  DrawText(buffer, x, y, FontSizeHeader, activeFontActive);

  // Battery Capacity in %
  snprintf(buffer, sizeof(buffer), "Battery %: %0.2f", bat.percentage);
  x = (ScreenWidth / 5 / 2) - (float)MeasureText(buffer, FontSizeText) / 2;
  y = ScreenHeight - PanelHeight / 2 - FontSizeText / 2;
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);

  // Sunlight in %
  snprintf(buffer, sizeof(buffer), "Sunlight %: %0.2f", Data.sunlight);
  x = ScreenWidth - (ScreenWidth / NumOfUPT / 2) -
      (float)MeasureText(buffer, FontSizeText) / 2;
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);

  // Electricity Input
  snprintf(buffer, sizeof(buffer), "Input: %0.2f", input);
  x = ScreenWidth / NumOfUPT + (ScreenWidth / NumOfUPT / 2) -
      (float)MeasureText(buffer, FontSizeText) / 2;
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);

  // Electricity Output
  snprintf(buffer, sizeof(buffer), "Output: %0.2f", ActualOutput);
  x = ScreenWidth - (ScreenWidth / NumOfUPT) - (ScreenWidth / NumOfUPT / 2) -
      (float)MeasureText(buffer, FontSizeText) / 2;
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);
  return;
}

#pragma region Render
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
    DrawRectangleRec(Batteries,
                     isHovering ? activeHover : activePanelBackground);

    if (isClicked) {
      CurrentScreen = SCREEN_UPGRADE;
      UpgScreen = UPG_BAT;
      isGameButtonAlowed = true;
      isUpgPopUpShowed = false;
    }

    Rectangle Generators = {Batteries.x, Batteries.y + PanelHeight, popup.width,
                            PanelHeight};
    isHovering = CheckCollisionPointRec(MousePoint, Generators);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    DrawRectangleRec(Generators,
                     isHovering ? activeHover : activePanelBackground);

    if (isClicked) {
      CurrentScreen = SCREEN_UPGRADE;
      UpgScreen = UPG_GEN;
      isGameButtonAlowed = true;
      isUpgPopUpShowed = false;
    }

    Rectangle Machines = {Generators.x, Generators.y + PanelHeight, popup.width,
                          PanelHeight};
    isHovering = CheckCollisionPointRec(MousePoint, Machines);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    DrawRectangleRec(Machines,
                     isHovering ? activeHover : activePanelBackground);

    if (isClicked) {
      CurrentScreen = SCREEN_UPGRADE;
      UpgScreen = UPG_MAC;
      isGameButtonAlowed = true;
      isUpgPopUpShowed = false;
    }

    // Third Render are Titles as Layer 3
    char buffer[128];
    for (int i = 0; i < NumOfUPUT; i++) {
      snprintf(buffer, sizeof(buffer), "%s", UpgradePopupTitles[i]);

      float x = Button.x + Spacing;
      float y = (Button.y + PanelHeight) * (i + 1) + Spacing;

      DrawText(buffer, x, y, FontSizeText, activeFontInactive);
    }
  }
  return;
}

// Function for drawing 3 division lines.
void DivideIntoThree() {
  for (int i = 0; i < 3; i++) {
    DrawLine(ScreenWidth / 3 * i - 1, PanelHeight + Spacing,
             ScreenWidth / 3 * i - 1, ScreenHeight - PanelHeight - Spacing,
             activeFontActive);
  }
  return;
}

// Function for rendering battery info on screen
void RenderBatteryPlayScreen() {
  // Buffer Variable for formatting other vars
  char buffer[128];

  float x = Spacing;
  float y = PanelHeight * 2;

  Color color = activeFontInactive;

  // Drawing Main Label + UnderLine
  snprintf(buffer, sizeof(buffer), "Battery: %s", bat.name);
  DrawText(buffer, x, y, FontSizeText, activeFontActive);

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

  // Max Input
  y += FontSizeText;
  snprintf(buffer, sizeof(buffer), "Max Input: %0.2f W/s", bat.maxInput);
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);
  return;
}

// Function for rendering generator info on screen
void RenderGeneratorPlayScreen() {
  char buffer[128];

  float x = ScreenWidth / 3 + Spacing;
  float y = PanelHeight * 2;
  // Generator Name
  snprintf(buffer, sizeof(buffer), "Generator: %s", gen.name);
  DrawText(buffer, x, y, FontSizeText, activeFontActive);

  // Generates
  y += FontSizeText;
  if (Data.ActiveGenerator == 0) {
    snprintf(buffer, sizeof(buffer), "Click Anywhere!");
    DrawText(buffer,
             ScreenWidth / 2 - (float)MeasureText(buffer, FontSizeHeader) / 2,
             ScreenHeight - PanelHeight * 3, FontSizeHeader,
             activeFontInactive);

    snprintf(buffer, sizeof(buffer), "Generates Per Click: %0.2f W",
             gen.genPerClick);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);
  } else {
    snprintf(buffer, sizeof(buffer), "Generates Per Sec: %0.2f W",
             gen.genPerSec);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);
  }

  // Specials
  y += FontSizeText;
  snprintf(buffer, sizeof(buffer), "Special: %s", gen.Special);
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);
  return;
}

// Function for rendering machine info on screen
void RenderMachinePlayScreen() {
  char buffer[128];

  float x = (ScreenWidth - ScreenWidth / 3) + Spacing;
  float y = PanelHeight * 2;

  // Main Label
  snprintf(buffer, sizeof(buffer), "Machine: %s", mac.name);
  DrawText(buffer, x, y, FontSizeText, activeFontActive);

  // Drainage
  y += FontSizeText;
  snprintf(buffer, sizeof(buffer), "Drains: %0.2f W/s", mac.drain);
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);

  // $ Per Drain
  y += FontSizeText;
  snprintf(buffer, sizeof(buffer), "Sells for: %0.2f$", mac.output);
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);
  return;
}

#pragma region TEXTS
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

// Function for wrapping text...
void WrapTextToFit(const char *text, char *output, Font font, float fontSize,
                   float spacing, float maxWidth) {
  const char *wordStart = text;
  char line[1024] = {0};
  output[0] = '\0';

  while (*wordStart) {
    const char *wordEnd = wordStart;
    while (*wordEnd && *wordEnd != ' ')
      wordEnd++;

    char word[128] = {0};
    strncpy(word, wordStart, wordEnd - wordStart);
    word[wordEnd - wordStart] = '\0';

    char testLine[1024] = {0};
    if (line[0] != '\0')
      snprintf(testLine, sizeof(testLine), "%s %s", line, word);
    else
      snprintf(testLine, sizeof(testLine), "%s", word);

    Vector2 size = MeasureTextEx(font, testLine, fontSize, spacing);
    if (size.x > maxWidth && line[0] != '\0') {
      strcat(output, line);
      strcat(output, "\n");
      snprintf(line, sizeof(line), "%s", word);
    } else {
      snprintf(line, sizeof(line), "%s", testLine);
    }

    wordStart = wordEnd;
    while (*wordStart == ' ')
      wordStart++;
  }

  strcat(output, line);
  return;
}

// Function for Rendering reuseable Textbox
void TextBox(Rectangle bounds, const char *text, const char *Title, Font font,
             float fontSize, float spacing, Color textColor, Color borderColor,
             Color bgColor) {
  DrawRectangleRec(bounds, bgColor);
  DrawRectangleLinesEx(bounds, 1, borderColor);

  char wrappedText[2048];
  WrapTextToFit(text, wrappedText, font, fontSize, spacing,
                bounds.width - 10); // Padding

  Vector2 pos = {bounds.x + 5, bounds.y + 5};
  DrawTextEx(font, wrappedText, pos, fontSize, spacing, textColor);

  // Title
  float x = bounds.x + bounds.width / 2 -
            (float)MeasureText(Title, FontSizeHeader) / 2;
  float y = bounds.y - PanelHeight;
  DrawText(Title, x, y, FontSizeHeader, activeFontInactive);
  return;
}

// Function for Setting up Texts for individual Information
void SetTexts() {
  // Set Active Generator Information
  switch (Data.ActiveGenerator) {
  case 0:
    // HandCrank
    snprintf(GeneratorInfoText, sizeof(GeneratorInfoText),
             "Handcrank is a weird contraption. It looks like a rotating dong. "
             "Try to touch it... You won't regret it.");
    break;
  case 1:
    // Solar Panel
    snprintf(GeneratorInfoText, sizeof(GeneratorInfoText),
             "These strange panels generate electricity just by being exposed "
             "to light. "
             "I was eating some gummy bears, took a piss, and accidentally "
             "peed neon-like glowing urine onto the panel. "
             "I connected it to my phone — it charged to 100%.");
    break;
  }
  return;
}

#pragma region OTHER
// Function for Game Button
void EnableGameButton(Vector2 VectorPointer) {
  if (isGameButtonAlowed) {
    Rectangle GameBtn = {0, 0, ScreenWidth / NumOfUPT, PanelHeight};
    isHovering = CheckCollisionPointRec(VectorPointer, GameBtn);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    DrawRectangleRec(GameBtn, isHovering ? activeHover : activePanelBackground);

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

void GeneratingElectricity() {
  if (bat.actualCapacity >= bat.maxCapacity) {
    isGenerating = false;
    bat.actualCapacity = bat.maxCapacity;
    return;
  }

  bat.actualCapacity += gen.genPerSec / TargetFps;
  return;
}

void GeneratingMoney() {
  if (bat.actualCapacity <= 0.0f) {
    isSelling = false;
    bat.actualCapacity = 0.0f;
    return;
  }

  bat.actualCapacity -= mac.drain / TargetFps;
  Data.money += mac.output / TargetFps;

  return;
}

#pragma region Define Stuff
// Define Batteries
void DefineBatteries(Battery *bat) {
  // Variables for Defining batteries and easier choosement (Low Voltage)
  if (Data.voltageBat == 0) {
    char Names[][32] = {
        "Lithium-Ion",
        "Lead-Acid",
    };

    double Capacities[] = {1000.0, 1500.0};
    double Inputs[] = {0.5, 3.0};
    double Prices[] = {0.0, 10.0};

    // Formatting and Defining as itself
    strcpy(bat->name, Names[Data.ActiveBattery]);
    bat->maxCapacity = Capacities[Data.ActiveBattery];
    bat->maxInput = Inputs[Data.ActiveBattery];

    strcpy(bat->NextName, Names[Data.ActiveBattery + 1]);
    bat->NextMaxCap = Capacities[Data.ActiveBattery + 1];
    bat->NextMaxInput = Inputs[Data.ActiveBattery + 1];
    bat->price = Prices[Data.ActiveBattery + 1];
  } else if (Data.voltageBat == 1) {
    //
  } else if (Data.voltageBat == 2) {
    //
  }
  return;
}

// Define Generators
void DefineGenerators(Generator *gen) {
  // Variables for Defining generators and easier choosement
  char Names[][32] = {"HandCrank", "Solar Panel"};
  char Specials[][32] = {"Is Manual", "Needs Sun light"};

  double Generates[] = {0.1, 1.5};
  double Prices[] = {0.0, 500.0};
  int HasExtra[] = {0, 1};
  int NeedGas[] = {0, 0};

  // Formatting and Defining as itself
  strcpy(gen->name, Names[Data.ActiveGenerator]);
  strcpy(gen->Special, Specials[Data.ActiveGenerator]);
  if (Data.ActiveGenerator == 0) {
    gen->genPerClick = Generates[Data.ActiveGenerator];
  } else {
    gen->genPerSec = Generates[Data.ActiveGenerator];
  }
  gen->hasExtra = HasExtra[Data.ActiveGenerator];
  gen->needGas = NeedGas[Data.ActiveGenerator];

  strcpy(gen->NextName, Names[Data.ActiveGenerator + 1]);
  strcpy(gen->NextSpecial, Specials[Data.ActiveGenerator + 1]);
  gen->NextGen = Generates[Data.ActiveGenerator + 1];
  gen->price = Prices[Data.ActiveGenerator + 1];
  return;
}

// Define Machines
void DefineMachines(Machines *mac) {
  // Variables for Defining machines and easier choosement
  char Names[][32] = {"Smoke Detector", "LED LightBulb"};

  double Drainage[] = {1.0, 3.0};
  double RevenueMultiplier[] = {1.1, 1.15};
  double Prices[] = {0.0, 500.0};

  // Formatting and Defining as itself
  strcpy(mac->name, Names[Data.ActiveMachine]);
  mac->drain = Drainage[Data.ActiveMachine];
  mac->output = mac->drain * RevenueMultiplier[Data.ActiveMachine];

  strcpy(mac->NextName, Names[Data.ActiveMachine + 1]);
  mac->NextDrain = Drainage[Data.ActiveMachine + 1];
  mac->NextOutput = mac->NextDrain * RevenueMultiplier[Data.ActiveMachine + 1];
  mac->price = Prices[Data.ActiveMachine + 1];
  return;
}

#pragma region UPG Render
// Function for Rendering Battery Upgrade Screen
void RenderBatteryUpgradeScreen(Rectangle rec1, Rectangle rec2,
                                Rectangle rec3) {
  char buffer[128];
  float x = 0.0f;
  float y = 0.0f;

  // Render OutLine of Upgrades
  DrawRectangleLines(rec1.x, rec1.y, rec1.width, rec1.height, activeFontActive);
  DrawRectangleLines(rec2.x, rec2.y, rec2.width, rec2.height, activeFontActive);
  DrawRectangleLines(rec3.x, rec3.y, rec3.width, rec3.height, activeFontActive);

#pragma region Battery Low
  // First Segment: Header Text
  snprintf(buffer, sizeof(buffer), "Low Voltage");
  x = rec1.x + rec1.width / 2 - (float)MeasureText(buffer, FontSizeHeader) / 2;
  y = rec1.y - PanelHeight;
  DrawText(buffer, x, y, FontSizeHeader, activeFontInactive);

  if (Data.voltageBat > 0) {
    snprintf(buffer, sizeof(buffer), "Completed.");
    x = rec1.x + rec1.width / 2 -
        (float)MeasureText(buffer, FontSizeHeader) / 2;
    y = rec1.y + rec1.height / 2 - FontSizeHeader / 2;
    DrawText(buffer, x, y, FontSizeHeader, activeFontInactive);
  } else {
    // Next
    snprintf(buffer, sizeof(buffer), "Next:");
    x = rec1.x + Spacing;
    y = rec1.y + Spacing;
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);
    DrawLine(x, y + FontSizeText, rec1.x + rec1.width - Spacing,
             y + FontSizeText, activeFontInactive);

    // Next: Battery Name
    y += FontSizeHeader;
    snprintf(buffer, sizeof(buffer), "Battery: %s", bat.NextName);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Next: Battery Max Capacity
    y += FontSizeText;
    snprintf(buffer, sizeof(buffer), "Max Capacity: %0.2f Wh", bat.NextMaxCap);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Next: Max Input
    y += FontSizeText;
    snprintf(buffer, sizeof(buffer), "Max Input: %0.2f W/s", bat.NextMaxInput);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Now Using
    y = rec1.y + rec1.height / 2;
    snprintf(buffer, sizeof(buffer), "Now Using:");
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);
    DrawLine(x, y + FontSizeText, rec1.x + rec1.width - Spacing,
             y + FontSizeText, activeFontInactive);

    // Now Using: Name
    y += FontSizeHeader;
    snprintf(buffer, sizeof(buffer), "Battery %s", bat.name);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Now Using: Battery Max Capacity
    y += FontSizeText;
    snprintf(buffer, sizeof(buffer), "Max Capacity: %0.2f Wh", bat.maxCapacity);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Now Using: Max Input
    y += FontSizeText;
    snprintf(buffer, sizeof(buffer), "Max Input: %0.2f", bat.maxInput);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Price for first segment
    x = rec1.x + rec1.width / 2;
    y = rec1.y + rec1.height - PanelHeight * 2;
    snprintf(buffer, sizeof(buffer), "Price: %0.2f$", bat.price);
    DrawText(buffer, x - (float)MeasureText(buffer, FontSizeText) / 2, y,
             FontSizeText, activeFontActive);
  }

#pragma region Battery Medium
  // Second Segment: Header Text
  snprintf(buffer, sizeof(buffer), "Medium Voltage");
  x = rec2.x + rec2.width / 2 - (float)MeasureText(buffer, FontSizeHeader) / 2;
  y = rec2.y - PanelHeight;
  DrawText(buffer, x, y, FontSizeHeader, activeFontInactive);

  // Locks if not unlocked
  if (Data.voltageBat < 1) {
    x = rec2.x + rec2.width / 2;
    y = rec2.y + rec2.height / 2 - FontSizeHeader / 2;
    snprintf(buffer, sizeof(buffer), "Locked");
    DrawText(buffer, x - (float)MeasureText(buffer, FontSizeHeader) / 2, y,
             FontSizeHeader, activeFontInactive);
  } else if (Data.voltageBat > 1) {
    snprintf(buffer, sizeof(buffer), "Completed.");
    x = rec2.x + rec2.width / 2 -
        (float)MeasureText(buffer, FontSizeHeader) / 2;
    y = rec2.y + rec2.height / 2 - FontSizeHeader / 2;
    DrawText(buffer, x, y, FontSizeHeader, activeFontInactive);
  } else {
    // Next
    snprintf(buffer, sizeof(buffer), "Next:");
    x = rec2.x + Spacing;
    y = rec2.y + Spacing;
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);
    DrawLine(x, y + FontSizeText, rec2.x + rec2.width - Spacing,
             y + FontSizeText, activeFontInactive);

    // Next: Battery Name
    y += FontSizeHeader;
    snprintf(buffer, sizeof(buffer), "Battery: %s", bat.NextName);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Next: Battery Max Capacity
    y += FontSizeText;
    snprintf(buffer, sizeof(buffer), "Max Capacity: %0.2f Wh", bat.NextMaxCap);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Next: Max Input
    y += FontSizeText;
    snprintf(buffer, sizeof(buffer), "Max Input: %0.2f W/s", bat.NextMaxInput);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Now Using
    y = rec1.y + rec1.height / 2;
    snprintf(buffer, sizeof(buffer), "Now Using:");
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);
    DrawLine(x, y + FontSizeText, rec1.x + rec1.width - Spacing,
             y + FontSizeText, activeFontInactive);

    // Now Using: Name
    y += FontSizeHeader;
    snprintf(buffer, sizeof(buffer), "Battery %s", bat.name);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Now Using: Battery Max Capacity
    y += FontSizeText;
    snprintf(buffer, sizeof(buffer), "Max Capacity: %0.2f Wh", bat.maxCapacity);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Now Using: Max Input
    y += FontSizeText;
    snprintf(buffer, sizeof(buffer), "Max Input: %0.2f", bat.maxInput);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);
  }
#pragma region Battery High
  // Third Segment: Header Text
  snprintf(buffer, sizeof(buffer), "High Voltage");
  x = rec3.x + rec3.width / 2 - (float)MeasureText(buffer, FontSizeHeader) / 2;
  y = rec3.y - PanelHeight;
  DrawText(buffer, x, y, FontSizeHeader, activeFontInactive);

  // Locks if not unlocked
  if (Data.voltageBat < 2) {
    x = rec3.x + rec3.width / 2;
    y = rec3.y + rec3.height / 2 - FontSizeHeader / 2;
    snprintf(buffer, sizeof(buffer), "Locked");
    DrawText(buffer, x - (float)MeasureText(buffer, FontSizeHeader) / 2, y,
             FontSizeHeader, activeFontInactive);
  } else {
    // Next
    snprintf(buffer, sizeof(buffer), "Next:");
    x = rec3.x + Spacing;
    y = rec3.y + Spacing;
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);
    DrawLine(x, y + FontSizeText, rec3.x + rec3.width - Spacing,
             y + FontSizeText, activeFontInactive);

    // Next: Battery Name
    y += FontSizeHeader;
    snprintf(buffer, sizeof(buffer), "Battery: %s", bat.NextName);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Next: Battery Max Capacity
    y += FontSizeText;
    snprintf(buffer, sizeof(buffer), "Max Capacity: %0.2f Wh", bat.NextMaxCap);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Next: Max Input
    y += FontSizeText;
    snprintf(buffer, sizeof(buffer), "Max Input: %0.2f W/s", bat.NextMaxInput);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Now Using
    y = rec1.y + rec1.height / 2;
    snprintf(buffer, sizeof(buffer), "Now Using:");
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);
    DrawLine(x, y + FontSizeText, rec1.x + rec1.width - Spacing,
             y + FontSizeText, activeFontInactive);

    // Now Using: Name
    y += FontSizeHeader;
    snprintf(buffer, sizeof(buffer), "Battery %s", bat.name);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Now Using: Battery Max Capacity
    y += FontSizeText;
    snprintf(buffer, sizeof(buffer), "Max Capacity: %0.2f Wh", bat.maxCapacity);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);

    // Now Using: Max Input
    y += FontSizeText;
    snprintf(buffer, sizeof(buffer), "Max Input: %0.2f", bat.maxInput);
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);
  }
  return;
}

#pragma region Generator
void RenderGeneratorUpgradeScreen(Rectangle MainRec, Rectangle ExtraRec) {
  char buffer[1024];

  float x = 0.0f;
  float y = 0.0f;

  // Render OutLine of Upgrades
  DrawRectangleLines(MainRec.x, MainRec.y, MainRec.width, MainRec.height,
                     activeFontActive);
  DrawRectangleLines(ExtraRec.x, ExtraRec.y, ExtraRec.width, ExtraRec.height,
                     activeFontActive);

  // Main Upgrade Title
  snprintf(buffer, sizeof(buffer), "Upgrade:");
  x = MainRec.x + MainRec.width / 2 -
      (float)MeasureText(buffer, FontSizeHeader) / 2;
  y = MainRec.y - PanelHeight;
  DrawText(buffer, x, y, FontSizeHeader, activeFontInactive);

  // Next Generator Name
  snprintf(buffer, sizeof(buffer), "%s", gen.NextName);
  x = MainRec.x + Spacing;
  y = MainRec.y + Spacing;
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);

  // Generator Price
  snprintf(buffer, sizeof(buffer), "Price: %0.2f$", gen.price);
  x = MainRec.x + MainRec.width - (float)MeasureText(buffer, FontSizeText) -
      Spacing;
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);

  // Specials
  snprintf(buffer, sizeof(buffer), "Special: %s", gen.NextSpecial);
  x = MainRec.x + Spacing;
  y += FontSizeText;
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);

  // Extra Upgrade Title
  snprintf(buffer, sizeof(buffer), "Extra:");
  x = ExtraRec.x + ExtraRec.width / 2 -
      (float)MeasureText(buffer, FontSizeHeader) / 2;
  y = ExtraRec.y - PanelHeight;
  DrawText(buffer, x, y, FontSizeHeader, activeFontInactive);

  if (Data.ActiveGenerator == 1) {
    // Solar Panel
    snprintf(buffer, sizeof(buffer), "Additional Panel");
    x = ExtraRec.x + Spacing;
    y = ExtraRec.y + Spacing;
    DrawText(buffer, x, y, FontSizeText, activeFontInactive);
  } else {
    // If not Additional upgrades avalaible
    snprintf(buffer, sizeof(buffer), "N/A");
    x = ExtraRec.x + ExtraRec.width / 2 -
        (float)MeasureText(buffer, FontSizeHeader) / 2;
    y = ExtraRec.y + ExtraRec.height / 2;
    DrawText(buffer, x, y, FontSizeHeader, activeFontInactive);
  }

  // Now Using
  Rectangle NowUsing = {ScreenWidth / 3 + Spacing, PanelHeight * 3 + Spacing,
                        ScreenWidth / 3 - Spacing * 2,
                        ScreenHeight - PanelHeight * 4 - Spacing * 2};
  DrawRectangleLines(NowUsing.x, NowUsing.y, NowUsing.width, NowUsing.height,
                     activeFontActive);

  // Title
  snprintf(buffer, sizeof(buffer), "Now Using:");
  x = NowUsing.x + Spacing;
  y = NowUsing.y + Spacing;
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);
  DrawLine(x, y + FontSizeText, x + NowUsing.width - Spacing, y + FontSizeText,
           activeFontInactive);

  snprintf(buffer, sizeof(buffer), "%s", gen.name);
  y += FontSizeHeader;
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);

  // Generates
  if (Data.ActiveGenerator == 0) {
    snprintf(buffer, sizeof(buffer), "Generates: %0.2f W/click",
             gen.genPerClick);
  } else {
    snprintf(buffer, sizeof(buffer), "Generates: %0.2f W/s", gen.genPerSec);
  }
  y += FontSizeText;
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);

  // Specials
  snprintf(buffer, sizeof(buffer), "Specials: %s", gen.Special);
  y += FontSizeText;
  DrawText(buffer, x, y, FontSizeText, activeFontInactive);

  // Information
  Rectangle TextRec = {ScreenWidth - ScreenWidth / 3 + Spacing,
                       PanelHeight * 3 + Spacing, ScreenWidth / 3 - Spacing * 2,
                       ScreenHeight - PanelHeight * 4 - Spacing * 2};
  SetTexts();
  snprintf(buffer, sizeof(buffer), "Information:");
  TextBox(TextRec, GeneratorInfoText, buffer, GetFontDefault(), FontSizeText,
          3.0f, activeFontInactive, activeFontActive, activeBackground);
  return;
}