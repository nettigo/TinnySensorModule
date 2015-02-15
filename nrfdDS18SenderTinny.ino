
#include <SPI85.h>
#include <Mirf.h>
#include <MirfHardwareSpi85Driver.h>

#include <OneWire.h>
#include <DS18B20.h>

#define CE 7
#define CSN 3
#define LED  1

#define ONEWIRE_PIN 0

byte address[8] = {0x28, 0xE6, 0x3C, 0xCC, 0x5, 0x0, 0x0, 0x3E};

OneWire onewire(ONEWIRE_PIN);
DS18B20 sensor(&onewire);





void setup()
{
  Mirf.cePin = CE;
  Mirf.csnPin = CSN;
  Mirf.spi = &MirfHardwareSpi85;
  Mirf.init();
  
  Mirf.payload = sizeof(float);
   
  Mirf.config();
  
  sensor.begin();
  sensor.request(address);
  
  power_setup();

}
void loop()
{
  

if (sensor.available())
  {

    float temperature = sensor.readTemperature(address);
    Mirf.setTADDR((byte *)"SERV0");
    Mirf.send((byte *) &temperature);
    while (Mirf.isSending());
    sensor.request(address);
    //pulseLED();
  }
//  delay(300);

  power_down();
  /*
    ADCSRA &= ~(1<<ADEN); //Disable ADC, saves ~230uA
    digitalWrite(CE, LOW);
    for (byte i=0; i<4; i++) {
      setup_watchdog(4); //Setup watchdog to go off after 8sec
      sleep_mode(); //Go to sleep! Wake up 1sec later and check water
    }  
      */
    ADCSRA |= (1<<ADEN); //Enable ADC
   digitalWrite(CE, HIGH);
   
/*
Mirf.setTADDR((byte *)"SERV0");
  Mirf.send((byte *) &heartbeat);
    while (Mirf.isSending());
//  pulseLED();
*/
} 

