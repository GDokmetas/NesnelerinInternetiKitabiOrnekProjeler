# Uygulama Yazarı : AgustinP 
# https://www.hackster.io/AgustinP
#!/usr/bin/python

import time, mraa
from ubidots import ApiClient



for i in range(0,5):
    try:
        print "Ubidots Jeton isteniyor"
        api = ApiClient('abebasfaf4e14d195c0044fcasdfdf9dsfab9d653af3')
        break
        # BURAYA UBIDOTS API ANAHTARINI MUTLAKA YAZIN!!!!!!!!
    except:
        print "Internet Baglantisi Yok Tekrar Baglaniyor..."
        time.sleep(5)

a0 = mraa.Aio(0)
a1 = mraa.Aio(1)

while(1):
    api.save_collection([{'variable': '558073727625425555af27e4','value':a0.read()}, {'variable': '5580737876254257514be1e6','value':a1.read()}])
