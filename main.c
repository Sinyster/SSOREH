/*
  I am not a proffecional coder. This Project is more likely to learn C language and raylib documentation.
  It's just a try to develop new thinking and get some experience in non-dynamic code language as C is unlike C# (I hate it).
  So if anyone have any suggestions, message me, I'm open to new ideas that could make me smarter. Thanks
*/

#include "raylib.h"
#include <stdio.h>
#include <string.h>

/*
  This code is not optimised, but I'm trying to.
  Trying to make save files as small as possible.
*/

typedef struct {
  double money;
  int timeOfDay;

  // Battery
  int lvlOfBattery;
  double batteryCharge;
  int activeBattery;
  int UnlockedEnergyUpgrades;

  // Generator
  int lvlOfGenerator;
  int activeGenerator;
  int numberOfSolarCells;
  int UnlockedGenerUpgrades;

  // Stuff
  int lvlOfStuff;
  int activeStuff
} GameData;

typedef struct {
  char name[30];
  char next[45];

  int lvlUnlocked;
  double capacity;
  double health;
  double price;
  double upgradePrice;
  double maxChargePerSecond;
  float percentage;
} Battery;

typedef struct {
  char name[30];
  char next[45];

  int lvlUnlocked;
  double whPerClick;
  double whPerSecond;
  double price;
  double upgradePrice;
  float efficiency;

  double maxChargePerSecond;
} Generator;

typedef struct {
  char name[30];
  char next[45];

  int lvlUnlocked;
  double drainPerSecond;
  double moneyPerSecond;
  double price;
} Stuff;

void setGenerator(Generator *gen, int selected, int lvl);
void setBattery(Battery *bat, int selected, int lvl);
void setStuff(Stuff *stuff, int selected, int lvl);

