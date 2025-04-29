#include "raylib.h"
#include <stdio.h>
#include <string.h>

typedef struct {
  // Variables
  double whPerSecond;
  double whPerClick;
  double money;
  double moneyPerWh;

  // Generator Variables
  int activeGenerator;
  int numOfPowerCells;
  int lvlOfGenerator;
  double paidGeneratorUpgrade;

  // Battery Variables
  int activeBattery;
  double batteryCharge;
  int lvlOfBattery;
  double paidBatteryUpgrade;

} GameData;

// Battery Variables
typedef struct {
  char name[30];
  double capacity;
  double currentCapacity;
  double maxChargeRate;
  double defectRate;
  double life;
  double price;
  double multiplier;

  double lvlUpPrice;

  double chargeInPercent;
} Battery;

// Generator Variables
typedef struct {
  char name[30];
  double generatesPerSecond;
  double damage;
  double efficiency;

  double lvlUpPrice;
} Generator;

void setBattery(Battery *battery, int selected);
void setGenerator(Generator *generator, GameData *data, int selected);

int main(void) {
  // Const Variables
  const double screenWidth = 1280.0;
  const double screenHeight = 720.0;
  const int targetFps = 60;
  char title[] = "SS-OREH";

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
  GameData data = {0}; // GAMEDATA
  data.whPerClick = 0.5;
  data.moneyPerWh = 1.0;

  Battery battery = {0}; // BATTERY
  data.activeBattery = 1;
  data.lvlOfBattery = 0;
  battery.multiplier = 1.6;
  battery.lvlUpPrice = 0.0;
  setBattery(&battery, 1);

  Generator generator = {0}; // GENERATOR
  data.activeGenerator = 1;
  data.lvlOfGenerator = 0.0;
  generator.efficiency = 1.0;
  generator.lvlUpPrice = 0.0;
  setGenerator(&generator, &data, data.activeGenerator);
  bool generating = false;

  // Color Pallete
  Color bgColor = BLACK;
  Color neonGreen = {57, 255, 20, 255};
  Color warning = {255, 10, 10, 255};

  // Next Upgrades
  char nextBatteryUpgrade[30];
  char nextGeneratorUpgrade[30];

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

      // Draw Basic Outlines
      DrawRectangleLines(6, 6, screenWidth - 12, screenHeight - 12, neonGreen);
      DrawRectangleLines(6, 6, screenWidth / 3, screenHeight - 12, neonGreen);
      DrawRectangleLines(6, 6, (screenWidth / 3) * 2 - 12, screenHeight - 12,
                         neonGreen);
      DrawRectangleLines(6, 6, screenWidth - 12, blockHeight + 3, neonGreen);
      DrawRectangleLines(6, 6, screenWidth / 3, screenHeight / 3, neonGreen);
      DrawRectangleLines(6, 6, screenWidth / 3, (screenHeight / 3) * 2,
                         neonGreen);
      DrawRectangleLines(6, 6, screenWidth - 12, blockHeight + 6, neonGreen);
      DrawLine(screenWidth / 3 + 6, screenHeight / 2 + blockHeight / 2,
               screenWidth - 6, screenHeight / 2 + blockHeight / 2, neonGreen);
      // Headers
      DrawText("Upgrades:", screenWidth / 2 - MeasureText("Upgrades:", 20) / 2,
               blockHeight / 2, 20, neonGreen);
      DrawText("Personal Upgrades&Control:",
               (screenWidth / 6) * 5 -
                   MeasureText("Personal Upgrades&Control:", 20) / 2,
               blockHeight / 2, 20, neonGreen);

      char moneyText[30];
      snprintf(moneyText, sizeof(moneyText), "Money $: %0.1f", data.money);
      DrawText(moneyText, screenWidth / 6 - MeasureText(moneyText, 20) / 2, 12,
               20, neonGreen);

#pragma region Exchange for Money
      Rectangle sellButton = {screenWidth - screenWidth / 3 - 1,
                              3 * 6 + blockHeight, screenWidth / 3 - 10,
                              (screenHeight / 2 - blockHeight / 2) / 4};
      isHovering = CheckCollisionPointRec(mousePoint, sellButton);
      isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
      DrawRectangleRec(sellButton, isHovering ? GRAY : BLACK);
      DrawRectangleLines(screenWidth - screenWidth / 3 - 1, 3 * 6 + blockHeight,
                         screenWidth / 3 - 10,
                         (screenHeight / 2 - blockHeight / 2) / 4, neonGreen);

      DrawText("Sell Energy",
               (screenWidth / 6) * 5 - MeasureText("Sell Energy", 20) / 2,
               4 * 6 + blockHeight, 20, neonGreen);

      char moneyPerWh[30];
      snprintf(moneyPerWh, sizeof(moneyPerWh), "Money per Wh: %0.1f$",
               data.moneyPerWh);
      DrawText(moneyPerWh,
               (screenWidth / 6) * 5 - MeasureText(moneyPerWh, 20) / 2,
               4 * 6 + blockHeight + 30, 20, neonGreen);

      if (data.batteryCharge > 0.0 && isClicked) {
        data.money += data.moneyPerWh * data.batteryCharge;
        data.batteryCharge -= data.batteryCharge;
      }

#pragma endregion

#pragma region Turn On/Off
      Rectangle onOffButton = {10, blockHeight + 16, (screenWidth / 3 - 10),
                               blockHeight * 2};
      isHovering = CheckCollisionPointRec(mousePoint, onOffButton);
      isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
      DrawRectangleRec(onOffButton, isHovering ? GRAY : BLACK);

      if (data.batteryCharge > battery.capacity) {
        data.batteryCharge = battery.capacity;
      }

      if (data.activeGenerator == 1) {
        DrawText("Rotate the Crank",
                 screenWidth / 6 - MeasureText("Rotate the Crank", 20) / 2,
                 blockHeight * 2, 20, neonGreen);
      }

      if (data.activeGenerator == 1 && isClicked) {
        data.batteryCharge += generator.generatesPerSecond;
      }

      if (isClicked && data.activeBattery != 1) {
      }
#pragma endregion

#pragma region Sunlight function
      char dayTimerText[30];
      snprintf(dayTimerText, sizeof(dayTimerText), "Sunlight %: %0.1f",
               dayPercentage);
      DrawText(dayTimerText,
               (screenWidth / 6) - (MeasureText(dayTimerText, 20) / 2),
               blockHeight / 2, 20, neonGreen);

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
#pragma endregion Inventory

#pragma endregion
#pragma region Battery

      // Formatting variables for text output.
      char batCapacityText[30];
      char batMaxChargeText[30];
      char batChargeText[30];
      char batChargePercentText[30];
      char batLifeText[30];

      snprintf(batCapacityText, sizeof(batCapacityText),
               "Max. Capacity: %0.0f Wh", battery.currentCapacity);
      snprintf(batMaxChargeText, sizeof(batMaxChargeText),
               "Max. Charge Rate: %0.2f Wh/s", battery.maxChargeRate);
      snprintf(batChargeText, sizeof(batChargeText), "Current Charge: %0.2f Wh",
               data.batteryCharge);
      snprintf(batChargePercentText, sizeof(batChargePercentText),
               "Current Charge in %: %0.2f", battery.chargeInPercent);
      snprintf(batLifeText, sizeof(batLifeText), "Battery Life in %: %0.1f",
               battery.life);

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
      if (battery.life < 25.0) {
        DrawText(batLifeText, 10, screenHeight / 3 + 170, 20, warning);
      } else {
        DrawText(batLifeText, 10, screenHeight / 3 + 170, 20, neonGreen);
      }

      // Calculations ============================
      // Percentage
      battery.chargeInPercent = 100.0 * (data.batteryCharge / battery.capacity);

      // Damaging the Battery
      if (data.activeGenerator == 1) {
        if (data.batteryCharge > battery.currentCapacity) {
          battery.currentCapacity -= data.whPerClick * battery.defectRate;
          data.batteryCharge = battery.currentCapacity;
        }
      }
      if (generating && data.batteryCharge >= battery.currentCapacity) {
        battery.life -= (data.whPerSecond * battery.defectRate);
      }

#pragma endregion
#pragma region Generator
      DrawText("Generator: ", 10, (screenHeight / 3) * 2 + 10, 20, neonGreen);
      DrawText(generator.name, 10 + MeasureText("Generator: ", 20),
               (screenHeight / 3) * 2 + 10, 20, neonGreen);
      if (data.activeGenerator == 1) {
        char whPerClickText[30];
        snprintf(whPerClickText, sizeof(whPerClickText),
                 "Generates per click: %0.2f Wh", data.whPerClick);
        DrawText(whPerClickText, 10, screenHeight / 3 * 2 + 50, 20, neonGreen);
      } else {
        DrawText("Generator: ", 10, (screenHeight / 3) * 2 + 10, 20, neonGreen);
        DrawText(generator.name, 10 + MeasureText("Generator: ", 20),
                 (screenHeight / 3) * 2 + 10, 20, neonGreen);
      }
#pragma endregion
#pragma region Upgrades
      // Setting Prices

      if (data.lvlOfBattery == 0) { // Basic Energy Storage Upgrade
        battery.lvlUpPrice = 10250.0;
        strcpy(nextBatteryUpgrade, "Mid-Tier Energy Upgrade");
      } else if (data.lvlOfBattery == 1) { // Mid-Tier Energy Storage Upgrade
        battery.lvlUpPrice = 75000.0;
        strcpy(nextBatteryUpgrade, "Advanced Energy Upgrade");
      } else if (data.lvlOfBattery == 2) { // Advanced Energy Storage Upgrade
        battery.lvlUpPrice = 120000.0;
        strcpy(nextBatteryUpgrade, "Premium Energy Upgrade");
      } else if (data.lvlOfBattery == 3) { // Premium Energy Storage Upgrade
        battery.lvlUpPrice = 1000000.0;
      }

      if (data.lvlOfGenerator == 0) {
        //
      }

      // Battery Upgrade
      // Variables
      Rectangle UpgradeBatteryButton = {
          (screenWidth / 3) + 8, 3 * 6 + blockHeight, screenWidth / 3 - 18,
          (screenHeight / 2 - blockHeight / 2) / 4};
      isHovering = CheckCollisionPointRec(mousePoint, UpgradeBatteryButton);
      isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
      DrawRectangleRec(UpgradeBatteryButton, isHovering ? GRAY : BLACK);
      DrawRectangleLines((screenWidth / 3) + 10, 3 * 6 + blockHeight,
                         screenWidth / 3 - 18,
                         (screenHeight / 2 - blockHeight / 2) / 4, neonGreen);

      // Function
      char lvlUpPriceText[30];
      snprintf(lvlUpPriceText, sizeof(lvlUpPriceText), "Price: %0.1f$",
               battery.lvlUpPrice);
      DrawText(nextBatteryUpgrade,
               (screenWidth / 2) - MeasureText(nextBatteryUpgrade, 20) / 2,
               (screenHeight / 2 - blockHeight / 2) / 4 + blockHeight * 0.25,
               20, neonGreen);
      DrawText(lvlUpPriceText,
               (screenWidth / 2) - MeasureText(lvlUpPriceText, 20) / 2,
               4 * 6 + blockHeight + 30, 20, neonGreen);
      if (data.lvlOfBattery == 0) {
        setBattery(&battery, 1);
        if (isClicked && data.money >= battery.lvlUpPrice) {
          data.money -= battery.lvlUpPrice;
          data.lvlOfBattery = 1;
        }
      }

#pragma endregion
    }

    EndDrawing();
  }
  CloseWindow();

  return 0;
}

