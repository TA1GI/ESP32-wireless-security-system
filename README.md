# ESP32-wireless-security-system
Wireless security system that can be controlled using telegram and sms.

The system is mostly inspired by:https://github.com/atc1441/DiyArduinoESP232AlarmSystem

Turkish
Sistem şuradaki proje temel alınarak yapılmıştır. 
https://github.com/atc1441/DiyArduinoESP232AlarmSystem

Sistemde Kullanılan bileşenler:
RF alıcı
RF verici
GSM modülü (sim800 vs)


Sisteme eklenen kumanda, sms (ve) veya telegram mesajları ile sistem kontrol edilir.
Sistem aktif haldeyken sensörlerden bilgi aldığında belirlenen süre boyunca siren çalar, belirlenen nuramaya (numaralara) sms gönderir (ve) veya telegramdan bilgi verir.
İsteğe göre sistem pasif haldeyken de sisteme eklenen su seviye sensörü, gaz sensörü gibi sensörlerden aldığı bilgiye göre sms (ve) veya telegramdan mesaj gönderir.
Rf verici sayesinde sms veya telegram ile kepenk gibi rf sinyal ile çalışan cihazlar kontrol edilebilir.
