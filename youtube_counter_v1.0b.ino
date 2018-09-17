/*******************************************************************
 *  Read YouTube Channel statistics from the YouTube API           *
 *                                                                 *
 *  By Pavel V. Hladkikh aka EU4DGC                                * 
 *  http://qsy.by                                                  *
 *  https://www.youtube.com/channel/UCJSjwbCZ5R5CGY8qzjd203w       *  
 *                                                                 *
 *******************************************************************/

#include <YoutubeApi.h> // YoutubeApi библиотека доступа к API YouTube. Спасибо говорим @Witnessmeno 
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h> // Этот скетч технически не нужен, но без него не работает

//------- Заменить на свои значения ------
char ssid[] = "SSID";       // Имя точки доступа
char password[] = "PASSWORD";  // Пароль от точки доступа
#define API_KEY "API_KEY"  // Токен выданный Google 
#define CHANNEL_ID "ID_CANNEL" // ID YouTube канала
const char *t0h = "t0.txt=\""; // Решил не заморачиваться
const char *t1h = "t1.txt=\""; // и отправлять данные в порт
const char *t2h = "t2.txt=\""; // без использования библиотек для Nextion
byte ter[4] = {0x22, 0xff, 0xff, 0xff}; // Массив данных необходимый для работы с дисплеем

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long api_mtbs = 60000; // Время между запросами к API
unsigned long api_lasttime;   //Переменная последнего запроса к API

long subs = 0;

void setup() {

  Serial.begin(115200); // Скорость подключения к ESP для отладки проекта
  Serial1.begin(115200);// Скорость подключения к Nextion 
  Serial.flush();
  Serial1.flush();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Подключаемся к Wifi:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  
 }

void loop() {

  if (millis() - api_lasttime > api_mtbs)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      Serial.println("---------Статистика---------");
      Serial.print("Кол-во Подписчиков: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("Кол-во Просмотров: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Кол-во Комментариев: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Кол-во Видео: ");
      Serial.println(api.channelStats.videoCount);
      Serial.println("------------------------");
      Serial1.print(t0h);
      Serial1.println(api.channelStats.subscriberCount);
      Serial1.write(ter, 4);
      Serial1.print(t1h);
      Serial1.println(api.channelStats.viewCount);
      Serial1.write(ter, 4);
      Serial1.print(t2h);
      Serial1.println(api.channelStats.commentCount);
      Serial1.write(ter, 4);
       
    }
    api_lasttime = millis();
  }
}
