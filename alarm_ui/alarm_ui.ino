#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "FS.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------
#include <RCSwitch.h>// https://github.com/sui77/rc-switch
#include <UniversalTelegramBot.h> //https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "password";
const char* PARAM_INPUT_3 = "ip";
const char* PARAM_INPUT_4 = "gateway";
const char* PARAM_INPUT_5 = "sirenTime";
const char* PARAM_INPUT_6 = "callNumber";
const char* PARAM_INPUT_7 = "smsNumber";
const char* PARAM_INPUT_8 = "telegramToken";
const char* PARAM_INPUT_9 = "chat_id";
const char* PARAM_INPUT_10 = "output";
const char* PARAM_INPUT_11 = "state";
const char* PARAM_INPUT_12 = "apPass";
const char* PARAM_INPUT_13 = "uiName";
const char* PARAM_INPUT_14 = "uiPass";
const char* PARAM_INPUT_15 = "smsSysEnable";
const char* PARAM_INPUT_16 = "smsSysDisable";
//sms control
const char* PARAM_INPUT_17 = "ctrlMessage1";
const char* PARAM_INPUT_18 = "ctrlMessage2";
const char* PARAM_INPUT_19 = "ctrlMessage3";
const char* PARAM_INPUT_20 = "ctrlMessage4";
const char* PARAM_INPUT_21 = "ctrlMessage5";
const char* PARAM_INPUT_22 = "ctrlMessage6";
const char* PARAM_INPUT_23 = "ctrlMessage7";
const char* PARAM_INPUT_24 = "ctrlMessage8";
const char* PARAM_INPUT_25 = "ctrlMessage9";
const char* PARAM_INPUT_26 = "ctrlMessage0";

const char* PARAM_INPUT_27 = "sendMessage1";
const char* PARAM_INPUT_28 = "sendMessage2";
const char* PARAM_INPUT_29 = "sendMessage3";
const char* PARAM_INPUT_30 = "sendMessage4";
const char* PARAM_INPUT_31 = "sendMessage5";
const char* PARAM_INPUT_32 = "sendMessage6";
const char* PARAM_INPUT_33 = "sendMessage7";
const char* PARAM_INPUT_34 = "sendMessage8";
const char* PARAM_INPUT_35 = "sendMessage9";
const char* PARAM_INPUT_36 = "sendMessage0";

const char* PARAM_INPUT_37 = "sendRf1";
const char* PARAM_INPUT_38 = "sendRf2";
const char* PARAM_INPUT_39 = "sendRf3";
const char* PARAM_INPUT_40 = "sendRf4";
const char* PARAM_INPUT_41 = "sendRf5";
const char* PARAM_INPUT_42 = "sendRf6";
const char* PARAM_INPUT_43 = "sendRf7";
const char* PARAM_INPUT_44 = "sendRf8";
const char* PARAM_INPUT_45 = "sendRf9";
const char* PARAM_INPUT_46 = "sendRf0";

//Variables to save values from HTML form
const char* ssid;
const char* password;
const char* ip;
const char* gateway;
const char* sirenTime;
const char* callNumber;
const char* smsNumber;
const char* telegramToken;
const char* chat_id;
const char* sysState;
const char* smsSysCtrl;
const char* smsSysEnable;
const char* smsSysDisable;
const char* smsCtrl;
String remoteIdGet;
const char* callState;
const char* smsState;
const char* telegramState;
const char* apPass;
const char* uiName;
const char* uiPass;
//sms control
const char* ctrlMessage1;
const char* ctrlMessage2;
const char* ctrlMessage3;
const char* ctrlMessage4;
const char* ctrlMessage5;
const char* ctrlMessage6;
const char* ctrlMessage7;
const char* ctrlMessage8;
const char* ctrlMessage9;
const char* ctrlMessage0;
const char* sendMessage1;
const char* sendMessage2;
const char* sendMessage3;
const char* sendMessage4;
const char* sendMessage5;
const char* sendMessage6;
const char* sendMessage7;
const char* sendMessage8;
const char* sendMessage9;
const char* sendMessage0;
const char* sendRf1;
const char* sendRf2;
const char* sendRf3;
const char* sendRf4;
const char* sendRf5;
const char* sendRf6;
const char* sendRf7;
const char* sendRf8;
const char* sendRf9;
const char* sendRf0;

unsigned long sirenSure;

WiFiClientSecure client;
UniversalTelegramBot bot(telegramToken, client);

//Checks for new messages every 1 second.
int botRequestDelay = 5000; //1000; //[ms]
unsigned long lastTimeBotRan;
unsigned long sirenzaman = 0;

RCSwitch rfReceiver = RCSwitch();

