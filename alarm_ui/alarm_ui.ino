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

//Devices
const char* PARAM_INPUT_47 = "rfCode1";
const char* PARAM_INPUT_48 = "rfCode2";
const char* PARAM_INPUT_49 = "rfCode3";
const char* PARAM_INPUT_50 = "rfCode4";
const char* PARAM_INPUT_51 = "rfCode5";
const char* PARAM_INPUT_52 = "rfCode6";
const char* PARAM_INPUT_53 = "rfCode7";
const char* PARAM_INPUT_54 = "rfCode8";
const char* PARAM_INPUT_55 = "rfCode9";
const char* PARAM_INPUT_56 = "rfCode10";
const char* PARAM_INPUT_57 = "rfCode11";
const char* PARAM_INPUT_58 = "rfCode12";
const char* PARAM_INPUT_59 = "rfCode13";
const char* PARAM_INPUT_60 = "rfCode14";
const char* PARAM_INPUT_61 = "rfCode15";
const char* PARAM_INPUT_62 = "rfCode16";
const char* PARAM_INPUT_63 = "rfCode17";
const char* PARAM_INPUT_64 = "rfCode18";
const char* PARAM_INPUT_65 = "rfCode19";
const char* PARAM_INPUT_66 = "rfCode20";
const char* PARAM_INPUT_67 = "rfCode21";
const char* PARAM_INPUT_68 = "rfCode22";
const char* PARAM_INPUT_69 = "rfCode23";
const char* PARAM_INPUT_70 = "rfCode24";
const char* PARAM_INPUT_71 = "rfCode25";
const char* PARAM_INPUT_72 = "rfCode26";
const char* PARAM_INPUT_73 = "rfCode27";
const char* PARAM_INPUT_74 = "rfCode28";
const char* PARAM_INPUT_75 = "rfCode29";
const char* PARAM_INPUT_76 = "rfCode30";

const char* PARAM_INPUT_77 = "rfMessage1";
const char* PARAM_INPUT_78 = "rfMessage2";
const char* PARAM_INPUT_79 = "rfMessage3";
const char* PARAM_INPUT_80 = "rfMessage4";
const char* PARAM_INPUT_81 = "rfMessage5";
const char* PARAM_INPUT_82 = "rfMessage6";
const char* PARAM_INPUT_83 = "rfMessage7";
const char* PARAM_INPUT_84 = "rfMessage8";
const char* PARAM_INPUT_85 = "rfMessage9";
const char* PARAM_INPUT_86 = "rfMessage10";
const char* PARAM_INPUT_87 = "rfMessage11";
const char* PARAM_INPUT_88 = "rfMessage12";
const char* PARAM_INPUT_89 = "rfMessage13";
const char* PARAM_INPUT_90 = "rfMessage14";
const char* PARAM_INPUT_91 = "rfMessage15";
const char* PARAM_INPUT_92 = "rfMessage16";
const char* PARAM_INPUT_93 = "rfMessage17";
const char* PARAM_INPUT_94 = "rfMessage18";
const char* PARAM_INPUT_95 = "rfMessage19";
const char* PARAM_INPUT_96 = "rfMessage20";
const char* PARAM_INPUT_97 = "rfMessage21";
const char* PARAM_INPUT_98 = "rfMessage22";
const char* PARAM_INPUT_99 = "rfMessage23";
const char* PARAM_INPUT_100 = "rfMessage24";
const char* PARAM_INPUT_101 = "rfMessage25";
const char* PARAM_INPUT_102 = "rfMessage26";
const char* PARAM_INPUT_103 = "rfMessage27";
const char* PARAM_INPUT_104 = "rfMessage28";
const char* PARAM_INPUT_105 = "rfMessage29";
const char* PARAM_INPUT_106 = "rfMessage30";

const char* PARAM_INPUT_107 = "rfGroup1";
const char* PARAM_INPUT_108 = "rfGroup2";
const char* PARAM_INPUT_109 = "rfGroup3";
const char* PARAM_INPUT_110 = "rfGroup4";
const char* PARAM_INPUT_111 = "rfGroup5";
const char* PARAM_INPUT_112 = "rfGroup6";
const char* PARAM_INPUT_113 = "rfGroup7";
const char* PARAM_INPUT_114 = "rfGroup8";
const char* PARAM_INPUT_115 = "rfGroup9";
const char* PARAM_INPUT_116 = "rfGroup10";
const char* PARAM_INPUT_117 = "rfGroup11";
const char* PARAM_INPUT_118 = "rfGroup12";
const char* PARAM_INPUT_119 = "rfGroup13";
const char* PARAM_INPUT_120 = "rfGroup14";
const char* PARAM_INPUT_121 = "rfGroup15";
const char* PARAM_INPUT_122 = "rfGroup16";
const char* PARAM_INPUT_123 = "rfGroup17";
const char* PARAM_INPUT_124 = "rfGroup18";
const char* PARAM_INPUT_125 = "rfGroup19";
const char* PARAM_INPUT_126 = "rfGroup20";
const char* PARAM_INPUT_127 = "rfGroup21";
const char* PARAM_INPUT_128 = "rfGroup22";
const char* PARAM_INPUT_129 = "rfGroup23";
const char* PARAM_INPUT_130 = "rfGroup24";
const char* PARAM_INPUT_131 = "rfGroup25";
const char* PARAM_INPUT_132 = "rfGroup26";
const char* PARAM_INPUT_133 = "rfGroup27";
const char* PARAM_INPUT_134 = "rfGroup28";
const char* PARAM_INPUT_135 = "rfGroup29";
const char* PARAM_INPUT_136 = "rfGroup30";



