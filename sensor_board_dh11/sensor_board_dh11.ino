/**
 * Controls the following sensors
 *  DHT11 - temperature and humidity sensor
 *        - cheap price, a bit slow
 *        - cannot measure below zero C
 *  Freescale MPX4115A - Absolute pressure
 * 
 * Data can be wired over the Ethernet jack
 */

//Ethernet part
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
EthernetServer server(80);

#include "DHT.h"
#define DHTPIN 5
#define DHTTYPE DHT11 //cheaper variety, cannot go below 0 C

DHT dht(DHTPIN, DHTTYPE);

#define TH_SNSR_WAIT_TIME 1000U
#define PRS_SNSR_WAIT_TIME 1000U

#define USB_BDR 9600U

//these are for the geiger counter
#define ADJUST_COEFFICIENT 1000U
#define update_time 100U   // update time 
#define SAMPL2UPDAT 3  // initial sampling time 

int count = 0;
unsigned long sampling_time = update_time*SAMPL2UPDAT;
unsigned long total_time = sampling_time+update_time;
float weight=( (float) sampling_time)/((float) total_time);
unsigned long geig_count=0;
unsigned long lastMills = millis();
unsigned long tratio = SAMPL2UPDAT;
int sievert;

// analog input pin for MPX4115A 
int pressurePin = 0;


float temp_c;
float humidity;
int val;
float pkPa; // pressure in kPa
float slp;
float elevation = 212.0; // elevation of aizuwakamatsu city

unsigned long previousMillis = 0;        // will store last time samples were taken
long interval = 10000;           // sampling interval

void setup()
{
   
   Serial.begin(USB_BDR); // Open serial connection to report values to host
   Serial.println("Starting up");
   
   startup_ethernet();
   dht.begin();
   // geiger counter setup
   int CounterPin = 2;
   int CounterPinInterruptNo = 0 ;
   pinMode( CounterPin, INPUT ) ;
   digitalWrite( CounterPin, HIGH ) ;//turn on pullup resistors
   attachInterrupt( CounterPinInterruptNo, countInterruptFunc, FALLING );
   Serial.println("Waiting for geiger counter to take long sample");
   initialize_geiger_sample();
   Serial.println("First sample over");
}

void loop()
{
  update_geiger_sample();
  sample_all_data();
  print_sensor_data();
  EthernetClient client = server.available(); 
  wire_sensor_data(client);
  
}

void sample_all_data(){
  
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {  
    sample_dht11_data(); 
    sample_pressure_sensor(elevation);
    previousMillis = currentMillis;  
  }
 
}

void print_sensor_data()
{
   // Print the values to the serial port
  /*Serial.print("Temperature: ");
  Serial.print(temp_c, DEC);
  Serial.print("C  ");
  //Serial.print(temp_f, DEC);
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%"); */
  
  Serial.print("Pressure: ");
  Serial.print(pkPa, DEC);
  Serial.println("kPa  ");
  Serial.print("SLP ");
  Serial.println(slp);
  
  Serial.print("Radiation:");
  Serial.print(geig_count);
  Serial.println(" cpm");
  sievert = count_to_sievert(geig_count, ADJUST_COEFFICIENT);
  Serial.print(sievert) ;
  Serial.print(" > ");
  Serial.println(to_string(sievert));
  
  Serial.print("LastMills: ");
  Serial.println(lastMills);
  
}

void print_ip_address()
{
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
}