#pragma region Setting Battery
void setBattery(Battery *battery, int selected) {
  switch (selected) {
  case 1:
    // Basic Battery (on start)
    strcpy(battery->name, "Lithium-Ion Battery");
    battery->capacity = 1000.0;
    battery->currentCapacity = battery->capacity;
    battery->maxChargeRate = 1.0;
    battery->defectRate = 5.0; // per 1Wh
    battery->life = 100.0;     // %
    break;
    // Basic storage Upgrade
  case 2:
    strcpy(battery->name, "Lead-Acid Battery");
    battery->capacity = 1500.0;
    battery->currentCapacity = battery->capacity;
    battery->maxChargeRate = 3.0;
    battery->defectRate = 1.0;
    battery->life = 100.0;

    battery->price = 1600.0; // In $
    break;
  // Mid-Tier Energy Storage
  case 3:
    strcpy(battery->name, "Supercharged Lithium-Ion Battery");
    battery->capacity = 5000.0;
    battery->currentCapacity = battery->capacity;
    battery->maxChargeRate = 5.0;
    break;
  case 4:
    strcpy(battery->name, "Flow Battery");
    battery->capacity = 10000.0;
    battery->currentCapacity = battery->capacity;
    battery->maxChargeRate = 8.0;
    break;
  // Advanced Energy Storage
  case 5:
    strcpy(battery->name, "Solid-State Battery");
    battery->capacity = 20000.0;
    battery->currentCapacity = battery->capacity;
    battery->maxChargeRate = 15.0;
    break;
  case 6:
    strcpy(battery->name, "SUpercapacitor");
    battery->capacity = 5000.0;
    battery->currentCapacity = battery->capacity;
    break;
  // Premium Energy Storage
  case 7:
    strcpy(battery->name, "Fusion Power Cell");
    battery->capacity = 100000.0;
    battery->currentCapacity = battery->capacity;
    battery->maxChargeRate = 50.0;
    battery->defectRate = 30.0;
    break;
  case 8:
    strcpy(battery->name, "Quantum Energy Vault");
    battery->capacity = 999999999999999;
    battery->currentCapacity = battery->capacity;
    battery->maxChargeRate = 100.0;
    break;
  }
}
#pragma endregion

#pragma region Setting Generators
void setGenerator(Generator *generator, GameData *data, int selected) {
  switch (selected) {
  case 1:
    strcpy(generator->name, "Hand Crank");
    generator->generatesPerSecond = 0.5; // Wh
    generator->efficiency = 1.0;
    break;
  // Basic Energy Generation
  case 2:
    strcpy(generator->name, "Solar Cells");
    generator->generatesPerSecond = (50.0 / 60 / 60); // In hours
    generator->damage = 5.0 / 60 / 60;
    break;
    strcpy(generator->name, "Small Wind Turbine");
    generator->generatesPerSecond = 1.0 * 60;
    generator->efficiency = 1.0;
    break;
  // Mid-Tier Energy Generation
  case 3:
    strcpy(generator->name, "Gasoline Generator");
    generator->generatesPerSecond = 5.0;
    generator->efficiency = 1.0;
  }
}
#pragma endregion