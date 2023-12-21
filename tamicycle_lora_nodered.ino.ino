#include <MKRWAN.h>
#include <HX711.h> //Poids
#include <Arduino_MKRGPS.h>

// Pins pour capteur de poids
#define LOADCELL_DOUT_PIN  1
#define LOADCELL_SCK_PIN  0

LoRaModem modem;

//Liaison avec The Things Network
String appEui = "0000000000000000";
String appKey = "EBC127F8D66B753D4E6112CF5F26FE1D";
String devAddr;
String nwkSKey;
String appSKey;

// POIDS
HX711 scale;
float calibration_factor = 21067; // for 16,6 -> 21067 and for ~93,5 -> 21387
float meas, p;

float real_meas = 134.20;
float pourcentage = 15.38;        //la charge de la batterie en pourcentage
float latitude =   55.8450559;
float longitude  = 7.3556285;

// Capteur de niveau
int liquidLevel = 1;

// variables pour stocker les valeurs mesurées en 16bits
short var_battery;
short var_poids;
short var_liquide;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);//Wait before accessing Sensor

  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  if (modem.version() != ARDUINO_FW_VERSION) {
    Serial.println("Please make sure that the latest modem firmware is installed.");
    Serial.println("To update the firmware upload the 'MKRWANFWUpdate_standalone.ino' sketch.");
  }

  int mode = 1;
  int connected;
  if (mode == 1) {
    appKey.trim();
    appEui.trim();
    connected = modem.joinOTAA(appEui, appKey);
  }

  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }

  delay(1000);

  /* Batterie */
  Serial.print("Pourcentage batterie : ");  // debug value
  Serial.println(pourcentage);
  var_battery = (short)(pourcentage*100);
  Serial.print("Valeur batterie envoyer  : ");
  Serial.println(var_battery);

  // Serial.println("HX711 calibration sketch");
  // Serial.println("Remove all weight from scale");
  // Serial.println("After readings begin, place known weight on scale");
  // Serial.println("Press + or a to increase calibration factor");
  // Serial.println("Press - or z to decrease calibration factor");
  
  // scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  // scale.set_scale();
  // scale.tare(); //Reset the scale to 0
  
  // long zero_factor = scale.read_average(); //Get a baseline reading
  // Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  // Serial.println(zero_factor);  
  // // Pin capteur de niveau
  // pinMode(A0, INPUT);

  // scale.set_scale(calibration_factor); //Adjust to this calibration factor
  // Serial.print("Reading: ");
  // meas = scale.get_units(10);
  // Serial.print(meas);
  // p = meas/10;
  // Serial.print(p);
  // Serial.print(" kg "); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  // real_meas = p*1000;
  // Serial.println(" real_ms: ");
  // Serial.println(real_meas);
  // Serial.print(" g "); //Change this to g
  // Serial.println(" Calibration_factor: ");
  // Serial.println(calibration_factor);
  // Serial.println();  

  /* Poids */
  Serial.print("Poids mesurer : ");  // debug value
  Serial.println(real_meas);
  var_poids = (short)(real_meas*100);
  Serial.print("Valeur poids  envoyer : ");
  Serial.println(var_poids);

  //   /* GPS */
  Serial.print("Location: ");
  Serial.print(latitude, 7);
  Serial.print(", ");
  Serial.println(longitude, 7);
  
  int var_latitude = latitude * 10000000.0;
  Serial.print("latitude envoyer : ");
  Serial.println(var_latitude);

  int var_longitude = longitude * 10000000.0;
  Serial.print("longitude envoyer : ");
  Serial.println(var_longitude);  

  var_liquide = (short) liquidLevel;
  Serial.print("niveau liquide envoyer : ");
  Serial.println(var_liquide);

  int err;
  modem.setPort(3);
  modem.beginPacket();
  modem.dataRate(2);
  
  // envoi du pourcentage de la batterie
  modem.write(var_battery);
  // envoi du poids
  modem.write(var_poids);
  // envoi liquide
  modem.write(var_liquide);
  // envoi latitude
  modem.write(var_latitude);
  // envoi longitude
  modem.write(var_longitude);
   

  err = modem.endPacket(true);
  if (err > 0) { //En cas d'erreur, indiquer sur moniteur serie
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
  }
}

void loop() {
  //Start of Program
  while (modem.available()) {
    Serial.write(modem.read());
  }
  modem.poll();
}
