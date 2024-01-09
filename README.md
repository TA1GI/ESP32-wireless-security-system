# ESP32 Wireless security system

Wireless security system that can be controlled using telegram and sms.

The system is mostly inspired by:https://github.com/atc1441/DiyArduinoESP232AlarmSystem





Turkish

# ESP32 Kablosuz güvenlik sistemi

Merhaba. İsmim Abdullah. Aslında yazılım bilgim çok çok az düzeyde. Yaptıklarımın çoğunu kopyala yapıştır ile yaptım. Elektronik bilgim de çok çok basit devreleri kopya çekerek tasarlayabilecek düzeyde.

Yazdığım kod çok amatör olabilir. Lütfen çekinmeden değişiklikler yapın. 

Sistem aktif durumda iken sisteme eklenen hareket ve kapı pencere sensörü gibi sensörlerden sinyal algılandığında arayüzde belirlenen süre boyunca siren çalıyor. Belirlenen numaraya ya da numaralara sms gönderiyor. Eğer telegram bilgileri girilmişse telegrama mesaj gönderiyor. Belirlenen numaraya arama yapıyor.

Sistem pasif durumda iken sisteme eklenen su seviye sensörü, gaz sensörü gibi sensörlerden aldığı bilgiye göre sms (ve) veya telegramdan mesaj gönderiyor.

Sisteme eklenen kumanda(lar), sms (ve) veya telegram mesajları ile sistem kontrol edilebilir. Bu ayarlar web arayüzünden yapılabilir.

Bu güvenlik sistemini bir arkadaşımın isteği üzerine onun işyeri için geliştirdim. Aslında yabancı bir arkadaşın kodunu geliştirerek yaptım.

Sistem şuradaki proje temel alınarak yapılmıştır: https://github.com/atc1441/DiyArduinoESP232AlarmSystem

Daha sonra chatgpt'den yardım alarak ve birçok projeyi inceleyerek daha da ileri düzeye taşıdım. Hala daha çok geliştirilmesi gerektiğini düşünüyorum ama bu hali ile bile bence çok kullanışlı.

Sistem için bir pcb tasarımı yaptım ama çok yetersiz gördüğüm için onu paylaşmadım. Breadboard üzerinde test edilebilir ama (ben çok sıkıntı çektim) hassas bileşenler olduğu için jumper kablolarla test edilmesini tavsiye ederim. Ben özel yaptığım jumper kablolar ile test ediyorum.

Sistemden biraz bahsedeyim. Sistemin kodunu dört şekilde paylaştım. Aslında hepsinin yaptığı iş aynı.

**_alarm_sms_arduino_** klasöründeki kod sadece Arduino uno, Arduino nano gibi kartlarda kullanmak için. Bir web arayüzü yok. Tüm ayarlar kod içerisinden yapılır. Telegram özeliği yok. Sms gönderip arama yapabilir.

**_alarm_sms_esp32_** klasöründeki kod esp32 için. (esp8266 da test etmedim ama muhtemelen çalışır.) Bir web arayüzü yok. Tüm ayarlar kod içerisinden yapılır. Telegram özeliği yok. Sms gönderip arama yapabilir.

**_alarm_telegram_sms_** klasöründeki kod esp32 için. (esp8266 da test etmedim ama muhtemelen çalışır.) Bir web arayüzü yok. Tüm ayarlar kod içerisinden yapılır. Sms gönderip arama yapabilir. Telegram özelliği var.

**_alarm_ui_** klasöründeki kod esp32 için. (esp8266 da test etmedim ama muhtemelen çalışır.) Bir web arayüzü var. Tüm ayarlar web arayüzünden yapılır. Sms gönderip arama yapabilir. Telegram özelliği var. Web arayüzü sayesinde başka özellikleri de var.

Burada yaptığım anlatımlar genelde alarm_ui klasörüne hitap ediyor.

Sistem kablosuz olarak çalışıyor. Temelinde esp32 var. Arduino kodu ile çalışıyor. Telegram ile kontrol edilebiliyor. Sms ile kontrol edilebiliyor. Web arayüzü ile ayarları yapılabiliyor. Sms gönderebiliyor. Arama yapabiliyor.


Sistemin ayarları bir web arayüzü sayesinde yapılabiliyor. Detaylı fotoğraflar ekleyeceğim. 

Sistem ilk açıldığında sisteme girilmiş olan modem bilgisi varsa modeme bağlanıyor. Modem tarafından atanan ip adresine girildiğinde kullanıcı adı şifre girilerek sistem arayüzüne ulaşılıyor. Bu ip adresinin ne olduğunu ilk başta öğrenmek için iki yol var modeminizin arayüzünden öğrenebilir yada seri monitörden öğrenebilirsiniz. Telegram bilgisi girildikten sonra sistem her açıldığında telegrama modemin atadığı ip adres bilgisi gönderiliyor.

Modem bilgisi girilmemişse sistem kendisi modem gibi (access point modu ile) açılıyor. Telefon veya bilgisayardan sistem wifi'ına bağlanılıyor. Şuanda kodda belirttiğimiz 192.168.4.22 (koddaki ilgili yerden değiştirilebilir) ip adresine tarayıcıdan girildiğinde kullanıcı adı ve şifre girerek sistemin web arayüzüne ulaşılabiliyor.

