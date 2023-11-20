#include <SPI.h>
#include <LoRa.h>
#include <Arduino_MKRGPS.h>

float latitude;
float longitude;
float altitude;
float speed;
float satellites;

byte localAddress = 0xBB;  //address of this device
byte destination = 0xFF;   //where we are sending data to

void setup() {

  // initialize serial communications and wait for port to open:
  Serial.begin(9600);

    while (!Serial)
    if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  if (!GPS.begin()) {
    Serial.println("Failed to initialize GPS!");
    while (1);
  }
}

void loop() {
  // check if there is new GPS data available
  if (GPS.available()) {

    // read GPS values
    latitude   = GPS.latitude();
    longitude  = GPS.longitude();
    altitude   = GPS.altitude();
    speed      = GPS.speed();
    satellites = GPS.satellites();

    // print GPS values
    printValues();
    
    // Create and send LoRa packet
    LoRa_send();
  }
  delay(1);
}

//function to send information over LoRa network
void LoRa_send() {
    LoRa.beginPacket();  //creates a LoRa packet
    LoRa.write(destination);  //destination address
    LoRa.print("LAT: ");
    LoRa.print(latitude);
    LoRa.print(" LONG: ");
    LoRa.print(longitude);
    LoRa.endPacket(); //sends the LoRa packet
    delay(10000); //a 10 second delay to limit the amount of packets sent
}

//function that prints all readings in the Serial Monitor
void printValues() {
  Serial.print("Location: ");
  Serial.print(latitude, 7);
  Serial.print(", ");
  Serial.println(longitude, 7);
  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println("m");
  Serial.print("Ground speed: ");
  Serial.print(speed);
  Serial.println(" km/h");
  Serial.print("Number of satellites: ");
  Serial.println(satellites);
  Serial.println();
}