const int rfReceiverPin = 18; //pin D2, works. Connect RFReceiver module to this pin. Power it from 3V3 (not from 5V because of risk of killing GPIO pin).
const int rfTransmitterPin = 19;

//Durum ledleri için değişken tanımlıyoruz
#define sistemAktifLed 12
#define sistemPasifLed 13

//Siren için değişken tanımlıyoruz
#define siren 2

// Variable to store text message
String textMessage;

typedef struct {
  int remoteID;
  String remoteName;
  int group; // 0 - disarming; 1 - arming; 2 - reported 24h, 3 - reported when armed
  long lastTime;
} device_struct;

#define NUM_DEVICES 33 //<<<
device_struct devices[NUM_DEVICES] = { // add your device RF codes and names here

  {11818849,  "3 nolu kumanda ile sistem pasif duruma getirildi", 0},
  {11818850,  "3 nolu kumanda ile sistem aktif duruma getirildi", 1},
  {11818852,  "RC1 flash", 2},
  {000,  "RC1 SOS", 2},
  {000,  "RC2 disarm", 0},
  {000,  "RC2 arm", 1},
  {000,  "RC2 flash", 2},
  {000,  "RC2 SOS", 2},
  {000,  "RC3 disarm", 0},
  {000,  "RC3 arm", 1},
  {000,  "RC3 flash", 2},
  {000,  "RC3 SOS", 2},
  {000, "RC4 disarm", 0},
  {000, "RC4 arm", 1},
  {000, "RC4 flash", 2},
  {000, "RC4 SOS", 2},

  {000,   "26 water spare", 2},
  {000,   "25 water EG", 2},
  {000, "24 water UG", 2},

  {000,  "23 smoke UG", 2},
  {000,  "22 smoke EG", 2},
  {000,  "21 smoke UG", 2},
  {000,  "20 smoke EG", 2},
  {000,  "19 smoke OG", 2},
  {000, "18 smoke OG", 2},
  {000,  "17 smoke OG", 2},
  {000, "16 smoke DG", 2},
  {000,  "15 smoke DG", 2},
  {000, "14 smoke DG", 2},

  {11818856, "haraket algilandi", 3},
  {000,  "12 motion spare", 3}, //danger of false alarms (foreign sensor)
  {000,  "11 motion OG", 3},
  {000,  "10 motion EG", 4}

}; //make the last entry above without a column

bool enable_system; //start armed
bool enable_sniffing = false;

DynamicJsonDocument config(1024);
DynamicJsonDocument smsconfig(1024);
DynamicJsonDocument smsconfig2(1024);
void readJSON(fs::FS &fs, const char * path, DynamicJsonDocument &doc) {
  Serial.printf("Reading Json: %s\r\n", path);
  String output;

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    char intRead = file.read();
    output += intRead;
  }
  deserializeJson(doc, output);
  file.close();
}

// Write file to LittleFS
void writeFile(fs::FS &fs, const char * path, DynamicJsonDocument &doc) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  String output;
  serializeJson(doc, output);
  if (file.print(output)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

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

      if ((devices[currentDevice].group == 0) && enable_system) {
        enable_system = false;
        digitalWrite(sistemAktifLed, LOW);
        digitalWrite(sistemPasifLed, HIGH);
        digitalWrite(siren, LOW);
        Serial.println("Sistem pasifleştirildiği için siren susturuldu");
        if(strcmp(telegramState,"0")){
          bot.sendMessage(chat_id, devices[currentDevice].remoteName, "");
        }
        if(strcmp(smsState,"0")){
          //*****Sistem kumanda ile pasif duruma getirildiğinde sms göndermek için ayarlama yapıyoruz.(Bunu istemiyorsanız bu bölümü silin)
          String message = devices[currentDevice].remoteName;
          sendSMS(message);
          //*****
        }
        config["sysState"] = "0";
        writeFile(LittleFS, "/config.json", config);
      }

      if (devices[currentDevice].group == 1) {
        enable_system = true;
        digitalWrite(sistemAktifLed, HIGH);
        digitalWrite(sistemPasifLed, LOW);
        if(strcmp(telegramState,"0")){
          bot.sendMessage(chat_id, devices[currentDevice].remoteName, "");
        }
        config["sysState"] = "1";
        writeFile(LittleFS, "/config.json", config);
      }

      if (devices[currentDevice].group == 2) {
        if(strcmp(telegramState,"0")){
          bot.sendMessage(chat_id, devices[currentDevice].remoteName, "");
        }
        if(strcmp(smsState,"0")){
          String message = devices[currentDevice].remoteName;
          sendSMS(message);
        }
      }

      if ((devices[currentDevice].group == 3) && enable_system) {
        sirenzaman = millis();
        digitalWrite(siren, HIGH);
        Serial.println("Sensör tetiklendiği için siren çalmaya başladı");
        if(strcmp(telegramState,"0")){
          bot.sendMessage(chat_id, devices[currentDevice].remoteName, "");
        }
        if(strcmp(smsState,"0")){
          String message = devices[currentDevice].remoteName;
          sendSMS(message);
        }
        delay(3000);

        if(strcmp(callState,"0")){
          Serial2.print("ATD+ ");
          Serial2.print(callNumber);
          Serial2.println(";");
          //Serial2.println("ATH");
          // Set module to send SMS data to serial out upon receipt
          Serial2.print("AT+CNMI=2,2,0,0,0\r");
        }
      }

      if (devices[currentDevice].group == 4) {
        digitalWrite(siren, LOW);
        Serial.println("Susturma tuşuna basıldığı için siren susturuldu");
      }

    }
  } else {
    Serial.println(String(remoteID) + ": " + "bilinmeyen kod");

    remoteIdGet = String(remoteID);
    
    if (enable_sniffing)
      bot.sendMessage(chat_id, String(remoteID) + ": " + "bilinmeyen kod", ""); //use this to get the sensor id codes
  }
}

