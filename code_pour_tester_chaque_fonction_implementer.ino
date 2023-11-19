#include <Servo.h>

// Constantes pour les broches, les timeouts et la vitesse du son
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
  vitesse = 225;
}

void loop() {
  testMesurerDistance();
  testArret();
  testAvancer();
  testTournerADroite();
  testTournerAGauche();
  testReculer();
  testRegarderAGauche();
  testRegarderADroite();
  testVoieLibreAGauche();
  testVoieLibreADroite();
  testTrouverMonChemin();
  delay(5000); // Attente entre les tests
}

void testMesurerDistance() {
  float distance = mesurerDistance();
  Serial.print("Distance mesuree: ");
  Serial.println(distance);
  delay(1000);
}

void testArret() {
  arret();
  Serial.println("Robot arrete");
  delay(1000);
}

void testAvancer() {
  Serial.println("Avancer");
  avancer();
  delay(1000);
}

void testTournerADroite() {
  Serial.println("Tourner a droite");
  tourneradroite();
  delay(1000);
}

void testTournerAGauche() {
  Serial.println("Tourner a gauche");
  tourneragauche();
  delay(1000);
}

void testReculer() {
  Serial.println("Reculer");
  reculer();
  delay(1000);
}

void testRegarderAGauche() {
  Serial.println("Regarder a gauche");
  regarderagauche();
  delay(1000);
}

void testRegarderADroite() {
  Serial.println("Regarder a droite");
  regarderadroite();
  delay(1000);
}

void testVoieLibreAGauche() {
  Serial.print("Voie libre a gauche: ");
  int libreAGauche = VoieLibreAGauche();
  Serial.println(libreAGauche);
  delay(1000);
}

void testVoieLibreADroite() {
  Serial.print("Voie libre a droite: ");
  int libreADroite = VoieLibreADroite();
  Serial.println(libreADroite);
  delay(1000);
}

void testTrouverMonChemin() {
  Serial.println("Trouver le chemin");
  trouverMonChemin();
  delay(1000);
}

// Reste des fonctions utilisées dans le programme