//Variables to save values from HTML form


const char* ssid;
const char* password;
const char* ap_ip_str = "192.168.4.22"; // AP'nin IP adresi
const char* ap_ssid = "Güvenlik Sistemi"; // access point adı
const char* apPass; // access point şifresi
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
//devices
const char* rfCode1;
const char* rfCode2;
const char* rfCode3;
const char* rfCode4;
const char* rfCode5;
const char* rfCode6;
const char* rfCode7;
const char* rfCode8;
const char* rfCode9;
const char* rfCode10;
const char* rfCode11;
const char* rfCode12;
const char* rfCode13;
const char* rfCode14;
const char* rfCode15;
const char* rfCode16;
const char* rfCode17;
const char* rfCode18;
const char* rfCode19;
const char* rfCode20;
const char* rfCode21;
const char* rfCode22;
const char* rfCode23;
const char* rfCode24;
const char* rfCode25;
const char* rfCode26;
const char* rfCode27;
const char* rfCode28;
const char* rfCode29;
const char* rfCode30;

const char* rfMessage1;
const char* rfMessage2;
const char* rfMessage3;
const char* rfMessage4;
const char* rfMessage5;
const char* rfMessage6;
const char* rfMessage7;
const char* rfMessage8;
const char* rfMessage9;
const char* rfMessage10;
const char* rfMessage11;
const char* rfMessage12;
const char* rfMessage13;
const char* rfMessage14;
const char* rfMessage15;
const char* rfMessage16;
const char* rfMessage17;
const char* rfMessage18;
const char* rfMessage19;
const char* rfMessage20;
const char* rfMessage21;
const char* rfMessage22;
const char* rfMessage23;
const char* rfMessage24;
const char* rfMessage25;
const char* rfMessage26;
const char* rfMessage27;
const char* rfMessage28;
const char* rfMessage29;
const char* rfMessage30;

const char* rfGroup1;
const char* rfGroup2;
const char* rfGroup3;
const char* rfGroup4;
const char* rfGroup5;
const char* rfGroup6;
const char* rfGroup7;
const char* rfGroup8;
const char* rfGroup9;
const char* rfGroup10;
const char* rfGroup11;
const char* rfGroup12;
const char* rfGroup13;
const char* rfGroup14;
const char* rfGroup15;
const char* rfGroup16;
const char* rfGroup17;
const char* rfGroup18;
const char* rfGroup19;
const char* rfGroup20;
const char* rfGroup21;
const char* rfGroup22;
const char* rfGroup23;
const char* rfGroup24;
const char* rfGroup25;
const char* rfGroup26;
const char* rfGroup27;
const char* rfGroup28;
const char* rfGroup29;
const char* rfGroup30;

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
  const char*  remoteName;
  int group; // 0 - disarming; 1 - arming; 2 - reported 24h, 3 - reported when armed
  long lastTime;
} device_struct;

#define NUM_DEVICES 30 //<<<
device_struct devices[NUM_DEVICES]; //make the last entry above without a column

bool enable_system; //start armed
bool enable_sniffing = false;

