/*
  GPS Location

  This sketch uses the GPS to determine the location of the board
  and prints it to the Serial Monitor.

  Circuit:
   - MKR board
   - MKR GPS Shield attached via I2C cable.
*/

#include <Arduino_MKRGPS.h>

boolean S = false; //LOW tout le temps, sauf quand V passe la limite//
boolean dansZone = true;
boolean dansZoneOld = true;

float X, Y;  //latitude actuelle, longitude actuelle

const float Xmax = 48.8450559; //latitude max
const float Xmin = 48.8450290; //latitude min
const float Ymax = 2.3564313; //longitude max
const float Ymin = 2.3565972; //longitude min

void TesteZone(float X, float Y) {
    if (X <= Xmax && X >= Xmin && Y <= Ymax && Y >= Ymin) {
        dansZone = true;  
    } else {
        dansZone = false;  
    }      
    
    if (dansZone != dansZoneOld) {
       S = true;
       dansZoneOld = dansZone; 
    }
}

void setup() {
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // If you are using the MKR GPS as shield, change the next line to pass
  // the GPS_MODE_SHIELD parameter to the GPS.begin(...)
  if (!GPS.begin()) {
    Serial.println("Failed to initialize GPS!");
    while (1);
  }
  if (!GPS.available()) {
    Serial.println("GPS not available!");
  }
}

void loop() {
  // check if there is new GPS data available
  if (GPS.available()) {
    // read GPS values
    float latitude   = GPS.latitude();
    float longitude  = GPS.longitude();
    float altitude   = GPS.altitude();
    float speed      = GPS.speed();
    int   satellites = GPS.satellites();

    // print GPS values
    Serial.print("Location: ");
    Serial.print(latitude, 7);
    Serial.print(", ");
    Serial.println(longitude, 7);

    // Serial.print("Altitude: ");
    // Serial.print(altitude);
    // Serial.println("m");

    // Serial.print("Ground speed: ");
    // Serial.print(speed);
    // Serial.println(" km/h");

    // Serial.print("Number of satellites: ");
    // Serial.println(satellites);

    // Serial.println();

    //Delimitation 
    TesteZone(latitude, longitude);
    Serial.println("Est-il dans ZONE ?");
    if (!dansZone) {
    Serial.print(dansZone);
    Serial.print("--> NON");
    }
    delay(300);
  }
}