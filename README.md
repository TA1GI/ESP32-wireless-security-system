# ESP32-wireless-security-system
Wireless security system that can be controlled using telegram and sms.

The system is mostly inspired by:https://github.com/atc1441/DiyArduinoESP232AlarmSystem





Turkish

Sistem şuradaki proje temel alınarak yapılmıştır : https://github.com/atc1441/DiyArduinoESP232AlarmSystem

Sistemde Kullanılan bileşenler:

RF alıcı

RF verici

GSM modülü (sim800 vs)


Sisteme eklenen kumanda, sms (ve) veya telegram mesajları ile sistem kontrol edilir.

Sistem aktif haldeyken sensörlerden bilgi aldığında belirlenen süre boyunca siren çalar, belirlenen nuramaya (numaralara) sms gönderir (ve) veya telegramdan bilgi verir.

İsteğe göre sistem pasif haldeyken de sisteme eklenen su seviye sensörü, gaz sensörü gibi sensörlerden aldığı bilgiye göre sms (ve) veya telegramdan mesaj gönderir.

Rf verici sayesinde sms veya telegram ile kepenk gibi rf sinyal ile çalışan cihazlar kontrol edilebilir.

![schematic](https://github.com/TA1AUB/ESP32-wireless-security-system/assets/11720438/895bad38-4966-45a4-9775-d3abdeb0f536)

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
