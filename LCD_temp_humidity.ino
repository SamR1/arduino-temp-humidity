// Librairies for LCD Screen
#include <LiquidCrystal.h> 

// for DS18B20 sensor
#include <OneWire.h>
#include <DallasTemperature.h>

// and for DHT11 sensor
#include <idDHT11.h>

// LCD Screen connected to pins 2, 3, 4, 5, 10 and 11
LiquidCrystal lcd(11,10,5,4,3,2);

// Sensor connected to pin 7
#define DS18B20 7
OneWire ourWire(DS18B20);
DallasTemperature sensors(&ourWire);

//Digital pin for comunications
int idDHT11pin = 18; 
//interrupt number (must be the one that use the previus defined pin (see table below)
/*
 Board            int.0   int.1   int.2   int.3   int.4   int.5
 Uno, Ethernet    2       3
 Mega2560         2       3       21      20      19      18
 Leonardo         3       2       0       1
 Due            (any pin, more info http://arduino.cc/en/Reference/AttachInterrupt)
 */
int idDHT11intNumber = 5; 

//declaration
void dht11_wrapper(); // must be declared before the lib initialization

// Lib instantiate
idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);

void setup()
{
   lcd.begin(16,2); // 16 characters by 2 line display
   lcd.write("Init de l'ecran");

   delay(5000); 
   // start reading
   sensors.begin();

   // USB connection init
   Serial.begin(9600);
}
 
// This wrapper is in charge of calling 
// must be defined like this for the lib work
void dht11_wrapper() {
  DHT11.isrCallback();
} 
  
void loop() {
  
   lcd.clear();  // places the cursor in the top left
   delay(10);    

   DHT11.acquire();
   while (DHT11.acquiring())
     ;
   int result = DHT11.getStatus();
   switch (result)
   {
   case IDDHTLIB_OK: 
     Serial.print("OK,"); 
     break;
   case IDDHTLIB_ERROR_CHECKSUM: 
     Serial.print("Error - Checksum error,"); 
     break;
   case IDDHTLIB_ERROR_ISR_TIMEOUT: 
     Serial.print("Error - ISR Time out error,"); 
     break;
   case IDDHTLIB_ERROR_RESPONSE_TIMEOUT: 
     Serial.print("Error - Response time out error,"); 
     break;
   case IDDHTLIB_ERROR_DATA_TIMEOUT: 
     Serial.print("Error - Data time out error,"); 
     break;
   case IDDHTLIB_ERROR_ACQUIRING: 
     Serial.print("Error - Acquiring,"); 
     break;
   case IDDHTLIB_ERROR_DELTA: 
     Serial.print("Error - Delta time to small,"); 
     break;
   case IDDHTLIB_ERROR_NOTSTARTED: 
     Serial.print("Error - Not started,"); 
     break;
   default: 
     Serial.print("Unknown error,"); 
     break;
   }
     
   // read temperature and output via serial
   Serial.print(DHT11.getHumidity(), 0);
   Serial.print(",");
   Serial.print(DHT11.getCelsius(), 0);
   Serial.print(",");
   sensors.requestTemperatures();
   Serial.print(sensors.getTempCByIndex(0));
   Serial.println("");
    
   // display on LCD screen
   lcd.write("Temp. : ");
   lcd.setCursor(8, 0);
   lcd.print(sensors.getTempCByIndex(0));
   lcd.setCursor(14, 0);
   lcd.print("*C");
   lcd.setCursor(0, 1);
   lcd.print("Humi. : ");
   lcd.setCursor(8, 1);
   lcd.print(DHT11.getHumidity(), 0);
   lcd.setCursor(14, 1);
   lcd.print("%");
   
   // reading every 10 minutes
   delay(60000);   

}
