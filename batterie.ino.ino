#include <MKRWAN.h>

LoRaModem modem;

//Liaison avec The Things Network
String appEui = "0000000000000000";
String appKey = "EBC127F8D66B753D4E6112CF5F26FE1D";
String devAddr;
String nwkSKey;
String appSKey;

float pourcentage = 15.38;        //la charge de la batterie en pourcentage

// variables pour stocker les valeurs mesurées en 16bits
short var_battery;

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
}

void loop() {
  //Start of Program
  while (modem.available()) {
    Serial.write(modem.read());
  }
  modem.poll();
}
