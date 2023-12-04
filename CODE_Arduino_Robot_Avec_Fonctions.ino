//IBRAHIMA G : Logigramme VERSION 2

#include <Servo.h>

const byte TRIGGER_PIN = A2;
const byte ECHO_PIN = A3;
const unsigned long MEASURE_TIMEOUT = 25000UL;
const float SOUND_SPEED = 340.0 / 1000;

Servo myservo;
int ena = 6;
int in1 = 2;
int in2 = 3;
int in3 = 4;
int in4 = 5;
int enb = 7;
int vitesse;

void setup() {
  Serial.begin(115200);

  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  
  myservo.attach(A6);

  pinMode(ena, OUTPUT);  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb, OUTPUT);
  vitesse = 400;
}

void loop() {
  float distance = mesurerDistance();
  if (distance > 500) {
    avancer();
  } else {
    while (true) {
      Serial.println("OBSTACLE");
      reculer();
      delay(500);
      trouverMonChemin();
      delay(500);
      avancer();
      break;
    }
  }
}


// Permet de mesurer la distance de l obstacle 

float mesurerDistance() {
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);

  float distance_mm = measure / 2.0 * SOUND_SPEED;
  Serial.println(distance_mm);
  return distance_mm;
}

void arret(){
  digitalWrite(in1, 0);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 0);  
  analogWrite(ena, 0);
  analogWrite(enb, 0);  
}

void avancer(){
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1);  
  analogWrite(ena, vitesse);
  analogWrite(enb, vitesse);
}

void tourneradroite(){
  digitalWrite(in1, 0);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1);  
  analogWrite(ena, 0);
  analogWrite(enb, vitesse);
}

void tourneragauche(){
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 0);
  analogWrite(ena, vitesse);
  analogWrite(enb, 0);    
}

void reculer(){
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0); 
  analogWrite(ena, vitesse);
  analogWrite(enb, vitesse); 
}

//Tourner l ultrason a gauche  
void regarderagauche() {
  myservo.write(0); // Angle de rotation pour regarder à gauche (à ajuster)
  delay(1000);
}
//Tourner l ultrason a doite  
void regarderadroite() {
  myservo.write(180); // Angle de rotation pour regarder à droite (à ajuster)
  delay(1000);
}
//Tourner l ultrason devant 
void regarderdevant() {
  myservo.write(90); // Angle de rotation pour regarder droit devant (à ajuster)
  delay(500);
}
//Fonction permmettant de voir si la voie est libre a Gauche
int VoieLibreAGauche() {
  int distanceAGauche;
  regarderagauche(); // Regarde à gauche
  distanceAGauche = mesurerDistance(); // Mesure la distance à gauche
  return distanceAGauche;
}
//Fonction permmettant de voir si la voie est libre a Droite
int VoieLibreADroite() {
  int distanceADroite;
  regarderadroite(); // Regarde à droite
  distanceADroite = mesurerDistance(); // Mesure la distance à droite
  return distanceADroite;
}

//Fonction permettant de trouver le chemin une fois un obstacle detecter 
void trouverMonChemin() {
  arret(); // Arrêt
  delay(1000);
  int distanceAGauche = VoieLibreAGauche(); // Mesure de la disponibilité de la voie à gauche
  Serial.print("DISTANCE A GAUCHE ");
  Serial.println(distanceAGauche);
  int distanceADroite = VoieLibreADroite(); // Mesure de la disponibilité de la voie à droite
  Serial.print("DISTANCE A DROITE ");
  Serial.println(distanceADroite);

  if (distanceAGauche < distanceADroite) {
    tourneragauche(); // Tourner à gauche
    delay(650);
    regarderdevant();
  } else {
    tourneradroite(); // Tourner à droite
    delay(650);
    regarderdevant();
  }
}