int main(void) {
  // App Variables
  const double screenWidth = 1280.0;
  const double screenHeight = 720.0;
  const int targetFps = 60;
  const char title[] = "SS-OREH";

  // App Init
  InitWindow(screenWidth, screenHeight, title);
  SetTargetFPS(targetFps);
  // SetExitKey(KEY_NULL);

  // Scenes
  typedef enum { SCENE_MAIN, SCENE_UPGRADES } GameScenes;
  GameScenes currentScene = SCENE_MAIN;

  // Energy Upgrades
  typedef enum { E_BASE, E_BASIC_UPG, E_MIDTIER_UPG } BEU;
  BEU currentEnergyUpgrades = E_BASE;

  // Generation Upgrades
  typedef enum { G_BASE, G_BASIC_UPG, G_MIDTIER_UPG } BGU;
  BGU currentGenUpgrades = G_BASE;

  // Color Pallete
  Color bgColor = WHITE;
  Color bgColor2 = LIGHTGRAY;
  Color hoverBg = GRAY;
  Color headerColor = DARKGRAY;
  Color textColor = GRAY;
  Color linesColor = GRAY;

  Color red = RED;
  Color green = GREEN;

  // DEFAULT VARIABLES
  GameData data = {0};
  data.activeBattery = 1;
  data.activeGenerator = 1;
  data.activeStuff = 1;

  data.lvlOfBattery = 1;
  data.lvlOfGenerator = 1;
  data.lvlOfStuff = 1;

  data.UnlockedEnergyUpgrades = 1;
  data.UnlockedGenerUpgrades = 1;

  Battery bat = {0};
  setBattery(&bat, data.activeBattery, data.lvlOfBattery);

  Generator gen = {0};
  setGenerator(&gen, data.activeGenerator, data.lvlOfGenerator);

  Stuff stuff = {0};
  setStuff(&stuff, data.activeStuff, data.lvlOfStuff);

  // Panel Variables
  const int PanelHeight = 35;
  const int headerFontSize = 22;
  const int textFontSize = 20;

  const int buttonWidth = 250;
  const int buttonHeight = 75;

  // Game Variables;
  bool isHovering = false;
  bool isClicked = false;
  bool isSelling = false;

  static float dayTimer = 0.0f;
  bool isDay = true;
  float sunlight = 0.0f;
  const int dayMinute = 900;
  const int nightMinute = 540;

  // Random Bool function because I dont know how to make it other way
  bool boughtFirst = false;
  bool boughtSecond = false;
  bool boughtThird = false;
  bool boughtForth = false;

  while (!WindowShouldClose()) {
    // Base Functions for All Scenes
    Vector2 mousePoint = GetMousePosition();
    BeginDrawing();
    ClearBackground(bgColor);

    setBattery(&bat, data.activeBattery, data.lvlOfBattery);
    setGenerator(&gen, data.activeGenerator, data.lvlOfGenerator);
    setStuff(&stuff, data.activeStuff, data.lvlOfStuff);

    if (data.batteryCharge <= 0.0) {
      data.batteryCharge = 0.0;
    }

    // Setting up which upgrades to draw
    if (data.UnlockedEnergyUpgrades == 1) {
      currentEnergyUpgrades = E_BASE;
    } else if (data.UnlockedEnergyUpgrades == 2) {
      currentEnergyUpgrades = E_BASIC_UPG;
    } else if (data.UnlockedEnergyUpgrades == 3) {
      currentEnergyUpgrades = E_MIDTIER_UPG;
    }

    if (data.UnlockedGenerUpgrades == 1) {
      currentGenUpgrades = G_BASE;
    } else if (data.UnlockedGenerUpgrades == 2) {
      currentGenUpgrades = G_BASIC_UPG;
    } else if (data.UnlockedGenerUpgrades == 3) {
      currentGenUpgrades = G_MIDTIER_UPG;
    }

#pragma region Upper Panel
    DrawRectangle(0, 0, screenWidth, PanelHeight, bgColor2);
    DrawLine(0, PanelHeight, screenWidth, PanelHeight, linesColor);
    DrawLine(screenWidth / 5, PanelHeight * 0.1, screenWidth / 5, PanelHeight,
             linesColor);
    DrawLine(screenWidth / 5 * 2, PanelHeight * 0.1, screenWidth / 5 * 2,
             PanelHeight * 0.9, linesColor);

    // First Part
    Rectangle mainSceneButton = {3, PanelHeight * 0.1, screenWidth / 5 - 6,
                                 PanelHeight * 0.8};
    isHovering = CheckCollisionPointRec(mousePoint, mainSceneButton);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    DrawRectangleRec(mainSceneButton, isHovering ? hoverBg : bgColor2);
    DrawText("Game",
             (screenWidth / 5) / 2 - MeasureText("Game", headerFontSize) / 2,
             PanelHeight / 2 - headerFontSize / 2, headerFontSize, headerColor);

    if (currentScene != SCENE_MAIN && isClicked) {
      currentScene = SCENE_MAIN;
    }
    // Second Part
    Rectangle upgradesButton = {screenWidth / 5 + 3, PanelHeight * 0.1,
                                screenWidth / 5 - 6, PanelHeight * 0.8};
    isHovering = CheckCollisionPointRec(mousePoint, upgradesButton);
    isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    DrawRectangleRec(upgradesButton, isHovering ? hoverBg : bgColor2);
    DrawText("Upgrades",
             screenWidth / 5 + (screenWidth / 5 / 2) -
                 MeasureText("Upgrades", headerFontSize) / 2,
             PanelHeight / 2 - headerFontSize / 2, headerFontSize, headerColor);

    if (isClicked && currentScene != SCENE_UPGRADES) {
      currentScene = SCENE_UPGRADES;
    }
    // Third Part
    // Forth Part
    // Fifth Part

#pragma endregion
#pragma region Lower Panel
    DrawRectangle(0, screenHeight - PanelHeight, screenWidth, PanelHeight,
                  bgColor2);
    DrawLine(0, screenHeight - PanelHeight, screenWidth,
             screenHeight - PanelHeight, linesColor);

    DrawLine(screenWidth / 5, (screenHeight - PanelHeight) + PanelHeight * 0.1,
             screenWidth / 5, (screenHeight - PanelHeight) + PanelHeight * 0.9,
             linesColor);
    DrawLine(screenWidth / 5 * 2,
             (screenHeight - PanelHeight) + PanelHeight * 0.1,
             screenWidth / 5 * 2,
             (screenHeight - PanelHeight) + PanelHeight * 0.9, linesColor);
    DrawLine(screenWidth / 5 * 3,
             (screenHeight - PanelHeight) + PanelHeight * 0.1,
             screenWidth / 5 * 3,
             (screenHeight - PanelHeight) + PanelHeight * 0.9, linesColor);
    // Drawing Money
    char moneyText[30];
    snprintf(moneyText, sizeof(moneyText), "$ %0.2f", data.money);
    DrawText(moneyText,
             (screenWidth / 5) / 2 - MeasureText(moneyText, headerFontSize) / 2,
             screenHeight - PanelHeight / 2 - headerFontSize / 2,
             headerFontSize, headerColor);

    // Drawing Sunlight %
    char sunlightText[30];
    snprintf(sunlightText, sizeof(sunlightText), "Sunlight %: %0.1f", sunlight);
    DrawText(sunlightText,
             (screenWidth / 5 * 2) - (screenWidth / 5 / 2) -
                 MeasureText(sunlightText, headerFontSize) / 2,
             screenHeight - PanelHeight / 2 - headerFontSize / 2,
             headerFontSize, headerColor);

    // Battery Charge
    char percentageText[30];
    snprintf(percentageText, sizeof(percentageText), "Charge in %: %0.2f",
             bat.percentage);
    DrawText(percentageText,
             screenWidth / 2 - MeasureText(percentageText, headerFontSize) / 2,
             screenHeight - PanelHeight / 2 - headerFontSize / 2,
             headerFontSize, headerColor);
#pragma endregion

    // ===========================================================================================================
#pragma region ===MAIN===
    if (currentScene == SCENE_MAIN) {
      // Editing Upper Panel
      DrawLine((screenWidth / 5) / 2 - MeasureText("Game", headerFontSize),
               PanelHeight * 0.9,
               (screenWidth / 5) / 2 + MeasureText("Game", headerFontSize),
               PanelHeight * 0.9, linesColor);

      // RENDER OF MAIN GAME PANEL

      DrawLine(screenWidth / 3, PanelHeight + PanelHeight * 0.1,
               screenWidth / 3, screenHeight - PanelHeight - PanelHeight * 0.1,
               linesColor);

      DrawLine(screenWidth / 3 * 2, PanelHeight + PanelHeight * 0.1,
               screenWidth / 3 * 2,
               screenHeight - PanelHeight - PanelHeight * 0.1, linesColor);

      // FIRST COLUMN
      // =======================================================================================================================================
      DrawText("Battery: ", PanelHeight, PanelHeight * 2, headerFontSize,
               textColor);

      DrawText(bat.name, PanelHeight + MeasureText("Battery: ", headerFontSize),
               PanelHeight * 2, headerFontSize, textColor);

      // Capacity
      char capacityText[30];
      snprintf(capacityText, sizeof(capacityText), "Max. Capacity: %0.0f Wh",
               bat.capacity);
      DrawText(capacityText, PanelHeight, PanelHeight * 3, textFontSize,
               textColor);

      // Max Charge
      char maxChargeText[30];
      snprintf(maxChargeText, sizeof(maxChargeText), "Max. Charge: %0.1f Wh/s",
               bat.maxChargePerSecond);
      DrawText(maxChargeText, PanelHeight, PanelHeight * 3 + textFontSize,
               textFontSize, textColor);

      // Actual Charge
      char chargeText[30];
      snprintf(chargeText, sizeof(chargeText), "Actual Charge: %0.1f Wh",
               data.batteryCharge);
      DrawText(chargeText, PanelHeight, PanelHeight * 4 + textFontSize,
               textFontSize, textColor);
      DrawText(percentageText, PanelHeight, PanelHeight * 4 + textFontSize * 2,
               textFontSize, textColor);

      // SECOND COLUMN
      // =======================================================================================================================================
      DrawText("Source: ", screenWidth / 3 + PanelHeight, PanelHeight * 2,
               headerFontSize, textColor);
      DrawText(gen.name,
               screenWidth / 3 + PanelHeight +
                   MeasureText("Source: ", headerFontSize),
               PanelHeight * 2, headerFontSize, textColor);
      char genPerClick[30];
      snprintf(genPerClick, sizeof(genPerClick),
               "Generates per Click: %0.1f Wh", gen.whPerClick);
      DrawText(genPerClick, screenWidth / 3 + PanelHeight, PanelHeight * 3,
               textFontSize, textColor);

      // THIRD COLUMN
      // =======================================================================================================================================
      DrawText("Powering: ", screenWidth / 3 * 2 + PanelHeight, PanelHeight * 2,
               headerFontSize, textColor);
      DrawText(stuff.name,
               screenWidth / 3 * 2 + PanelHeight +
                   MeasureText("Powering: ", headerFontSize),
               PanelHeight * 2, headerFontSize, textColor);

      char drainText[30];
      snprintf(drainText, sizeof(drainText), "Drains: %0.1f Wh/s",
               stuff.drainPerSecond);
      DrawText(drainText, screenWidth / 3 * 2 + PanelHeight, PanelHeight * 3,
               textFontSize, textColor);

      char gensText[30];
      snprintf(gensText, sizeof(gensText), "Gains: %0.1f $/s",
               stuff.moneyPerSecond);
      DrawText(gensText, screenWidth / 3 * 2 + PanelHeight,
               PanelHeight * 3 + textFontSize, textFontSize, textColor);

#pragma region Generation
      if (strcmp(gen.name, "Hand Crank") == 0) {
        Rectangle handCrank = {screenWidth / 2 - buttonWidth / 2,
                               screenHeight / 2, buttonWidth, buttonHeight};
        isHovering = CheckCollisionPointRec(mousePoint, handCrank);
        isClicked = isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        DrawRectangleRec(handCrank, isHovering ? GRAY : LIGHTGRAY);
        DrawText("Rotate the Crank",
                 screenWidth / 2 -
                     MeasureText("Rotate the Crank", headerFontSize) / 2,
                 screenHeight / 2 - headerFontSize / 2 + buttonHeight / 2,
                 headerFontSize, headerColor);
        if (isClicked) {
          data.batteryCharge += gen.whPerClick;
        }
      }

#pragma endregion

#pragma region Selling
      // Rendering
      Rectangle sellButton = {screenWidth / 6 * 5 - buttonWidth / 2,
                              screenHeight / 2, buttonWidth, buttonHeight};
      isHovering = CheckCollisionPointRec(mousePoint, sellButton);
      isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

      DrawRectangleRec(sellButton, isHovering ? hoverBg : bgColor2);

      if (isClicked) {
        if (data.batteryCharge > 0.0) {
          isSelling = isSelling ? false : true;
        }
      }

      // TURN ON/OFF TEXT
      if (isSelling) {
        DrawText("Turn Off",
                 (screenWidth / 6 * 5) -
                     MeasureText("Turn Off", headerFontSize) / 2,
                 screenHeight / 2 + buttonHeight / 2 - headerFontSize / 2,
                 headerFontSize, headerColor);
      } else {
        DrawText("Turn On",
                 (screenWidth / 6 * 5) -
                     MeasureText("Turn On", headerFontSize) / 2,
                 screenHeight / 2 + buttonHeight / 2 - headerFontSize / 2,
                 headerFontSize, headerColor);
      }

      // Sell Function
      if (isSelling) {
        if (data.batteryCharge > 0.0) {
          data.batteryCharge -= stuff.drainPerSecond / targetFps;
          data.money += stuff.moneyPerSecond / targetFps;
        } else {
          isSelling = false;
        }
      }

#pragma endregion

#pragma region Battery Calc

      // Battery Percentage
      bat.percentage = 100.0f * (data.batteryCharge / bat.capacity);

#pragma endregion

#pragma region Sunlight Func
      // Calculation
      if (isDay) {
        dayTimer += GetFrameTime();
        if (dayTimer < dayMinute / 2) {
          sunlight += 100.0f / (dayMinute / 2 * targetFps);
          if (sunlight > 100.0f)
            sunlight = 100.0f;
        } else if (dayTimer > dayMinute / 2) {
          sunlight -= 100.0f / (dayMinute / 2 * targetFps);
          if (sunlight < 0.1f) {
            sunlight = 0.1f;
            dayTimer = 0.0f;
            isDay = false;
          }
        }
      } else {
        dayTimer += GetFrameTime();
        if (dayTimer > nightMinute) {
          isDay = true;
          dayTimer = 0.0f;
        }
      }

#pragma endregion
#pragma region ===UPGR===
    } else if (currentScene == SCENE_UPGRADES) {
      // Basic Scene Render
      ClearBackground(bgColor);

      // Separation Lines
      DrawLine(screenWidth / 3, PanelHeight + PanelHeight * 0.1,
               screenWidth / 3, screenHeight - PanelHeight - PanelHeight * 0.1,
               linesColor);

      DrawLine(screenWidth / 3 * 2, PanelHeight + PanelHeight * 0.1,
               screenWidth / 3 * 2,
               screenHeight - PanelHeight - PanelHeight * 0.1, linesColor);

      // Editing Upper Panel
      DrawLine(screenWidth / 5 + (screenWidth / 5 / 2) -
                   MeasureText("Upgrades", headerFontSize),
               PanelHeight * 0.9,
               screenWidth / 5 + (screenWidth / 5 / 2) +
                   MeasureText("Upgrades", headerFontSize),
               PanelHeight * 0.9, linesColor);

      // Headers
      DrawText("Batteries",
               screenWidth / 6 - MeasureText("Batteries", headerFontSize) / 2,
               PanelHeight * 2, headerFontSize, headerColor);
      DrawText("Generators",
               screenWidth / 2 - MeasureText("Generators", headerFontSize) / 2,
               PanelHeight * 2, headerFontSize, headerColor);
      DrawText("Electric Machines",
               screenWidth / 6 * 5 -
                   MeasureText("Electric Machines", headerFontSize) / 2,
               PanelHeight * 2, headerFontSize, headerColor);

      // Drawing Upgrades
      // =========================================================================
      // Battery Section =========
      if (currentEnergyUpgrades == E_BASE) {
        DrawText("No Upgrades.",
                 screenWidth / 6 -
                     MeasureText("No Upgrades.", headerFontSize) / 2,
                 screenHeight / 2, headerFontSize, headerColor);
      } else if (currentEnergyUpgrades == E_BASIC_UPG) {
        Rectangle batUpgButton = {
            screenWidth / 6 - ((screenWidth / 3 - 12) / 2),
            PanelHeight * 4 + buttonHeight, screenWidth / 3 - 12, buttonHeight};
        isHovering = CheckCollisionPointRec(mousePoint, batUpgButton);
        isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        DrawRectangleRec(batUpgButton, isHovering ? hoverBg : bgColor2);
      }

      // Draw Battery Next Upgrade
      Rectangle batteryNextUpgrade = {
          screenWidth / 6 - ((screenWidth / 3 - 12) / 2), PanelHeight * 3,
          screenWidth / 3 - 12, buttonHeight};
      isHovering = CheckCollisionPointRec(mousePoint, batteryNextUpgrade);
      isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
      DrawRectangleRec(batteryNextUpgrade, isHovering ? hoverBg : bgColor2);

      DrawText(bat.next, screenWidth / 6 - ((screenWidth / 3 - 12) / 2) + 10,
               PanelHeight * 3 + 4, headerFontSize, headerColor);

      // Calculating and Drawing Price
      DrawText("Price: ", screenWidth / 6 - ((screenWidth / 3 - 12) / 2) + 10,
               PanelHeight * 3 + 6 + textFontSize, textFontSize, headerColor);
      DrawText("Unlocks new technology for storing energy.",
               screenWidth / 6 - ((screenWidth / 3 - 12) / 2) + 10,
               PanelHeight * 3 + 8 + textFontSize * 2, textFontSize * 0.9,
               headerColor);

      char nextBatPriceText[30];
      snprintf(nextBatPriceText, sizeof(nextBatPriceText), "%0.1f $",
               bat.upgradePrice);
      if (data.money > bat.upgradePrice) {
        DrawText(nextBatPriceText,
                 screenWidth / 6 - ((screenWidth / 3 - 12) / 2) + 10 +
                     MeasureText("Price: ", textFontSize),
                 PanelHeight * 3 + 6 + textFontSize, textFontSize, green);
      } else {
        DrawText(nextBatPriceText,
                 screenWidth / 6 - ((screenWidth / 3 - 12) / 2) + 10 +
                     MeasureText("Price: ", textFontSize),
                 PanelHeight * 3 + 6 + textFontSize, textFontSize, red);
      }

      // Buy
      if (isClicked && data.money >= bat.upgradePrice) {
        data.money -= bat.upgradePrice;
        data.lvlOfBattery = 2;
        data.UnlockedEnergyUpgrades += 1;
      }

      // RENDER BATTERY UPGRADES
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}

void setGenerator(Generator *gen, int selected, int lvl) {
  if (lvl == 1) {
    strcpy(gen->next, "Basic Energy Generation");
    gen->price = 2500.0;
  } else if (lvl == 2) {
    strcpy(gen->next, "Mid-Tier Energy Generation");
    gen->price = 15000.0;
  }

  // Setting Generators
  if (selected == 1 && lvl == 1) { // Hand Crank
    strcpy(gen->name, "Hand Crank");
    gen->whPerClick = 0.5;
  }
  if (selected == 2 && lvl == 2) { // Basic Energy Generation
    strcpy(gen->name, "Solar cells");
    gen->whPerSecond = 50.0 / 60.0 / 60.0;
  }
  if (selected == 3 && lvl == 3) {
    strcpy(gen->name, "Gasoline Engine");
    gen->whPerSecond = 5.0;
  }
}
void setBattery(Battery *bat, int selected, int lvl) {
  if (lvl == 1) {
    strcpy(bat->next, "Basic Energy Storage");
    bat->upgradePrice = 1.0;
  } else if (lvl == 2) {
    strcpy(bat->next, "Mid-Tier Energy Storage");
    bat->upgradePrice = 100000.0;
  }

  // Setting Batteries
  if (selected == 1 && lvl == 1) { // Basic Battery from Start
    strcpy(bat->name, "Lithium-Ion Battery");
    bat->capacity = 1000.0;
    bat->maxChargePerSecond = 1.0;
  }
  if (selected == 2 && lvl == 2) { // Basic Energy Storage
    strcpy(bat->name, "Lead-Acid Battery");
    bat->capacity = 1500.0;
    bat->maxChargePerSecond = 3.0;
  }
}
void setStuff(Stuff *stuff, int selected, int lvl) {
  if (lvl == 1) {
    strcpy(stuff->next, "Portable Fan");
    stuff->price = 500.0;
  }
  if (lvl == 2) {
    //
  }

  // Setting Stuff
  if (selected == 1 && lvl == 1) { // Light Bulb
    strcpy(stuff->name, "Light Bulb");
    stuff->drainPerSecond = 0.2; // Wh
    stuff->moneyPerSecond = 0.1; // Wh
  }
  if (selected == 2 && lvl == 2) {
    strcpy(stuff->name, "Portable Fan");
    stuff->drainPerSecond = 0.4;
    stuff->moneyPerSecond = 0.3;
  }
}