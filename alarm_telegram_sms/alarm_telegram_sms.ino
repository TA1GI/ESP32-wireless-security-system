/*******************************************************************
    A Telegram App bot for ESP8266 that controls the 
    433 MHz home alarm system.

 *******************************************************************/

// The version of ESP8266 core needs to be 2.5 or higher
// or else your bot will not connect.

// ----------------------------
// Standard ESP8266 Libraries
// ----------------------------
#include <WiFi.h>//for ESP32
#include <WiFiClientSecure.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------
#include <RCSwitch.h>// https://github.com/sui77/rc-switch
#include <UniversalTelegramBot.h> //https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot

//Sirenin çalmaya devam edeceği süre (ms)
#define sirenCalmaSuresi 30000

//Mesaj gönderilecek ve aranacak telefon numarası
const String PHONE = "+905555555555";

// Initialize Wifi connection to the router
char ssid[] = "XXX";     // your network SSID (name)
char password[] = "12345678"; // your network key

// Initialize Telegram BOT
#define BOTtoken "xxx" // your Bot Token (Get from Botfather)
String chat_id = "111111"; // your telegram user id (to chat with)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

RCSwitch rfReceiver = RCSwitch();

//Checks for new messages every 1 second.
int botRequestDelay = 5000; //1000; //[ms]
unsigned long lastTimeBotRan;
unsigned long sirenzaman=0;

const int rfReceiverPin = 18; //pin D2, works. Connect RFReceiver module to this pin. Power it from 3V3 (not from 5V because of risk of killing GPIO pin).
const int rfTransmitPin = 10; //rf alıcısının pini

typedef struct {
  int remoteID;
  String remoteName;
  int group; // 0 - disarming; 1 - arming; 2 - reported 24h, 3 - reported when armed
  long lastTime;
} device_struct;

//Siren için değişken tanımlıyoruz
#define siren 2

//Durum ledleri için değişken tanımlıyoruz
#define sistemAktifLed 12
#define sistemPasifLed 13

#define NUM_DEVICES 10 //<<< DİKKAT!! UNUTMA Cihaz sayısı
device_struct devices[NUM_DEVICES] = { // add your device RF codes and names here
  
  {000,  "1 nolu kumanda ile sistem DEVRE DISI birakildi", 0},
  {000,  "1 nolu kumanda ile sistem AKTIF hale getirildi", 1},
  {000,  "1 nolu kumanda ile flash sinyali gönderildi", 2},
  {000,  "1 nolu kumanda ile ACIL YARDIM butonuna basildi", 2},
  
  {000,   "Su seviyesi yuksek", 2},
  {000,   "Su seviyesi orta", 2},
  {000,   "Su seviyesi dusuk", 2},
  
  {000, "1 Nolu Kapi Acildi", 3},
  {000, "Misafir odasinda hareket algilandi", 3},
  
  {000, "Siren susturuldu", 4},
  
}; //make the last entry above without a column

// Variable to store text message
String textMessage;

bool enable_system = true; //start armed
bool enable_sniffing = false;

int idSearch(int remoteID) {
  for (int i = 0; i < NUM_DEVICES; i++) {
    if (devices[i].remoteID == remoteID)
      return i;
  }
  return -1;
}

void parseRemote(int remoteID) {
  int currentDevice = idSearch(remoteID);
  if (currentDevice >= 0 && currentDevice < NUM_DEVICES ) {
    if (millis() - devices[currentDevice].lastTime > 1500) {
      devices[currentDevice].lastTime = millis();
      Serial.println(String(remoteID) + ": " + devices[currentDevice].remoteName);
      
      if ((devices[currentDevice].group == 0) && enable_system){
        enable_system = false;
        digitalWrite(sistemAktifLed, LOW);
        digitalWrite(sistemPasifLed, HIGH);
        digitalWrite(siren, LOW);
        bot.sendMessage(chat_id, devices[currentDevice].remoteName, "");
        //*****Sistem kumanda ile pasif duruma getirildiğinde sms göndermek için ayarlama yapıyoruz.(Bunu istemiyorsanız bu bölümü silin)
        String message = devices[currentDevice].remoteName;
        sendSMS(message);
        //*****
      }

      if (devices[currentDevice].group == 1){
        enable_system = true;
        digitalWrite(sistemAktifLed, HIGH);
        digitalWrite(sistemPasifLed, LOW);
        bot.sendMessage(chat_id, devices[currentDevice].remoteName, "");
      }
        
      if (devices[currentDevice].group == 2){
        bot.sendMessage(chat_id, devices[currentDevice].remoteName, "");
        String message = devices[currentDevice].remoteName;
        sendSMS(message);
      }
      
      if ((devices[currentDevice].group == 3) && enable_system){
        sirenzaman = millis();
        digitalWrite(siren, HIGH);
        bot.sendMessage(chat_id, devices[currentDevice].remoteName, "");
        String message = devices[currentDevice].remoteName;
        sendSMS(message);
        delay(3000);
        
        Serial2.println("ATD+ "+PHONE+";");
        //Serial2.println("ATH");
        // Set module to send SMS data to serial out upon receipt 
        Serial2.print("AT+CNMI=2,2,0,0,0\r");
      }
      
      if (devices[currentDevice].group == 4)
        digitalWrite(siren, LOW);
      
    }
  } else {
    Serial.println(String(remoteID) + ": " + "bilinmeyen kod");
    if (enable_sniffing)
      bot.sendMessage(chat_id, String(remoteID) + ": " + "bilinmeyen kod", ""); //use this to get the sensor id codes 
  }
}