Sistem ayrıca Telegramdan da kontrol edilebiliyor. Öncelikle Telegramdaki BotFather ile bir bot oluşturmanız gerekiyor. (Tamamen ücretsiz.) Nasıl yapılacağının bilgisini internetten kolaylıkla bulabilirsiniz. Size verilen TOKEN ve ID bilgilerini Web arayüzünden (web arayüzü olmayan versiyonlarda kod üzerinden) sisteme girmelisiniz.

Telegramdan sistemi kontrol etmek için sistem botunuza */baslat* yada */yardim* yazarak kontrol listesine ulaşabilirsiniz.

/aktif : Sistemi Devreye Alır

/pasif : Sistemi Devre Dışı Bırakır

/durum : Sistem Durumu Bildirilir

/liste : Bilinen tüm cihazları listeler

/kontrol : Kontrol edilebilir cihazları listeler

/dinle : RF kodları dinleme-algılama modunu değiştirir

/reboot : Sistemi yeniden başlatır

/baslat veya /yardim : Komutları gösterir

Sistemde rf alıcı var. Ben sensörleri daha kolay bulunabilir olduğu için 433mhz alıcı, sensörler ve kumandalar kullanıyorum. Bu alıcı sayesinde sisteme kablosuz sensörler ekleyebiliyoruz. Hareket sensörü, kapı pencere sensörü, su basma sensörü, duman sensörü, çeşitli gaz sensörleri gibi uzayıp gidiyor bu liste. Şu an sisteme kumandalar da dahil olmak üzere toplam 30 cihaz eklenebiliyor. Nasıl eklendiği sistem arayüzünde anlatılıyor. Sisteme eklenen kumandalar sayesinde sistem kumandalar ile açılıp kapanabiliyor. 

Sitemde bir de rf verici bulunuyor. Bu verici sayesinde sistem ile kepenk gibi rf kumanda ile kontrol edilen cihazlar kontrol edilebiliyor.

Sistemde gsm modülü var. Ben sim800c kullandım. (Bozulduğunda sim800l kullandım) Bu modül sayesinde sistemden sms ve arama alınabiliyor. Sms ile sistem uzaktan kontrol edilebiliyor. Sms ile sisteme eklenen rf kumanda ile çalışan cihazlar kontrol edilebiliyor. Bunun için bir kütüphane kullanmadım. Direk AT komutlarını kullandım.

Sistemde birde siren var. Ben araç alarmlarında kullanılan sirenlerden kullandım. Sireni sürmek için irfz44n kullandım.

Sistemin elektrikler kesildiğinde de çalışması için 12v 1.2ah akü kullandım. Elektrikler kesildiğinde direk aküye geçmesini röle ile sağladım. (çok sağlıklı olmadı) Aküyü şarj etmek için XH-M603 Akü Şarj Kontrol modülü kullandım. Aşırı deşarj için XH-M609 modülü kullandım. Ama bu iki modül sağlıklı çalışmadı. 

Aslında sistem için güzel bir pcb gerekiyor. Yani tüm modülleri, besleme katı, akü şarj ve aşırı deşarj, siren çıkışı, durum ledleri çıkışı gibi tüm bileşenleri tek kartta barındıran bir devreye ihtiyacım var. Bu konuda desteğe açığım.


**Gerekli kütüphaneler:**

ESPAsyncWebServer

AsyncTCP

AsyncElegantOTA

FS

LittleFS

ArduinoJson

RCSwitch https://github.com/sui77/rc-switch

UniversalTelegramBot  https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot


## Temelde sistemin bağlantı şeması:
![schematic](https://github.com/TA1AUB/ESP32-wireless-security-system/assets/11720438/8b77be8f-a3ee-49f6-b810-279d69cf4e5c)



## Web arayüz görüntüleri
![1](https://github.com/TA1AUB/ESP32-wireless-security-system/assets/11720438/c154d649-65ef-466a-9b93-d762864b9542)
![2](https://github.com/TA1AUB/ESP32-wireless-security-system/assets/11720438/57bb3df1-4be6-4b3e-bc84-3bdb4fa384ca)
![3](https://github.com/TA1AUB/ESP32-wireless-security-system/assets/11720438/845f913c-56ed-4ac4-b37b-6bbb2cd4cc6a)
![4](https://github.com/TA1AUB/ESP32-wireless-security-system/assets/11720438/311cb592-9252-4d55-8635-d172df7d3e21)
![5](https://github.com/TA1AUB/ESP32-wireless-security-system/assets/11720438/f1d40e25-996e-4bac-bf7e-ec82b6968c53)
![6](https://github.com/TA1AUB/ESP32-wireless-security-system/assets/11720438/2e1c2160-3589-4441-b50c-4de1ee9917d3)
![7](https://github.com/TA1AUB/ESP32-wireless-security-system/assets/11720438/68e264b5-f017-404a-826a-e809b21ac166)
![8](https://github.com/TA1AUB/ESP32-wireless-security-system/assets/11720438/935147ed-74d6-417a-b765-a8d64dd151a1)
![9](https://github.com/TA1AUB/ESP32-wireless-security-system/assets/11720438/4ebdc96f-7e5a-4df6-b994-3a87274088dd)
![10](https://github.com/TA1AUB/ESP32-wireless-security-system/assets/11720438/2d541c97-8c68-4565-8a6e-a7909360e121)
![11](https://github.com/TA1AUB/ESP32-wireless-security-system/assets/11720438/4435061a-e8c8-4c20-8def-d0408af2a8da)
