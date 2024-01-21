"""
bu dosya cpp ve gui arasında olucak yani 
kullanıcı komutu buraya girip 
kontrol dosyasıdır özetle
"""

import schedule
import time
import subprocess
from datetime import datetime

#belirtilen saatlere göre sistem durumunu kontrol
#sistem aktif ise sistemi durdur eklenecek

sistem_aktif = False
#sistem kapalı başlar

def saatigoster():
    print(datetime.now())
    time.sleep(1)
    saatigoster()

#iki buton eklenebilir 
#baslat_button = Button(text="Başlat", command=baslat)
#durdur_button = Button(text="Durdur", command=durdur)

def baslat():
    global sistem_aktif
    if not sistem_aktif:
        print(f"{datetime.now()} - Sistem başlatılıyor...")
        subprocess.run(["/cpp/PORJEYOLUEKLENECEK"])
        sistem_aktif = True

def durdur():
    global sistem_aktif
    if sistem_aktif:
        print(f"{datetime.now()} - Sistem durduruluyor...")
        #c++ için kod eklenecek
        subprocess.run(["/cpp/PORJEYOLU EKLENECEK"])
        sistem_aktif = False

def kontrol():
    global sistem_aktif
    if sistem_aktif:
        print(f"{datetime.now()} - Sistem aktif")
    else:
        print(f"{datetime.now()} - Sistem kapalı")
        baslat()
    time.sleep(1)

def set_hours(baslamasaati, durmasaati):
    schedule.every().day.at(baslamasaati).do(baslat)
    schedule.every().day.at(durmasaati).do(durdur)
##günler için özellik eklenebilir 
#haftanın belirli günleri için ayarlanabilir
#sabah, ogle, aksam, gece 
#sabah_start = "06:00"
#sabah_stop = "10:00"
    

#Kullanıcı arayüzüne inputlar için alan gerek 
sabahb = input("Sabah başlama saati (HH:MM): ")
sabahd = input("Sabah bitiş saati (HH:MM): ")

ogleb = input("Öğle başlama saati (HH:MM): ")
ogled = input("Öğle bitiş saati (HH:MM): ")

aksamb = input("Akşam başlama saati (HH:MM): ")
aksamd = input("Akşam bitiş saati (HH:MM): ")

geceb = input("Gece başlama saati (HH:MM): ")
geced = input("Gece bitiş saati (HH:MM): ")

set_hours(sabahb, sabahd)
set_hours(ogleb, ogled)
set_hours(aksamb, aksamd)
set_hours(geceb, geced)

# Programın ana döngüsü
while True:
    schedule.run_pending()
    time.sleep(1)