void handleNewMessages(int numNewMessages) {
  //Serial.println("handleNewMessages");
  //Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
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
      config["sysState"] = "1";
      writeFile(LittleFS, "/config.json", config);
    }

    else if (text == "/pasif") {
      enable_system = false;
      digitalWrite(sistemAktifLed, LOW);
      digitalWrite(sistemPasifLed, HIGH);
      bot.sendMessage(chat_id, "Sistem Devre Dışı", "");
      config["sysState"] = "0";
      writeFile(LittleFS, "/config.json", config);
    }

    else if (text == "/durum") {
      if (enable_system) {
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
      if (enable_sniffing) {
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

//#########################################################################
IPAddress localIP;

//#########################################################################
// Set your Gateway IP address
IPAddress localGateway;

//#########################################################################
IPAddress subnet(255, 255, 0, 0);

// Initialize LittleFS
void initFS() {
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  else{
    Serial.println("LittleFS mounted successfully");
  }
}

// Initialize WiFi
bool initWiFi() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");
  delay(5000);
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect.");
    return false;
  }
  return true;
}

// Replaces placeholder with state value
String processor(const String& var) {
  if(var == "SSIDSTATE") {
    return ssid;
  }

  if(var == "PASSSTATE") {
    return password;
  }

  if(var == "IPSTATE") {
    return ip;
  }

  if(var == "GATEWAYSTATE") {
    return gateway;
  }

  if(var == "UINAMESTATE") {
    return uiName;
  }

  if(var == "UIPASSSTATE") {
    return uiPass;
  }

  if(var == "APPASSSTATE") {
    return apPass;
  }

  if(var == "SIRENSTATE") {
    return sirenTime;
  }

  if(var == "SYSSMSENABLE") {
    return smsSysEnable;
  }

  if(var == "SYSSMSDISABLE") {
    return smsSysDisable;
  }

  if(var == "RFCODEESTATE") {
    return remoteIdGet;
  }

  if(var == "CALLNOSTATE") {
    return callNumber;
  }

  if(var == "SMSNOSTATE") {
    return smsNumber;
  }

  if(var == "TOKENSTATE") {
    return telegramToken;
  }

  if(var == "TIDSTATE") {
    return chat_id;
  }

  //sms control

  if(var == "CTRLSMS1") {
    return ctrlMessage1;
  }

  if(var == "CTRLSMS2") {
    return ctrlMessage2;
  }

  if(var == "CTRLSMS3") {
    return ctrlMessage3;
  }

  if(var == "CTRLSMS4") {
    return ctrlMessage4;
  }

  if(var == "CTRLSMS5") {
    return ctrlMessage5;
  }

  if(var == "CTRLSMS6") {
    return ctrlMessage6;
  }

  if(var == "CTRLSMS7") {
    return ctrlMessage7;
  }

  if(var == "CTRLSMS8") {
    return ctrlMessage8;
  }

  if(var == "CTRLSMS9") {
    return ctrlMessage9;
  }

  if(var == "CTRLSMS0") {
    return ctrlMessage0;
  }

  if(var == "SENDSMS1") {
    return sendMessage1;
  }

  if(var == "SENDSMS2") {
    return sendMessage2;
  }

  if(var == "SENDSMS3") {
    return sendMessage3;
  }

  if(var == "SENDSMS4") {
    return sendMessage4;
  }

  if(var == "SENDSMS5") {
    return sendMessage5;
  }

  if(var == "SENDSMS6") {
    return sendMessage6;
  }

  if(var == "SENDSMS7") {
    return sendMessage7;
  }

  if(var == "SENDSMS8") {
    return sendMessage8;
  }

  if(var == "SENDSMS9") {
    return sendMessage9;
  }

  if(var == "SENDSMS0") {
    return sendMessage0;
  }

  if(var == "SENDRFSTATE1") {
    return sendRf1;
  }

  if(var == "SENDRFSTATE2") {
    return sendRf2;
  }

  if(var == "SENDRFSTATE3") {
    return sendRf3;
  }

  if(var == "SENDRFSTATE4") {
    return sendRf4;
  }

  if(var == "SENDRFSTATE5") {
    return sendRf5;
  }

  if(var == "SENDRFSTATE6") {
    return sendRf6;
  }

  if(var == "SENDRFSTATE7") {
    return sendRf7;
  }

  if(var == "SENDRFSTATE8") {
    return sendRf8;
  }

  if(var == "SENDRFSTATE9") {
    return sendRf9;
  }

  if(var == "SENDRFSTATE0") {
    return sendRf0;
  }




  if(var == "SYSSTATE") {
    String sysState1;
    if(strcmp(sysState,"0")){
      sysState1 ="checked";
    }else{
      sysState1 ="";
    }
    return sysState1;
  }

  if(var == "SMSSYSCTRLSTATE") {
    String smsSysCtrl1;
    if(strcmp(smsSysCtrl,"0")){
      smsSysCtrl1 ="checked";
    }else{
      smsSysCtrl1 ="";
    }
    return smsSysCtrl1;
  }

  if(var == "SMSCTRLSTATE") {
    String smsCtrl1;
    if(strcmp(smsCtrl,"0")){
      smsCtrl1 ="checked";
    }else{
      smsCtrl1 ="";
    }
    return smsCtrl1;
  }

  if(var == "CALLSWITCHSTATE") {
    String callState1;
    if(strcmp(callState,"0")){
      callState1 ="checked";
    }else{
      callState1 ="";
    }
    return callState1;
  }

  if(var == "SMSSWITCHSTATE") {
    String smsState1;
    if(strcmp(smsState,"0")){
      smsState1 ="checked";
    }else{
      smsState1 ="";
    }
    return smsState1;
  }

  if(var == "TLGSTATE") {
    String telegramState1;
    if(strcmp(telegramState,"0")){
      telegramState1 ="checked";
    }else{
      telegramState1 ="";
    }
    return telegramState1;
  }

  return String();
}

// Function that sends SMS
void sendSMS(String message) {
  // AT command to set SIM900 to SMS mode
  Serial2.println("AT+CMGF=1\r\n");
  delay(100);

  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  Serial2.print("AT+CMGS=\"");
  Serial2.print(smsNumber);
  Serial2.println("\"\r\n");
  delay(500);

  // Send the SMS
  Serial2.println(message);
  delay(500);

  // End AT command with a ^Z, ASCII code 26
  Serial2.println((char)26);
}


void SmsSysControl() {

  //Sms ile sistemi açmak için ayarlıyoruz
  if ((textMessage.indexOf(smsSysEnable) >= 0) && !enable_system) { //Gönderilecek sms içeriği # dahil
    enable_system = true;
    digitalWrite(sistemAktifLed, HIGH);
    digitalWrite(sistemPasifLed, LOW);
    config["sysState"] = "1";
    writeFile(LittleFS, "/config.json", config);
    if(strcmp(smsState,"0")){
      String message = "Sistem Sms ile Aktif Duruma Getirildi";
      sendSMS(message);
    }
    textMessage = "";
  }

  //Sms ile sistemi kapatmak için ayarlıyoruz
  if ((textMessage.indexOf(smsSysDisable) >= 0) && enable_system) { //Gönderilecek sms içeriği # dahil
    enable_system = false;
    digitalWrite(sistemAktifLed, LOW);
    digitalWrite(sistemPasifLed, HIGH);
    digitalWrite(siren, LOW);
    config["sysState"] = "0";
    writeFile(LittleFS, "/config.json", config);
    Serial.println("Sms ile sistem pasifleştirildiği için siren susturuldu");
    if(strcmp(smsState,"0")){
      String message = "Sistem Sms ile Pasif Duruma Getirildi";
      sendSMS(message);
    }
    textMessage = "";
  }

  //Sms ile sistemin durumunu sorguluyoruz
  if (textMessage.indexOf("#DURUM") >= 0) { //Gönderilecek sms içeriği # dahil
    if (enable_system) {
      if(strcmp(smsState,"0")){
      String message = "Durum: Sistem Aktif";
      sendSMS(message);
      }
      textMessage = "";
    } else {
      String message = "Durum: Sistem Devre Disi";
      sendSMS(message);
      textMessage = "";
    }
  }

}

void SMScontrol() {

  //Sms ile cihaza komut gönderiyoruz
  if (textMessage.indexOf(ctrlMessage1) >= 0) { //Gönderilecek sms içeriği
    rfReceiver.send(atol(sendRf1), 24);
    String message = sendMessage1;
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }

  if (textMessage.indexOf(ctrlMessage2) >= 0) { //Gönderilecek sms içeriği
    rfReceiver.send(atol(sendRf2), 24);
    String message = sendMessage2;
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }
  
  if (textMessage.indexOf(ctrlMessage3) >= 0) { //Gönderilecek sms içeriği
    rfReceiver.send(atol(sendRf3), 24);
    String message = sendMessage3;
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }
  
  if (textMessage.indexOf(ctrlMessage4) >= 0) { //Gönderilecek sms içeriği
    rfReceiver.send(atol(sendRf4), 24);
    String message = sendMessage4;
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }
  
  if (textMessage.indexOf(ctrlMessage5) >= 0) { //Gönderilecek sms içeriği
    rfReceiver.send(atol(sendRf5), 24);
    String message = sendMessage5;
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }
  
  if (textMessage.indexOf(ctrlMessage6) >= 0) { //Gönderilecek sms içeriği
    rfReceiver.send(atol(sendRf6), 24);
    String message = sendMessage6;
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }
  
  if (textMessage.indexOf(ctrlMessage7) >= 0) { //Gönderilecek sms içeriği
    rfReceiver.send(atol(sendRf7), 24);
    String message = sendMessage7;
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }
  
  if (textMessage.indexOf(ctrlMessage8) >= 0) { //Gönderilecek sms içeriği
    rfReceiver.send(atol(sendRf8), 24);
    String message = sendMessage8;
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }
  
  if (textMessage.indexOf(ctrlMessage9) >= 0) { //Gönderilecek sms içeriği
    rfReceiver.send(atol(sendRf9), 24);
    String message = sendMessage9;
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }
  
  if (textMessage.indexOf(ctrlMessage0) >= 0) { //Gönderilecek sms içeriği
    rfReceiver.send(atol(sendRf0), 24);
    String message = sendMessage0;
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }

}

void setup() {
  //Esp32 ile SIM800 seri haberleşmesi başlatılıyor
  Serial.begin(115200);
  Serial2.begin(9600);

  initFS();

  readJSON(LittleFS, "/config.json", config);
  readJSON(LittleFS, "/smsconfig.json", smsconfig);
  readJSON(LittleFS, "/smsconfig2.json", smsconfig2);
  ssid = config["ssid"];
  password = config["password"];
  ip = config["ip"];
  gateway = config["gateway"];
  sirenTime = config["sirenTime"];
  callNumber = config["callNumber"];
  smsNumber = config["smsNumber"];
  telegramToken = config["telegramToken"];
  chat_id = config["chat_id"];
  sysState = config["sysState"];
  smsSysCtrl = config["smsSysCtrl"];
  smsSysEnable = config["smsSysEnable"];
  smsSysDisable = config["smsSysDisable"];
  smsCtrl = config["smsCtrl"];
  callState = config["callState"];
  smsState = config["smsState"];
  telegramState = config["telegramState"];
  apPass = config["apPass"];
  uiName = config["uiName"];
  uiPass = config["uiPass"];

  //sms control
  ctrlMessage1 = smsconfig["ctrlMessage1"];
  ctrlMessage2 = smsconfig["ctrlMessage2"];
  ctrlMessage3 = smsconfig["ctrlMessage3"];
  ctrlMessage4 = smsconfig["ctrlMessage4"];
  ctrlMessage5 = smsconfig["ctrlMessage5"];
  ctrlMessage6 = smsconfig["ctrlMessage6"];
  ctrlMessage7 = smsconfig["ctrlMessage7"];
  ctrlMessage8 = smsconfig["ctrlMessage8"];
  ctrlMessage9 = smsconfig["ctrlMessage9"];
  ctrlMessage0 = smsconfig["ctrlMessage0"];
  sendMessage1 = smsconfig["sendMessage1"];
  sendMessage2 = smsconfig["sendMessage2"];
  sendMessage3 = smsconfig["sendMessage3"];
  sendMessage4 = smsconfig["sendMessage4"];
  sendMessage5 = smsconfig["sendMessage5"];
  sendMessage6 = smsconfig2["sendMessage6"];
  sendMessage7 = smsconfig2["sendMessage7"];
  sendMessage8 = smsconfig2["sendMessage8"];
  sendMessage9 = smsconfig2["sendMessage9"];
  sendMessage0 = smsconfig2["sendMessage0"];
  sendRf1 = smsconfig2["sendRf1"];
  sendRf2 = smsconfig2["sendRf2"];
  sendRf3 = smsconfig2["sendRf3"];
  sendRf4 = smsconfig2["sendRf4"];
  sendRf5 = smsconfig2["sendRf5"];
  sendRf6 = smsconfig2["sendRf6"];
  sendRf7 = smsconfig2["sendRf7"];
  sendRf8 = smsconfig2["sendRf8"];
  sendRf9 = smsconfig2["sendRf9"];
  sendRf0 = smsconfig2["sendRf0"];

  //config
  Serial.print("ssid: ");
  Serial.println(ssid);
  Serial.print("password: ");
  Serial.println(password);
  Serial.print("ip: ");
  Serial.println(ip);
  Serial.print("gateway: ");
  Serial.println(gateway);
  Serial.print("sirenTime: ");
  Serial.println(sirenTime);
  Serial.print("callNumber: ");
  Serial.println(callNumber);
  Serial.print("smsNumber: ");
  Serial.println(smsNumber);
  Serial.print("telegramToken: ");
  Serial.println(telegramToken);
  Serial.print("chat_id: ");
  Serial.println(chat_id);
  Serial.print("sysState: ");
  Serial.println(sysState);
  Serial.print("smsSysCtrl: ");
  Serial.println(smsSysCtrl);
  Serial.print("smsSysEnable: ");
  Serial.println(smsSysEnable);
  Serial.print("smsSysDisable: ");
  Serial.println(smsSysDisable);
  Serial.print("smsCtrl: ");
  Serial.println(smsCtrl);
  Serial.print("callState: ");
  Serial.println(callState);
  Serial.print("smsState: ");
  Serial.println(smsState);
  Serial.print("telegramState: ");
  Serial.println(telegramState);
  Serial.print("apPass: ");
  Serial.println(apPass);
  Serial.print("uiName: ");
  Serial.println(uiName);
  Serial.print("uiPass: ");
  Serial.println(uiPass);

  //sms control
  Serial.print("ctrlMessage1: ");
  Serial.println(ctrlMessage1);
  Serial.print("sendMessage1: ");
  Serial.println(sendMessage1);
  Serial.print("sendRf1: ");
  Serial.println(sendRf1);
  Serial.print("ctrlMessage2: ");
  Serial.println(ctrlMessage2);
  Serial.print("sendMessage2: ");
  Serial.println(sendMessage2);
  Serial.print("sendRf2: ");
  Serial.println(sendRf2);
  Serial.print("ctrlMessage3: ");
  Serial.println(ctrlMessage3);
  Serial.print("sendMessage3: ");
  Serial.println(sendMessage3);
  Serial.print("sendRf3: ");
  Serial.println(sendRf3);
  Serial.print("ctrlMessage4: ");
  Serial.println(ctrlMessage4);
  Serial.print("sendMessage4: ");
  Serial.println(sendMessage4);
  Serial.print("sendRf4: ");
  Serial.println(sendRf4);
  Serial.print("ctrlMessage5: ");
  Serial.println(ctrlMessage5);
  Serial.print("sendMessage5: ");
  Serial.println(sendMessage5);
  Serial.print("sendRf5: ");
  Serial.println(sendRf5);
  Serial.print("ctrlMessage6: ");
  Serial.println(ctrlMessage6);
  Serial.print("sendMessage6: ");
  Serial.println(sendMessage6);
  Serial.print("sendRf6: ");
  Serial.println(sendRf6);
  Serial.print("ctrlMessage7: ");
  Serial.println(ctrlMessage7);
  Serial.print("sendMessage7: ");
  Serial.println(sendMessage7);
  Serial.print("sendRf7: ");
  Serial.println(sendRf7);
  Serial.print("ctrlMessage8: ");
  Serial.println(ctrlMessage8);
  Serial.print("sendMessage8: ");
  Serial.println(sendMessage8);
  Serial.print("sendRf8: ");
  Serial.println(sendRf8);
  Serial.print("ctrlMessage9: ");
  Serial.println(ctrlMessage9);
  Serial.print("sendMessage9: ");
  Serial.println(sendMessage9);
  Serial.print("sendRf9: ");
  Serial.println(sendRf9);
  Serial.print("ctrlMessage0: ");
  Serial.println(ctrlMessage0);
  Serial.print("sendMessage0: ");
  Serial.println(sendMessage0);
  Serial.print("sendRf0: ");
  Serial.println(sendRf0);
  
  sirenSure = strtoul(sirenTime, NULL, 10);

  bot.updateToken(telegramToken);

  // This is the simplest way of getting this working
  // if you are passing sensitive information, or controlling
  // something important, please either use certStore or at
  // least client.setFingerPrint
  client.setInsecure();
  
  if(initWiFi()) {
      Serial.println(WiFi.localIP());
  }
  else {
    // Connect to Wi-Fi network with SSID and password
    Serial.println("Setting AP (Access Point)");
    // NULL sets an open Access Point
    WiFi.softAP("Güvenlik Sistemi", apPass);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP); 
  }

  rfReceiver.enableReceive(rfReceiverPin);
  rfReceiver.enableTransmit(rfTransmitterPin);

  if(strcmp(sysState,"0")) {
    enable_system = true;
  }else {
    enable_system = false;
  }

  //Siren pininin çıkış pini olduğunu belirtiyoruz
  pinMode (siren, OUTPUT);

  //Durum ledlerini ayarlıyoruz
  pinMode (sistemAktifLed, OUTPUT);
  pinMode (sistemPasifLed, OUTPUT);

  if(enable_system){
    digitalWrite(sistemAktifLed, HIGH);
    digitalWrite(sistemPasifLed, LOW);
  }
  else {
    digitalWrite(sistemAktifLed, LOW);
    digitalWrite(sistemPasifLed, HIGH);
  }

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      if(!request->authenticate(uiName, uiPass))
      return request->requestAuthentication();
    request->send(LittleFS, "/index.html", "text/html", false, processor);
  });

  // Route to load style.css file
  server.on("css/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "css/style.css", "text/css");
  });

    // Route to load style.css file
  server.on("js/scripts.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "js/scripts.js", "text/javascript");
  });
  
  server.serveStatic("/", LittleFS, "/");
  
  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
    int params = request->params();
    for(int i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      //Variables to save values from HTML form

      if(p->isPost()){
        // HTTP POST ssid value
        if (p->name() == PARAM_INPUT_1) {
          config["ssid"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST pass value
        if (p->name() == PARAM_INPUT_2) {
          config["password"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST ip value
        if (p->name() == PARAM_INPUT_3) {
          config["ip"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST gateway value
        if (p->name() == PARAM_INPUT_4) {
          config["gateway"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST apPass value
        if (p->name() == PARAM_INPUT_12) {
          config["apPass"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST gateway value
        if (p->name() == PARAM_INPUT_13) {
          config["uiName"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST gateway value
        if (p->name() == PARAM_INPUT_14) {
          config["uiPass"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST sirenTime value
        if (p->name() == PARAM_INPUT_5) {
          config["sirenTime"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST smsSysEnable value
        if (p->name() == PARAM_INPUT_15) {
          config["smsSysEnable"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST smsSysDisable value
        if (p->name() == PARAM_INPUT_16) {
          config["smsSysDisable"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST callNumber value
        if (p->name() == PARAM_INPUT_6) {
          config["callNumber"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST smsNumber value
        if (p->name() == PARAM_INPUT_7) {
          config["smsNumber"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST telegramToken value
        if (p->name() == PARAM_INPUT_8) {
          config["telegramToken"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST chat_id value
        if (p->name() == PARAM_INPUT_9) {
          config["chat_id"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        //SMS CONTROL
        if (p->name() == PARAM_INPUT_17) {
          smsconfig["ctrlMessage1"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }
        
        if (p->name() == PARAM_INPUT_18) {
          smsconfig["ctrlMessage2"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }
        
        if (p->name() == PARAM_INPUT_19) {
          smsconfig["ctrlMessage3"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }
        
        if (p->name() == PARAM_INPUT_20) {
          smsconfig["ctrlMessage4"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }
        
        if (p->name() == PARAM_INPUT_21) {
          smsconfig["ctrlMessage5"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }
        
        if (p->name() == PARAM_INPUT_22) {
          smsconfig["ctrlMessage6"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }
        
        if (p->name() == PARAM_INPUT_23) {
          smsconfig["ctrlMessage7"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }
        
        if (p->name() == PARAM_INPUT_24) {
          smsconfig["ctrlMessage8"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }
        
        if (p->name() == PARAM_INPUT_25) {
          smsconfig["ctrlMessage9"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }
        
        if (p->name() == PARAM_INPUT_26) {
          smsconfig["ctrlMessage0"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }

        if (p->name() == PARAM_INPUT_27) {
          smsconfig["sendMessage1"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }

        if (p->name() == PARAM_INPUT_28) {
          smsconfig["sendMessage2"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }

        if (p->name() == PARAM_INPUT_29) {
          smsconfig["sendMessage3"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }

        if (p->name() == PARAM_INPUT_30) {
          smsconfig["sendMessage4"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }

        if (p->name() == PARAM_INPUT_31) {
          smsconfig["sendMessage5"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig.json", smsconfig);
        }

        if (p->name() == PARAM_INPUT_32) {
          smsconfig2["sendMessage6"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }

        if (p->name() == PARAM_INPUT_33) {
          smsconfig2["sendMessage7"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }

        if (p->name() == PARAM_INPUT_34) {
          smsconfig2["sendMessage8"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }

        if (p->name() == PARAM_INPUT_35) {
          smsconfig2["sendMessage9"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }

        if (p->name() == PARAM_INPUT_36) {
          smsconfig2["sendMessage0"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }
        
        if (p->name() == PARAM_INPUT_37) {
          smsconfig2["sendRf1"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }
        
        if (p->name() == PARAM_INPUT_38) {
          smsconfig2["sendRf2"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }
        
        if (p->name() == PARAM_INPUT_39) {
          smsconfig2["sendRf3"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }
        
        if (p->name() == PARAM_INPUT_40) {
          smsconfig2["sendRf4"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }
        
        if (p->name() == PARAM_INPUT_41) {
          smsconfig2["sendRf5"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }
        
        if (p->name() == PARAM_INPUT_42) {
          smsconfig2["sendRf6"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }
        
        if (p->name() == PARAM_INPUT_43) {
          smsconfig2["sendRf7"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }
        
        if (p->name() == PARAM_INPUT_44) {
          smsconfig2["sendRf8"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }
        
        if (p->name() == PARAM_INPUT_45) {
          smsconfig2["sendRf9"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }
        
        if (p->name() == PARAM_INPUT_46) {
          smsconfig2["sendRf0"] = p->value().c_str();
          writeFile(LittleFS, "/smsconfig2.json", smsconfig2);
        }

        //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String paramState;
    String paramOutput;

    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<paramState>
    if (request->hasParam(PARAM_INPUT_10) && request->hasParam(PARAM_INPUT_11)) {
      paramOutput = request->getParam(PARAM_INPUT_10)->value();
      paramState = request->getParam(PARAM_INPUT_11)->value();
      //digitalWrite(inputMessage1.toInt(), paramState.toInt());

      if(paramOutput == "sysSwitch"){
      config["sysState"] = paramState;
          writeFile(LittleFS, "/config.json", config);
      }

      if(paramOutput == "smsSysCtrlSwitch"){
      config["smsSysCtrl"] = paramState;
          writeFile(LittleFS, "/config.json", config);
      }

      if(paramOutput == "smsCtrlSwitch"){
      config["smsCtrl"] = paramState;
          writeFile(LittleFS, "/config.json", config);
      }

      if(paramOutput == "callSwitch"){
      config["callState"] = paramState;
          writeFile(LittleFS, "/config.json", config);
      }

      if(paramOutput == "smsSwitch"){
      config["smsState"] = paramState;
          writeFile(LittleFS, "/config.json", config);
      }

      if(paramOutput == "telegramSwitch"){
      config["telegramState"] = paramState;
          writeFile(LittleFS, "/config.json", config);
      }

    }

    Serial.print(paramOutput);
    Serial.print(" - Set to: ");
    Serial.println(paramState);
  });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html", false, processor);
    delay(1000);
    ESP.restart();
  });

  server.begin();
  // AT command to set SIM900 to SMS mode
  Serial2.print("AT+CMGF=1\r");
  delay(100);
  // Set module to send SMS data to serial out upon receipt
  Serial2.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);

  if(strcmp(telegramState,"0")){
    bot.sendMessage(chat_id, "Sistem Başlatıldı", "");
  }
  Serial.println("Sistem Başlatıldı.");

}

void loop() {
  //Sim800den seri monitöre komut göndermek için ayarlama yapıyoruz. Bu arada textMessage değişkenini sim800den gelen komut ile sabitliyoruz
  if (Serial2.available() > 0) {
    textMessage = Serial2.readString();
    Serial.print(textMessage);
  }

  //Seri monitörden sim800e komut göndermek için ayarlıyoruz. Bu kod test aşamasında gerekli. Daha sonra kaldıralım
  if (Serial.available()) {
    Serial2.write(Serial.read());
  }

  if(strcmp(smsSysCtrl,"0")) {
    SmsSysControl();
  }

  if(strcmp(smsCtrl,"0")) {
    SMScontrol();
  }

  if (millis() - sirenzaman > sirenSure) {
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