DynamicJsonDocument config(1024);
DynamicJsonDocument smsconfig(1024);
DynamicJsonDocument smsconfig2(1024);
DynamicJsonDocument devices1(1024);
DynamicJsonDocument devices2(1024);
DynamicJsonDocument devices3(1024);
DynamicJsonDocument devices4(1024);
DynamicJsonDocument devices5(1024);
DynamicJsonDocument devices6(1024);
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

      if ((devices[currentDevice].group == 1) && !enable_system) {
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

      if ((devices[currentDevice].group == 4) && digitalRead(siren) == 1) {
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
void initWiFi() {

  // WiFi bağlantısını başlat
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Bağlanılıyor...");

  // Modeme bağlanmaya çalış
  int wifiTime = 0;
  while (WiFi.status() != WL_CONNECTED && wifiTime < 10) {
    delay(1000);
    Serial.print(".");
    wifiTime++;
  }

  // Modeme bağlanamazsa AP modunda çalış
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("");
    Serial.println("Modeme bağlanılamadı. AP modunda çalışılıyor...");
    // AP modunu başlat
    WiFi.mode(WIFI_AP);
    delay(100);
    // AP'nin IP adresini, gateway ve subnet maskesini ayarla
    IPAddress ap_ip;
    IPAddress gateway(192,168,1,1);
    IPAddress subnet(255, 255, 255, 0);
    ap_ip.fromString(ap_ip_str);
    if (!WiFi.softAPConfig(ap_ip, gateway, subnet)) {
      Serial.println("AP konfigürasyonu başarısız oldu!");
    }
    WiFi.softAP(ap_ssid, apPass);
    // AP IP adresini kontrol et
    IPAddress myIP = WiFi.softAPIP();
    if (myIP.toString() == "0.0.0.0") {
      Serial.println("AP IP adresi atanamadı!");
    } else {
      Serial.print("AP IP adresi: ");
      Serial.println(myIP);
    }

  } else {
    // STA IP adresini kontrol et
    IPAddress mystaIP = WiFi.localIP();
    if (mystaIP.toString() == "0.0.0.0") {
      Serial.println("STA IP adresi atanamadı!");
    }  else {
      // WiFi istasyon modunda başarılı bağlantı
      Serial.println("");
      Serial.print("Bağlandı! IP adresi: ");
      Serial.println(mystaIP);
    }

  }
}

// Replaces placeholder with state value
String processor(const String& var) {
  if(var == "SSIDSTATE") {
    return ssid;
  }

  if(var == "PASSSTATE") {
    return password;
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

//devices
if(var == "RFSTATE1") {
  return rfCode1;
}

if(var == "RFSTATE2") {
  return rfCode2;
}

if(var == "RFSTATE3") {
  return rfCode3;
}

if(var == "RFSTATE4") {
  return rfCode4;
}

if(var == "RFSTATE5") {
  return rfCode5;
}

if(var == "RFSTATE6") {
  return rfCode6;
}

if(var == "RFSTATE7") {
  return rfCode7;
}

if(var == "RFSTATE8") {
  return rfCode8;
}

if(var == "RFSTATE9") {
  return rfCode9;
}

if(var == "RFSTATE10") {
  return rfCode10;
}

if(var == "RFSTATE11") {
  return rfCode11;
}

if(var == "RFSTATE12") {
  return rfCode12;
}

if(var == "RFSTATE13") {
  return rfCode13;
}

if(var == "RFSTATE14") {
  return rfCode14;
}

if(var == "RFSTATE15") {
  return rfCode15;
}

if(var == "RFSTATE16") {
  return rfCode16;
}

if(var == "RFSTATE17") {
  return rfCode17;
}

if(var == "RFSTATE18") {
  return rfCode18;
}

if(var == "RFSTATE19") {
  return rfCode19;
}

if(var == "RFSTATE20") {
  return rfCode20;
}

if(var == "RFSTATE21") {
  return rfCode21;
}

if(var == "RFSTATE22") {
  return rfCode22;
}

if(var == "RFSTATE23") {
  return rfCode23;
}

if(var == "RFSTATE24") {
  return rfCode24;
}

if(var == "RFSTATE25") {
  return rfCode25;
}

if(var == "RFSTATE26") {
  return rfCode26;
}

if(var == "RFSTATE27") {
  return rfCode27;
}

if(var == "RFSTATE28") {
  return rfCode28;
}

if(var == "RFSTATE29") {
  return rfCode29;
}

if(var == "RFSTATE30") {
  return rfCode30;
}


if(var == "MESSAGESTATE1") {
  return rfMessage1;
}

if(var == "MESSAGESTATE2") {
  return rfMessage2;
}

if(var == "MESSAGESTATE3") {
  return rfMessage3;
}

if(var == "MESSAGESTATE4") {
  return rfMessage4;
}

if(var == "MESSAGESTATE5") {
  return rfMessage5;
}

if(var == "MESSAGESTATE6") {
  return rfMessage6;
}

if(var == "MESSAGESTATE7") {
  return rfMessage7;
}

if(var == "MESSAGESTATE8") {
  return rfMessage8;
}

if(var == "MESSAGESTATE9") {
  return rfMessage9;
}

if(var == "MESSAGESTATE10") {
  return rfMessage10;
}

if(var == "MESSAGESTATE11") {
  return rfMessage11;
}

if(var == "MESSAGESTATE12") {
  return rfMessage12;
}

if(var == "MESSAGESTATE13") {
  return rfMessage13;
}

if(var == "MESSAGESTATE14") {
  return rfMessage14;
}

if(var == "MESSAGESTATE15") {
  return rfMessage15;
}

if(var == "MESSAGESTATE16") {
  return rfMessage16;
}

if(var == "MESSAGESTATE17") {
  return rfMessage17;
}

if(var == "MESSAGESTATE18") {
  return rfMessage18;
}

if(var == "MESSAGESTATE19") {
  return rfMessage19;
}

if(var == "MESSAGESTATE20") {
  return rfMessage20;
}

if(var == "MESSAGESTATE21") {
  return rfMessage21;
}

if(var == "MESSAGESTATE22") {
  return rfMessage22;
}

if(var == "MESSAGESTATE23") {
  return rfMessage23;
}

if(var == "MESSAGESTATE24") {
  return rfMessage24;
}

if(var == "MESSAGESTATE25") {
  return rfMessage25;
}

if(var == "MESSAGESTATE26") {
  return rfMessage26;
}

if(var == "MESSAGESTATE27") {
  return rfMessage27;
}

if(var == "MESSAGESTATE28") {
  return rfMessage28;
}

if(var == "MESSAGESTATE29") {
  return rfMessage29;
}

if(var == "MESSAGESTATE30") {
  return rfMessage30;
}


if(var == "RFGROUP1") {
  return rfGroup1;
}

if(var == "RFGROUP2") {
  return rfGroup2;
}

if(var == "RFGROUP3") {
  return rfGroup3;
}

if(var == "RFGROUP4") {
  return rfGroup4;
}

if(var == "RFGROUP5") {
  return rfGroup5;
}

if(var == "RFGROUP6") {
  return rfGroup6;
}

if(var == "RFGROUP7") {
  return rfGroup7;
}

if(var == "RFGROUP8") {
  return rfGroup8;
}

if(var == "RFGROUP9") {
  return rfGroup9;
}

if(var == "RFGROUP10") {
  return rfGroup10;
}

if(var == "RFGROUP11") {
  return rfGroup11;
}

if(var == "RFGROUP12") {
  return rfGroup12;
}

if(var == "RFGROUP13") {
  return rfGroup13;
}

if(var == "RFGROUP14") {
  return rfGroup14;
}

if(var == "RFGROUP15") {
  return rfGroup15;
}

if(var == "RFGROUP16") {
  return rfGroup16;
}

if(var == "RFGROUP17") {
  return rfGroup17;
}

if(var == "RFGROUP18") {
  return rfGroup18;
}

if(var == "RFGROUP19") {
  return rfGroup19;
}

if(var == "RFGROUP20") {
  return rfGroup20;
}

if(var == "RFGROUP21") {
  return rfGroup21;
}

if(var == "RFGROUP22") {
  return rfGroup22;
}

if(var == "RFGROUP23") {
  return rfGroup23;
}

if(var == "RFGROUP24") {
  return rfGroup24;
}

if(var == "RFGROUP25") {
  return rfGroup25;
}

if(var == "RFGROUP26") {
  return rfGroup26;
}

if(var == "RFGROUP27") {
  return rfGroup27;
}

if(var == "RFGROUP28") {
  return rfGroup28;
}

if(var == "RFGROUP29") {
  return rfGroup29;
}

if(var == "RFGROUP30") {
  return rfGroup30;
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
  readJSON(LittleFS, "/devices1.json", devices1);
  readJSON(LittleFS, "/devices2.json", devices2);
  readJSON(LittleFS, "/devices3.json", devices3);
  readJSON(LittleFS, "/devices4.json", devices4);
  readJSON(LittleFS, "/devices5.json", devices5);
  readJSON(LittleFS, "/devices6.json", devices6);
  ssid = config["ssid"];
  password = config["password"];
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

  //devices
  rfCode1 = devices1["rfCode1"];
  rfCode2 = devices1["rfCode2"];
  rfCode3 = devices1["rfCode3"];
  rfCode4 = devices1["rfCode4"];
  rfCode5 = devices1["rfCode5"];
  rfCode6 = devices1["rfCode6"];
  rfCode7 = devices1["rfCode7"];
  rfCode8 = devices1["rfCode8"];
  rfCode9 = devices1["rfCode9"];
  rfCode10 = devices1["rfCode10"];
  rfCode11 = devices1["rfCode11"];
  rfCode12 = devices1["rfCode12"];
  rfCode13 = devices1["rfCode13"];
  rfCode14 = devices1["rfCode14"];
  rfCode15 = devices1["rfCode15"];

  rfCode16 = devices2["rfCode16"];
  rfCode17 = devices2["rfCode17"];
  rfCode18 = devices2["rfCode18"];
  rfCode19 = devices2["rfCode19"];
  rfCode20 = devices2["rfCode20"];
  rfCode21 = devices2["rfCode21"];
  rfCode22 = devices2["rfCode22"];
  rfCode23 = devices2["rfCode23"];
  rfCode24 = devices2["rfCode24"];
  rfCode25 = devices2["rfCode25"];
  rfCode26 = devices2["rfCode26"];
  rfCode27 = devices2["rfCode27"];
  rfCode28 = devices2["rfCode28"];
  rfCode29 = devices2["rfCode29"];
  rfCode30 = devices2["rfCode30"];

  rfMessage1 = devices3["rfMessage1"];
  rfMessage2 = devices3["rfMessage2"];
  rfMessage3 = devices3["rfMessage3"];
  rfMessage4 = devices3["rfMessage4"];
  rfMessage5 = devices3["rfMessage5"];
  rfMessage6 = devices3["rfMessage6"];
  rfMessage7 = devices3["rfMessage7"];
  rfMessage8 = devices3["rfMessage8"];
  rfMessage9 = devices3["rfMessage9"];
  rfMessage10 = devices3["rfMessage10"];
  rfMessage11 = devices3["rfMessage11"];
  rfMessage12 = devices3["rfMessage12"];
  rfMessage13 = devices3["rfMessage13"];
  rfMessage14 = devices3["rfMessage14"];
  rfMessage15 = devices3["rfMessage15"];

  rfMessage16 = devices4["rfMessage16"];
  rfMessage17 = devices4["rfMessage17"];
  rfMessage18 = devices4["rfMessage18"];
  rfMessage19 = devices4["rfMessage19"];
  rfMessage20 = devices4["rfMessage20"];
  rfMessage21 = devices4["rfMessage21"];
  rfMessage22 = devices4["rfMessage22"];
  rfMessage23 = devices4["rfMessage23"];
  rfMessage24 = devices4["rfMessage24"];
  rfMessage25 = devices4["rfMessage25"];
  rfMessage26 = devices4["rfMessage26"];
  rfMessage27 = devices4["rfMessage27"];
  rfMessage28 = devices4["rfMessage28"];
  rfMessage29 = devices4["rfMessage29"];
  rfMessage30 = devices4["rfMessage30"];

  rfGroup1 = devices5["rfGroup1"];
  rfGroup2 = devices5["rfGroup2"];
  rfGroup3 = devices5["rfGroup3"];
  rfGroup4 = devices5["rfGroup4"];
  rfGroup5 = devices5["rfGroup5"];
  rfGroup6 = devices5["rfGroup6"];
  rfGroup7 = devices5["rfGroup7"];
  rfGroup8 = devices5["rfGroup8"];
  rfGroup9 = devices5["rfGroup9"];
  rfGroup10 = devices5["rfGroup10"];
  rfGroup11 = devices5["rfGroup11"];
  rfGroup12 = devices5["rfGroup12"];
  rfGroup13 = devices5["rfGroup13"];
  rfGroup14 = devices5["rfGroup14"];
  rfGroup15 = devices5["rfGroup15"];

  rfGroup16 = devices6["rfGroup16"];
  rfGroup17 = devices6["rfGroup17"];
  rfGroup18 = devices6["rfGroup18"];
  rfGroup19 = devices6["rfGroup19"];
  rfGroup20 = devices6["rfGroup20"];
  rfGroup21 = devices6["rfGroup21"];
  rfGroup22 = devices6["rfGroup22"];
  rfGroup23 = devices6["rfGroup23"];
  rfGroup24 = devices6["rfGroup24"];
  rfGroup25 = devices6["rfGroup25"];
  rfGroup26 = devices6["rfGroup26"];
  rfGroup27 = devices6["rfGroup27"];
  rfGroup28 = devices6["rfGroup28"];
  rfGroup29 = devices6["rfGroup29"];
  rfGroup30 = devices6["rfGroup30"];

  devices[0] = {atoi(rfCode1), rfMessage1, atoi(rfGroup1)};
  devices[1] = {atoi(rfCode2), rfMessage2, atoi(rfGroup2)};
  devices[2] = {atoi(rfCode3), rfMessage3, atoi(rfGroup3)};
  devices[3] = {atoi(rfCode4), rfMessage4, atoi(rfGroup4)};
  devices[4] = {atoi(rfCode5), rfMessage5, atoi(rfGroup5)};
  devices[5] = {atoi(rfCode6), rfMessage6, atoi(rfGroup6)};
  devices[6] = {atoi(rfCode7), rfMessage7, atoi(rfGroup7)};
  devices[7] = {atoi(rfCode8), rfMessage8, atoi(rfGroup8)};
  devices[8] = {atoi(rfCode9), rfMessage9, atoi(rfGroup9)};
  devices[9] = {atoi(rfCode10), rfMessage10, atoi(rfGroup10)};
  devices[10] = {atoi(rfCode11), rfMessage11, atoi(rfGroup11)};
  devices[11] = {atoi(rfCode12), rfMessage12, atoi(rfGroup12)};
  devices[12] = {atoi(rfCode13), rfMessage13, atoi(rfGroup13)};
  devices[13] = {atoi(rfCode14), rfMessage14, atoi(rfGroup14)};
  devices[14] = {atoi(rfCode15), rfMessage15, atoi(rfGroup15)};
  devices[15] = {atoi(rfCode16), rfMessage16, atoi(rfGroup16)};
  devices[16] = {atoi(rfCode17), rfMessage17, atoi(rfGroup17)};
  devices[17] = {atoi(rfCode18), rfMessage18, atoi(rfGroup18)};
  devices[18] = {atoi(rfCode19), rfMessage19, atoi(rfGroup19)};
  devices[19] = {atoi(rfCode20), rfMessage20, atoi(rfGroup20)};
  devices[20] = {atoi(rfCode21), rfMessage21, atoi(rfGroup21)};
  devices[21] = {atoi(rfCode22), rfMessage22, atoi(rfGroup22)};
  devices[22] = {atoi(rfCode23), rfMessage23, atoi(rfGroup23)};
  devices[23] = {atoi(rfCode24), rfMessage24, atoi(rfGroup24)};
  devices[24] = {atoi(rfCode25), rfMessage25, atoi(rfGroup25)};
  devices[25] = {atoi(rfCode26), rfMessage26, atoi(rfGroup26)};
  devices[26] = {atoi(rfCode27), rfMessage27, atoi(rfGroup27)};
  devices[27] = {atoi(rfCode28), rfMessage28, atoi(rfGroup28)};
  devices[28] = {atoi(rfCode29), rfMessage29, atoi(rfGroup29)};
  devices[29] = {atoi(rfCode30), rfMessage30, atoi(rfGroup30)};

  //config
  Serial.print("ssid: ");
  Serial.println(ssid);
  Serial.print("password: ");
  Serial.println(password);
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

  //devices
Serial.print("rfCode1: ");
Serial.println(rfCode1);
Serial.print("rfCode2: ");
Serial.println(rfCode2);
Serial.print("rfCode3: ");
Serial.println(rfCode3);
Serial.print("rfCode4: ");
Serial.println(rfCode4);
Serial.print("rfCode5: ");
Serial.println(rfCode5);
Serial.print("rfCode6: ");
Serial.println(rfCode6);
Serial.print("rfCode7: ");
Serial.println(rfCode7);
Serial.print("rfCode8: ");
Serial.println(rfCode8);
Serial.print("rfCode9: ");
Serial.println(rfCode9);
Serial.print("rfCode10: ");
Serial.println(rfCode10);
Serial.print("rfCode11: ");
Serial.println(rfCode11);
Serial.print("rfCode12: ");
Serial.println(rfCode12);
Serial.print("rfCode13: ");
Serial.println(rfCode13);
Serial.print("rfCode14: ");
Serial.println(rfCode14);
Serial.print("rfCode15: ");
Serial.println(rfCode15);
Serial.print("rfCode16: ");
Serial.println(rfCode16);
Serial.print("rfCode17: ");
Serial.println(rfCode17);
Serial.print("rfCode18: ");
Serial.println(rfCode18);
Serial.print("rfCode19: ");
Serial.println(rfCode19);
Serial.print("rfCode20: ");
Serial.println(rfCode20);
Serial.print("rfCode21: ");
Serial.println(rfCode21);
Serial.print("rfCode22: ");
Serial.println(rfCode22);
Serial.print("rfCode23: ");
Serial.println(rfCode23);
Serial.print("rfCode24: ");
Serial.println(rfCode24);
Serial.print("rfCode25: ");
Serial.println(rfCode25);
Serial.print("rfCode26: ");
Serial.println(rfCode26);
Serial.print("rfCode27: ");
Serial.println(rfCode27);
Serial.print("rfCode28: ");
Serial.println(rfCode28);
Serial.print("rfCode29: ");
Serial.println(rfCode29);
Serial.print("rfCode30: ");
Serial.println(rfCode30);

Serial.print("rfMessage1: ");
Serial.println(rfMessage1);
Serial.print("rfMessage2: ");
Serial.println(rfMessage2);
Serial.print("rfMessage3: ");
Serial.println(rfMessage3);
Serial.print("rfMessage4: ");
Serial.println(rfMessage4);
Serial.print("rfMessage5: ");
Serial.println(rfMessage5);
Serial.print("rfMessage6: ");
Serial.println(rfMessage6);
Serial.print("rfMessage7: ");
Serial.println(rfMessage7);
Serial.print("rfMessage8: ");
Serial.println(rfMessage8);
Serial.print("rfMessage9: ");
Serial.println(rfMessage9);
Serial.print("rfMessage10: ");
Serial.println(rfMessage10);
Serial.print("rfMessage11: ");
Serial.println(rfMessage11);
Serial.print("rfMessage12: ");
Serial.println(rfMessage12);
Serial.print("rfMessage13: ");
Serial.println(rfMessage13);
Serial.print("rfMessage14: ");
Serial.println(rfMessage14);
Serial.print("rfMessage15: ");
Serial.println(rfMessage15);
Serial.print("rfMessage16: ");
Serial.println(rfMessage16);
Serial.print("rfMessage17: ");
Serial.println(rfMessage17);
Serial.print("rfMessage18: ");
Serial.println(rfMessage18);
Serial.print("rfMessage19: ");
Serial.println(rfMessage19);
Serial.print("rfMessage20: ");
Serial.println(rfMessage20);
Serial.print("rfMessage21: ");
Serial.println(rfMessage21);
Serial.print("rfMessage22: ");
Serial.println(rfMessage22);
Serial.print("rfMessage23: ");
Serial.println(rfMessage23);
Serial.print("rfMessage24: ");
Serial.println(rfMessage24);
Serial.print("rfMessage25: ");
Serial.println(rfMessage25);
Serial.print("rfMessage26: ");
Serial.println(rfMessage26);
Serial.print("rfMessage27: ");
Serial.println(rfMessage27);
Serial.print("rfMessage28: ");
Serial.println(rfMessage28);
Serial.print("rfMessage29: ");
Serial.println(rfMessage29);
Serial.print("rfMessage30: ");
Serial.println(rfMessage30);

Serial.print("rfGroup1: ");
Serial.println(rfGroup1);
Serial.print("rfGroup2: ");
Serial.println(rfGroup2);
Serial.print("rfGroup3: ");
Serial.println(rfGroup3);
Serial.print("rfGroup4: ");
Serial.println(rfGroup4);
Serial.print("rfGroup5: ");
Serial.println(rfGroup5);
Serial.print("rfGroup6: ");
Serial.println(rfGroup6);
Serial.print("rfGroup7: ");
Serial.println(rfGroup7);
Serial.print("rfGroup8: ");
Serial.println(rfGroup8);
Serial.print("rfGroup9: ");
Serial.println(rfGroup9);
Serial.print("rfGroup10: ");
Serial.println(rfGroup10);
Serial.print("rfGroup11: ");
Serial.println(rfGroup11);
Serial.print("rfGroup12: ");
Serial.println(rfGroup12);
Serial.print("rfGroup13: ");
Serial.println(rfGroup13);
Serial.print("rfGroup14: ");
Serial.println(rfGroup14);
Serial.print("rfGroup15: ");
Serial.println(rfGroup15);
Serial.print("rfGroup16: ");
Serial.println(rfGroup16);
Serial.print("rfGroup17: ");
Serial.println(rfGroup17);
Serial.print("rfGroup18: ");
Serial.println(rfGroup18);
Serial.print("rfGroup19: ");
Serial.println(rfGroup19);
Serial.print("rfGroup20: ");
Serial.println(rfGroup20);
Serial.print("rfGroup21: ");
Serial.println(rfGroup21);
Serial.print("rfGroup22: ");
Serial.println(rfGroup22);
Serial.print("rfGroup23: ");
Serial.println(rfGroup23);
Serial.print("rfGroup24: ");
Serial.println(rfGroup24);
Serial.print("rfGroup25: ");
Serial.println(rfGroup25);
Serial.print("rfGroup26: ");
Serial.println(rfGroup26);
Serial.print("rfGroup27: ");
Serial.println(rfGroup27);
Serial.print("rfGroup28: ");
Serial.println(rfGroup28);
Serial.print("rfGroup29: ");
Serial.println(rfGroup29);
Serial.print("rfGroup30: ");
Serial.println(rfGroup30);

  sirenSure = strtoul(sirenTime, NULL, 10) * 1000UL;

  bot.updateToken(telegramToken);

  // This is the simplest way of getting this working
  // if you are passing sensitive information, or controlling
  // something important, please either use certStore or at
  // least client.setFingerPrint
  client.setInsecure();
  
  initWiFi();

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
        // HTTP POST apPass value
        if (p->name() == PARAM_INPUT_12) {
          config["apPass"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST uiName value
        if (p->name() == PARAM_INPUT_13) {
          config["uiName"] = p->value().c_str();
          writeFile(LittleFS, "/config.json", config);
        }
        // HTTP POST uiPass value
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

        //devices
        if (p->name() == PARAM_INPUT_47) {
          devices1["rfCode1"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_48) {
          devices1["rfCode2"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_49) {
          devices1["rfCode3"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_50) {
          devices1["rfCode4"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_51) {
          devices1["rfCode5"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_52) {
          devices1["rfCode6"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_53) {
          devices1["rfCode7"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_54) {
          devices1["rfCode8"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_55) {
          devices1["rfCode9"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_56) {
          devices1["rfCode10"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_57) {
          devices1["rfCode11"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_58) {
          devices1["rfCode12"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_59) {
          devices1["rfCode13"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_60) {
          devices1["rfCode14"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_61) {
          devices1["rfCode15"] = p->value().c_str();
          writeFile(LittleFS, "/devices1.json", devices1);
        }

        if (p->name() == PARAM_INPUT_62) {
          devices2["rfCode16"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_63) {
          devices2["rfCode17"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_64) {
          devices2["rfCode18"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_65) {
          devices2["rfCode19"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_66) {
          devices2["rfCode20"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_67) {
          devices2["rfCode21"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_68) {
          devices2["rfCode22"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_69) {
          devices2["rfCode23"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_70) {
          devices2["rfCode24"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_71) {
          devices2["rfCode25"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_72) {
          devices2["rfCode26"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_73) {
          devices2["rfCode27"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_74) {
          devices2["rfCode28"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_75) {
          devices2["rfCode29"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_76) {
          devices2["rfCode30"] = p->value().c_str();
          writeFile(LittleFS, "/devices2.json", devices2);
        }

        if (p->name() == PARAM_INPUT_77) {
          devices3["rfMessage1"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_78) {
          devices3["rfMessage2"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_79) {
          devices3["rfMessage3"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_80) {
          devices3["rfMessage4"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_81) {
          devices3["rfMessage5"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_82) {
          devices3["rfMessage6"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_83) {
          devices3["rfMessage7"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_84) {
          devices3["rfMessage8"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_85) {
          devices3["rfMessage9"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_86) {
          devices3["rfMessage10"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_87) {
          devices3["rfMessage11"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_88) {
          devices3["rfMessage12"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_89) {
          devices3["rfMessage13"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_90) {
          devices3["rfMessage14"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_91) {
          devices3["rfMessage15"] = p->value().c_str();
          writeFile(LittleFS, "/devices3.json", devices3);
        }

        if (p->name() == PARAM_INPUT_92) {
          devices4["rfMessage16"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_93) {
          devices4["rfMessage17"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_94) {
          devices4["rfMessage18"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_95) {
          devices4["rfMessage19"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_96) {
          devices4["rfMessage20"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_97) {
          devices4["rfMessage21"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_98) {
          devices4["rfMessage22"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_99) {
          devices4["rfMessage23"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_100) {
          devices4["rfMessage24"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_101) {
          devices4["rfMessage25"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_102) {
          devices4["rfMessage26"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_103) {
          devices4["rfMessage27"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_104) {
          devices4["rfMessage28"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_105) {
          devices4["rfMessage29"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_106) {
          devices4["rfMessage30"] = p->value().c_str();
          writeFile(LittleFS, "/devices4.json", devices4);
        }

        if (p->name() == PARAM_INPUT_107) {
          devices5["rfGroup1"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_108) {
          devices5["rfGroup2"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_109) {
          devices5["rfGroup3"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_110) {
          devices5["rfGroup4"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_111) {
          devices5["rfGroup5"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_112) {
          devices5["rfGroup6"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_113) {
          devices5["rfGroup7"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_114) {
          devices5["rfGroup8"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_115) {
          devices5["rfGroup9"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_116) {
          devices5["rfGroup10"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_117) {
          devices5["rfGroup11"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_118) {
          devices5["rfGroup12"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_119) {
          devices5["rfGroup13"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_120) {
          devices5["rfGroup14"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_121) {
          devices5["rfGroup15"] = p->value().c_str();
          writeFile(LittleFS, "/devices5.json", devices5);
        }

        if (p->name() == PARAM_INPUT_122) {
          devices6["rfGroup16"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        if (p->name() == PARAM_INPUT_123) {
          devices6["rfGroup17"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        if (p->name() == PARAM_INPUT_124) {
          devices6["rfGroup18"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }
        if (p->name() == PARAM_INPUT_125) {
          devices6["rfGroup19"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        if (p->name() == PARAM_INPUT_126) {
          devices6["rfGroup20"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        if (p->name() == PARAM_INPUT_127) {
          devices6["rfGroup21"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        if (p->name() == PARAM_INPUT_128) {
          devices6["rfGroup22"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        if (p->name() == PARAM_INPUT_129) {
          devices6["rfGroup23"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        if (p->name() == PARAM_INPUT_130) {
          devices6["rfGroup24"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        if (p->name() == PARAM_INPUT_131) {
          devices6["rfGroup25"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        if (p->name() == PARAM_INPUT_132) {
          devices6["rfGroup26"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        if (p->name() == PARAM_INPUT_133) {
          devices6["rfGroup27"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        if (p->name() == PARAM_INPUT_134) {
          devices6["rfGroup28"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        if (p->name() == PARAM_INPUT_135) {
          devices6["rfGroup29"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        if (p->name() == PARAM_INPUT_136) {
          devices6["rfGroup30"] = p->value().c_str();
          writeFile(LittleFS, "/devices6.json", devices6);
        }

        //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String paramState;
    String paramOutput;

    // GET input1 value on <ESP_IP>/update?output=<paramOutput>&state=<paramState>
    if (request->hasParam(PARAM_INPUT_10) && request->hasParam(PARAM_INPUT_11)) {
      paramOutput = request->getParam(PARAM_INPUT_10)->value();
      paramState = request->getParam(PARAM_INPUT_11)->value();
      //digitalWrite(paramOutput.toInt(), paramState.toInt());

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

  server.on("/rfcodeestate", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", remoteIdGet.c_str());
  });

  server.begin();
  // AT command to set SIM900 to SMS mode
  Serial2.print("AT+CMGF=1\r");
  delay(100);
  // Set module to send SMS data to serial out upon receipt
  Serial2.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);

  if(strcmp(telegramState,"0")){
    String ipAdresi = WiFi.localIP().toString();
    bot.sendMessage(chat_id, "Sistem Başlatıldı! IP adresi: " + ipAdresi);
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
