#include <Wire.h>
#include <BH1750FVI.h>

#include "Ambient.h"

#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP 120 // 秒単位で送信する間隔を設定

// ↓ ここで Wifi の設定
const char* ssid = "Wifi の識別番号"; (1)
const char* password = "Wifi のパスワード"; (1)

// ↓ チャンネルとライトキーを設定
const int channelId = Ambientのチャンネル番号; (2)
const char* writeKey = "Ambientのライトキー"; (2)

const boolean displayMode = true;

// Settings
uint8_t ADDRESSPIN = 27;
BH1750FVI::eDeviceAddress_t DEVICEADDRESS = BH1750FVI::k_DevAddress_H;
BH1750FVI::eDeviceMode_t DEVICEMODE = BH1750FVI::k_DevModeContHighRes;

// Create the Lightsensor instance
BH1750FVI LightSensor(ADDRESSPIN, DEVICEADDRESS, DEVICEMODE);

WiFiClient client;
Ambient ambient;

RTC_DATA_ATTR int bootCount = 0; // RTCスローメモリに変数を確保

boolean flag = false;

void setup()
{
  Serial.begin(9600);
  // 初回起動だけはシリアル初期化待ち
  if( bootCount == 0 ){
      delay(1000);
  }
  bootCount = 1;

  Serial.println("WiFi connecting....");
  WiFi.begin(ssid, password); //  Wi-Fiの初期化
  LightSensor.begin();  //光センサーの開始
  while (WiFi.status() != WL_CONNECTED) {  //  Wi-Fiアクセスポイントへの接続待ち
      WiFi.begin(ssid, password); //  Wi-Fiの初期化
      delay(1000);
  }
  Serial.println("WiFi connected!!!");
  ambient.begin(channelId, writeKey, &client);


  uint16_t lux = LightSensor.GetLightIntensity();
  Serial.print("Light: ");
  Serial.println(lux);

  /* 読み取れたかどうかのチェック */
  if (isnan(lux)) {
    Serial.println("Read failure!");
  } else {
    /* Ambient にデータを送信 */
    ambient.set(1, lux);
    ambient.send();

    Serial.println("Sended to Ambient");

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    esp_deep_sleep_start();
  }
}

void loop()
{
}
