#include <SoftwareSerial.h>

#include <RCSwitch.h>// https://github.com/sui77/rc-switch

//Mesaj gönderilecek ve aranacak telefon numarası
const String PHONE = "+905555555555";

unsigned long sirenzaman = 0;

RCSwitch rfReceiver = RCSwitch();

const int rfReceiverPin = 0; //pin D2, works. Connect RFReceiver module to this pin. Power it from 3V3 (not from 5V because of risk of killing GPIO pin).
const int rfTransmitterPin = 10; //rf alıcısının pini

//Durum ledleri için değişken tanımlıyoruz
#define sistemAktifLed 5
#define sistemPasifLed 6

//Siren için değişken tanımlıyoruz
#define siren 13

//Sirenin çalmaya devam edeceği süre (ms)
#define sirenCalmaSuresi 30000

// Variable to store text message
String textMessage;

typedef struct {
  int remoteID;
  String remoteName;
  int group; // 0 - disarming; 1 - arming; 2 - reported 24h, 3 - reported when armed
  long lastTime;
} device_struct;


//SIM800 TX is connected to Arduino D3
#define SIM800_TX_PIN 4

//SIM800 RX is connected to Arduino D4
#define SIM800_RX_PIN 3

SoftwareSerial serialSIM800(SIM800_TX_PIN, SIM800_RX_PIN);

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

bool enable_system = true; //start armed

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
        //*****Sistem kumanda ile pasif duruma getirildiğinde sms göndermek için ayarlama yapıyoruz.(Bunu istemiyorsanız bu bölümü silin)
        String message = devices[currentDevice].remoteName;
        sendSMS(message);
        //*****
      }

      if (devices[currentDevice].group == 1) {
        enable_system = true;
        digitalWrite(sistemAktifLed, HIGH);
        digitalWrite(sistemPasifLed, LOW);
      }

      if (devices[currentDevice].group == 2) {
        String message = devices[currentDevice].remoteName;
        sendSMS(message);
      }

      if ((devices[currentDevice].group == 3) && enable_system) {
        sirenzaman = millis();
        digitalWrite(siren, HIGH);
        Serial.println("Sensör tetiklendiği için siren çalmaya başladı");
        String message = devices[currentDevice].remoteName;
        sendSMS(message);
        delay(3000);

        serialSIM800.println("ATD+ " + PHONE + ";");
        //Serial2.println("ATH");
        // Set module to send SMS data to serial out upon receipt
        serialSIM800.print("AT+CNMI=2,2,0,0,0\r");
      }

      if (devices[currentDevice].group == 4) {
        digitalWrite(siren, LOW);
        Serial.println("Susturma tuşuna basıldığı için siren susturuldu");
      }

    }
  } else {
    Serial.println(String(remoteID) + ": " + "bilinmeyen kod");
  }
}

// Function that sends SMS
void sendSMS(String message) {
  // AT command to set SIM900 to SMS mode
  serialSIM800.println("AT+CMGF=1\r\n");
  delay(100);

  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  serialSIM800.println("AT+CMGS=\"" + PHONE + "\"\r\n");
  delay(500);

  // Send the SMS
  serialSIM800.println(message);
  delay(500);

  // End AT command with a ^Z, ASCII code 26
  serialSIM800.println((char)26);
}

void SMScontrol() {

  //Sms ile sistemi açmak için ayarlıyoruz
  if ((textMessage.indexOf("#AC") >= 0) && !enable_system) { //Gönderilecek sms içeriği # dahil
    enable_system = true;
    digitalWrite(sistemAktifLed, HIGH);
    digitalWrite(sistemPasifLed, LOW);
    String message = "Sistem Sms ile Aktif Duruma Getirildi";
    sendSMS(message);
    textMessage = "";
  }

  //Sms ile sistemi kapatmak için ayarlıyoruz
  if ((textMessage.indexOf("#KAPAT") >= 0) && enable_system) { //Gönderilecek sms içeriği # dahil
    enable_system = false;
    digitalWrite(sistemAktifLed, LOW);
    digitalWrite(sistemPasifLed, HIGH);
    digitalWrite(siren, LOW);
    Serial.println("Sms ile sistem pasifleştirildiği için siren susturuldu");
    String message = "Sistem Sms ile Pasif Duruma Getirildi";
    sendSMS(message);
    textMessage = "";
  }

  //Sms ile sistemin durumunu sorguluyoruz
  if (textMessage.indexOf("#DURUM") >= 0) { //Gönderilecek sms içeriği # dahil
    if (enable_system) {
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
  if (textMessage.indexOf("#KEPENK1") >= 0) { //Gönderilecek sms içeriği # dahil
    rfReceiver.send("000"); //Kepenk için gönderilecek rf kodunu girin
    String message = "SMS iLE 1. KEPENKE KOMUT GONDERiLDi.";
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }

  //Sms ile 2. kepenke komut gönderiyoruz
  if (textMessage.indexOf("#KEPENK2") >= 0) { //Gönderilecek sms içeriği # dahil
    rfReceiver.send("000"); //Kepenk için gönderilecek rf kodunu girin
    String message = "SMS iLE 2. KEPENKE KOMUT GONDERiLDi.";
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }

  //Sms ile 3. kepenke komut gönderiyoruz
  if (textMessage.indexOf("#KEPENK3") >= 0) { //Gönderilecek sms içeriği # dahil
    rfReceiver.send("000"); //Kepenk için gönderilecek rf kodunu girin
    String message = "SMS iLE 3. KEPENKE KOMUT GONDERiLDi.";
    sendSMS(message);
    delay(1000);
    textMessage = "";
  }

}

void setup() {
  //Arduino ile SIM800 seri haberleşmesi başlatılıyor
  Serial.begin(9600);
  serialSIM800.begin(9600);

  rfReceiver.enableReceive(rfReceiverPin);
  rfReceiver.enableTransmit(rfTransmitterPin);

  //Siren pininin çıkış pini olduğunu belirtiyoruz
  pinMode (siren, OUTPUT);

  //Durum ledlerini ayarlıyoruz
  pinMode (sistemAktifLed, OUTPUT);
  pinMode (sistemPasifLed, OUTPUT);

  digitalWrite(sistemAktifLed, HIGH);
  digitalWrite(sistemPasifLed, LOW);

  // Sim800ün çalışması için 5 saniye bekliyoruz(Bunu yapmazsak aşağıdaki sim800ü sms moduna alma işlemi gerçekleşmez)
  delay(5000);

  // AT command to set SIM900 to SMS mode
  serialSIM800.print("AT+CMGF=1\r");
  delay(100);
  // Set module to send SMS data to serial out upon receipt
  serialSIM800.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);

  Serial.println("Sistem başlatıldı");
}

void loop() {
  //Sim800den seri monitöre komut göndermek için ayarlama yapıyoruz. Bu arada textMessage değişkenini sim800den gelen komut ile sabitliyoruz
  if (serialSIM800.available() > 0) {
    textMessage = serialSIM800.readString();
    Serial.print(textMessage);
  }

  //Seri monitörden sim800e komut göndermek için ayarlıyoruz. Bu kod test aşamasında gerekli. Daha sonra kaldıralım
  if (Serial.available()) {
    serialSIM800.write(Serial.read());
  }

  SMScontrol();

  if (millis() - sirenzaman > sirenCalmaSuresi) {
    digitalWrite(siren, LOW);
  }

  // listen to rfReceiver
  if (rfReceiver.available()) {
    parseRemote(rfReceiver.getReceivedValue());
    rfReceiver.resetAvailable();
  }
}