void handleNewMessages(int numNewMessages) {
  //Serial.println("handleNewMessages");
  //Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    //String chat_id = String(bot.messages[i].chat_id);

    //String from_name = bot.messages[i].from_name;
    //if (from_name == "") from_name = "Guest";

    if (bot.messages[i].chat_id != chat_id) { // process commands only from me
      //bot.sendMessage(bot.messages[i].chat_id, "You are not authorized", "");
      return;
    }
    
    String text = bot.messages[i].text;
    
    if (text == "/aktif") {
      enable_system = true;
      digitalWrite(sistemAktifLed, HIGH);
      digitalWrite(sistemPasifLed, LOW);
      bot.sendMessage(chat_id, "Sistem Devrede", "");
    }

    else if (text == "/pasif") {
      enable_system = false;
      digitalWrite(sistemAktifLed, LOW);
      digitalWrite(sistemPasifLed, HIGH);
      bot.sendMessage(chat_id, "Sistem Devre Dışı", "");
    }
    
    else if (text == "/durum") {
      if(enable_system){
        bot.sendMessage(chat_id, "Durum: Sistem Aktif", "");
      } else {
        bot.sendMessage(chat_id, "Durum: Sistem Devre Dışı", "");
      }
    }
    
    else if (text == "/liste") {
      String list = "Cihaz kodları, isimleri ve grupları:\n";
      for (int currentDevice = 0; currentDevice < NUM_DEVICES; currentDevice++ ) {
        list += String(devices[currentDevice].remoteID) + ": " + devices[currentDevice].remoteName + " (" + String(devices[currentDevice].group) + ")\n";
      }
      bot.sendMessage(chat_id, list, "");
    }
    
    else if (text == "/dinle") {
      enable_sniffing = !enable_sniffing;
      if(enable_sniffing){
        bot.sendMessage(chat_id, "RF algılama açık", "");
      } else {
        bot.sendMessage(chat_id, "RF algılama kapalı", "");
      }
    }

    else if ((text == "/baslat") || (text == "/yardim")) {
      //String welcome = "Welcome, " + from_name + ".\n";
      String welcome = "Komutlar:\n";
      welcome += "/aktif : Sistemi Devreye Alma\n";
      welcome += "/pasif : Sistemi Devreden Çıkartma\n";
      welcome += "/durum : Sistem Durumu\n";
      welcome += "/liste : Bilinen tüm cihazları listeler \n";
      welcome += "/dinle : RF kodları dinleme-algılama modunu değiştirmek için\n";
      welcome += "/baslat veya /yardim : Komutları göstermek için\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void setup() {
  //Esp32 ile SIM800 seri haberleşmesi başlatılıyor
  Serial.begin(115200);
  Serial2.begin(9600);

  rfReceiver.enableReceive(rfReceiverPin);
  rfReceiver.enableTransmit(rfTransmitPin);

  // Sim800ün çalışması için 5 saniye bekliyoruz(Bunu yapmazsak aşağıdaki sim800ü sms moduna alma işlemi gerçekleşmez)
  delay(5000);

  // AT command to set SIM900 to SMS mode
  Serial2.print("AT+CMGF=1\r"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  Serial2.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
  
  //Siren pininin çıkış pini olduğunu belirtiyoruz
  pinMode (siren, OUTPUT);

  //Durum ledlerini ayarlıyoruz
  pinMode (sistemAktifLed, OUTPUT);
  pinMode (sistemPasifLed, OUTPUT);
  digitalWrite(sistemAktifLed, HIGH);
  digitalWrite(sistemPasifLed, LOW);

  delay(10);

  // This is the simplest way of getting this working
  // if you are passing sensitive information, or controlling
  // something important, please either use certStore or at
  // least client.setFingerPrint
  client.setInsecure();

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
  Serial.print("Wifi Bağlanıyor: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi bağlandı");
  Serial.print("IP adres: ");
  Serial.println(WiFi.localIP());


  // Sim800ün çalışması için 4 saniye bekliyoruz(Bunu yapmazsak aşağıdaki sim800ü sms moduna alma işlemi gerçekleşmez)
  delay(4000);

  // AT command to set SIM900 to SMS mode
  Serial2.print("AT+CMGF=1\r"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  Serial2.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);

  bot.sendMessage(chat_id, "Sistem Başlatıldı", "");
}

void loop() {
  //Sim800den seri monitöre komut göndermek için ayarlama yapıyoruz. Bu arada textMessage değişkenini sim800den gelen komut ile sabitliyoruz
  if(Serial2.available()>0){
    textMessage = Serial2.readString();
    Serial.print(textMessage);
  }

  //Seri monitörden sim800e komut göndermek için ayarlıyoruz. Bu kod test aşamasında gerekli. Daha sonra kaldıralım
  if(Serial.available()) {
    Serial2.write(Serial.read()); 
  }

  //Sms ile sistemi açmak için ayarlıyoruz
  if((textMessage.indexOf("#AC")>=0) && !enable_system){ //Gönderilecek sms içeriği # dahil
    enable_system = true;
    digitalWrite(sistemAktifLed, HIGH);
    digitalWrite(sistemPasifLed, LOW);
    String message = "Sistem Sms ile Aktif Duruma Getirildi";
    sendSMS(message);
    textMessage = "";   
  }

  //Sms ile sistemi kapatmak için ayarlıyoruz
  if((textMessage.indexOf("#KAPAT")>=0) && enable_system){ //Gönderilecek sms içeriği # dahil
    enable_system = false;
    digitalWrite(sistemAktifLed, LOW);
    digitalWrite(sistemPasifLed, HIGH);
    digitalWrite(siren, LOW);
    String message = "Sistem Sms ile Pasif Duruma Getirildi";
    sendSMS(message);
    textMessage = ""; 
  }

  //Sms ile sistemin durumunu sorguluyoruz
  if(textMessage.indexOf("#DURUM")>=0){ //Gönderilecek sms içeriği # dahil
    if(enable_system){
      String message = "Durum: Sistem Aktif";
      sendSMS(message);
      textMessage = "";
    } else {
      String message = "Durum: Sistem Devre Disi";
      sendSMS(message);
      textMessage = "";
    }
  }

  //Sms ile 1. kepenke komut gönderiyoruz
  if(textMessage.indexOf("#KEPENK1")>=0){ //Gönderilecek sms içeriği # dahil
    rfReceiver.send("000"); //Kepenk için gönderilecek rf kodunu girin
    String message = "SMS iLE 1. KEPENKE KOMUT GONDERiLDi.";
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }

  //Sms ile 2. kepenke komut gönderiyoruz
  if(textMessage.indexOf("#KEPENK2")>=0){ //Gönderilecek sms içeriği # dahil
    rfReceiver.send("000"); //Kepenk için gönderilecek rf kodunu girin
    String message = "SMS iLE 2. KEPENKE KOMUT GONDERiLDi.";
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }

  //Sms ile 3. kepenke komut gönderiyoruz
  if(textMessage.indexOf("#KEPENK3")>=0){ //Gönderilecek sms içeriği # dahil
    rfReceiver.send("000"); //Kepenk için gönderilecek rf kodunu girin
    String message = "SMS iLE 3. KEPENKE KOMUT GONDERiLDi.";
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }

  if (millis() - sirenzaman > sirenCalmaSuresi){
    digitalWrite(siren, LOW);
  }

  // listen to rfReceiver
  if (rfReceiver.available()) {
    parseRemote(rfReceiver.getReceivedValue());
    rfReceiver.resetAvailable();
  }

  // check for new Telegram messages
  // This seems to take a second, the rfReception seems to be not listerning for that busy time, it is missing RF codes.
  // I do not have a solution for this problem. A radical solution is not to check messages at all, as in "SimleAlarmSystem" project.
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    //while(numNewMessages) { //AMA commented this out, check only one message per loop cycle
      //Serial.println("got response");
      handleNewMessages(numNewMessages);
      //numNewMessages = bot.getUpdates(bot.last_message_received + 1); //AMA commented this out, process only one message per turn
    //}
    lastTimeBotRan = millis();
  }
}

// Function that sends SMS
void sendSMS(String message){
  // AT command to set SIM900 to SMS mode
  Serial2.println("AT+CMGF=1\r\n"); 
  delay(100);

  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  Serial2.println("AT+CMGS=\""+PHONE+"\"\r\n"); 
  delay(500);
  
  // Send the SMS
  Serial2.println(message); 
  delay(500);

  // End AT command with a ^Z, ASCII code 26
  Serial2.println((char)26);
}
