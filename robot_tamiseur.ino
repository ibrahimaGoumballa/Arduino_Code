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

boolean S = false; //LOW tout le temps, sauf quand V passe la limite//
boolean dansZone = true;
boolean dansZoneOld = true;

float X, Y;  //latitude actuelle, longitude actuelle

const float Xmax = 48.8450559; //latitude max
const float Xmin = 48.8450290; //latitude min
const float Ymax = 2.3564313; //longitude max
const float Ymin = 2.3565972; //longitude min

float pourcentage = 15.38;        //la charge de la batterie en pourcentage

// variables pour stocker les valeurs mesurées en 16bits
short var_battery;

// pOIDS
HX711 scale;
float calibration_factor = 21067; // for 16,6 -> 21067 and for ~93,5 -> 21387
float meas, p, real_meas;
/*
La technique de calibration consiste à :

--envoyer la commande set_scale()   (sans aucun paramètre)
--envoyer la commande tare()   (sans aucun paramètre)
--placer un poids connu sur la balance
--faire une lecture avec get-units(10)  ( 10 mesures consécutives ) et afficher le résultat.
--diviser la valeur obtenue par le poids référence en utilisant la bonne unité ( kg, g, lbs... )
--le résultat x de cette division est la valeur à envoyer par set_scale(x) avant de faire les mesures,
--mais une seule fois pour l'ensemble des mesures, jusqu'à une éventuelle nouvelle calibration.
*/
// Capteur de niveau
int liquidLevel = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);//Delay to let system boot
  Serial.println("Pourcentage Batterie\n\n");
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

  delay(2000);

  /* Batterie */
  Serial.print("Pourcentage batterie : ");  // debug value
  Serial.println(pourcentage);
  //var_battery = (short)(pourcentage*100);
  var_battery = (short)(pourcentage);
  Serial.print("Valeur batterie  : ");
  Serial.println(var_battery);

  int err;
  modem.setPort(3);
  modem.beginPacket();
  modem.dataRate(2);
  
  // envoi du pourcentage de la batterie
  modem.write(var_battery);

  err = modem.endPacket(true);
  if (err > 0) { //En cas d'erreur, indiquer sur moniteur serie
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
  }

  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);  
  // Pin capteur de niveau
  pinMode(A0, INPUT);

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

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.print("Reading: ");
  //Serial.print(scale.get_units(), 1);
  meas = scale.get_units(10);
  Serial.print(meas);
  p = meas/10;
  Serial.print(p);
  Serial.print(" kg "); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  real_meas = p*1000;
  Serial.println(" real_ms: ");
  Serial.println(real_meas);
  Serial.print(" g "); //Change this to g
  Serial.println(" Calibration_factor: ");
  Serial.println(calibration_factor);
  Serial.println();  
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 10;
  }
  // Capteur de niveau
  liquidLevel = digitalRead(A0);
  Serial.print("liquidLevel= "); Serial.println(liquidLevel, DEC);
  //Start of Program
  while (modem.available()) {
    Serial.write(modem.read());
  }
  modem.poll();

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
    Serial.println("--> NON");
    }else {
    Serial.println("--> OUI");
    }
    delay(300);
  }  
}
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