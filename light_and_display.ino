#include <Wire.h>
#include <BH1750FVI.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

const boolean displayMode = true;

uint8_t ADDRESSPIN = 27;
BH1750FVI::eDeviceAddress_t DEVICEADDRESS = BH1750FVI::k_DevAddress_H;
BH1750FVI::eDeviceMode_t DEVICEMODE = BH1750FVI::k_DevModeContHighRes;

BH1750FVI LightSensor(ADDRESSPIN, DEVICEADDRESS, DEVICEMODE);

void setup() 
{
  Serial.begin(9600);
  delay(1000);
  
  LightSensor.begin();
  
  lcd.init();
  lcd.backlight();
}

void loop()
{
  uint16_t lux = LightSensor.GetLightIntensity();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Light: ");
  lcd.print(lux);

  Serial.print("Light: "); 
  Serial.println(lux);

  delay(3000);
}
