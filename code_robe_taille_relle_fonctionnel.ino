/*
 //IBRAHIMA G : Logigramme VERSION 2
 * +----------------------------------+
|             DÉMARRAGE             |
+----------------------------------+
           |
           V
+----------------------------------+
|  MESURE DE LA DISTANCE           |
|     (mesurerDistance())          |
+----------------------------------+
           |
           V
+----------------------------------+
|  SI distance < 500 ALORS         |
|     Avancer()                    |
+----------------------------------+
|      |                           |
|      | (Sinon, si distance >= 500)|
|      V                           |
+----------------------------------+
|  TANT QUE true                   |
|      trouverMonChemin()          |
|      SI chemin trouvé ALORS       |
|          Avancer()                |
+----------------------------------+
           |
           V
+----------------------------------+
|     RETOUR À LA MESURE           |
+----------------------------------+
*/


#include <Servo.h>

const byte TRIGGER_PIN1 = A2;
const byte ECHO_PIN1 = A3;
const byte TRIGGER_PIN_2 = A1;
const byte ECHO_PIN_2 = A4;

const unsigned long MEASURE_TIMEOUT = 25000UL;
const float SOUND_SPEED = 340.0 / 1000;

Servo myservo1;
Servo myservo2;
int ena = 6;
int in1 = 2;
int in2 = 3;
int in3 = 4;
int in4 = 5;
int enb = 7;
int vitesse;

void setup() {
  Serial.begin(115200);

  pinMode(TRIGGER_PIN1, OUTPUT);
  digitalWrite(TRIGGER_PIN1, LOW);
  pinMode(ECHO_PIN1, INPUT);
  
  pinMode(TRIGGER_PIN_2, OUTPUT);
  digitalWrite(TRIGGER_PIN_2, LOW);
  pinMode(ECHO_PIN_2, INPUT);
  
  
  myservo1.attach(A5);
  myservo2.attach(A6);
  pinMode(ena, OUTPUT);  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb, OUTPUT);
  vitesse = 400;
}
/*void loop() {
  float distance1 = mesurerDistance1();
  float distance2 = mesurerDistance2();
  if(distance1 !=0 || distance2 !=0){  
    if (distance1 < 500 || distance2 < 500) {
      Serial.println("OBSTACLE");
      trouverMonChemin();
    } else {
      avancer();
    }
  }
}*/
void loop() {
  // regarderdevant();
  // delay(1000);
  // regarderadroite();
  // delay(1000);
  // regarderagauche();
  // delay(1000);
  // VoieLibreAGauche();
  // delay(1000);
  // VoieLibreADroite();
  reculer();

  

/*  float distance1 = mesurerDistance1();
  float distance2 = mesurerDistance2();
  if ((distance1||distance2) > 500) {
    avancer();
  } else {
    if((distance1||distance2)!=0 &(distance1||distance2)<500) {
      Serial.println("OBSTACLE");
      trouverMonChemin();
      avancer();
      //break;
    }
    avancer();
  }
  avancer();*/
}


// Permet de mesurer la distance de l obstacle 

float mesurerDistance1() {
  digitalWrite(TRIGGER_PIN1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN1, LOW);

  long measure = pulseIn(ECHO_PIN1, HIGH, MEASURE_TIMEOUT);

  float distance_mm = measure / 2.0 * SOUND_SPEED;
  Serial.println(distance_mm);
  return distance_mm;
}
float mesurerDistance2() {
  digitalWrite(TRIGGER_PIN_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN_2, LOW);

  long measure = pulseIn(ECHO_PIN_2, HIGH, MEASURE_TIMEOUT);

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

void reculer(){
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
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);  
  analogWrite(ena, 0);
  analogWrite(enb, vitesse);
}

void tourneragauche(){
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 0);
  digitalWrite(in4, 0);
  analogWrite(ena, vitesse);
  analogWrite(enb, 0);    
}

void avancer(){
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0); 
  analogWrite(ena, vitesse);
  analogWrite(enb, vitesse); 
}

//Tourner l ultrason a gauche  
void regarderagauche() {
  myservo2.write(0); // Angle de rotation pour regarder à gauche (à ajuster)
  delay(500);
}
//Tourner l ultrason a doite  
void regarderadroite() {
  myservo1.write(180); // Angle de rotation pour regarder à droite (à ajuster)
  delay(500);
}
//Tourner l ultrason devant 
void regarderdevant() {
  myservo1.write(90); // Angle de rotation pour regarder droit devant (à ajuster)
  myservo2.write(90); // Angle de rotation pour regarder droit devant (à ajuster)
}
//Fonction permmettant de voir si la voie est libre a Gauche
int VoieLibreAGauche() {
  int distanceAGauche;
  regarderagauche(); // Regarde à gauche
  distanceAGauche = mesurerDistance1(); // Mesure la distance à gauche
  return distanceAGauche;
}
//Fonction permmettant de voir si la voie est libre a Droite
int VoieLibreADroite() {
  int distanceADroite;
  regarderadroite(); // Regarde à droite
  distanceADroite = mesurerDistance2(); // Mesure la distance à droite
  return distanceADroite;
}

//Fonction permettant de trouver le chemin une fois un obstacle detecter 
/*void trouverMonChemin() {
  arret(); // Arrêt
 // delay(2000);
  int distanceAGauche = VoieLibreAGauche(); // Mesure de la disponibilité de la voie à gauche
  Serial.print("DISTANCE A GAUCHE ");
  Serial.println(distanceAGauche);
  int distanceADroite = VoieLibreADroite(); // Mesure de la disponibilité de la voie à droite
  Serial.print("DISTANCE A DROITE ");
  Serial.println(distanceADroite);
  delay(2000);
  regarderdevant();
  if (distanceAGauche < distanceADroite) {    
    tourneragauche(); // Tourner à gauche
    delay(500);
  } else {
    tourneradroite(); // Tourner à droit
    delay(500);
  }
  avancer();
}*/
void trouverMonChemin() {
  arret();
  int distanceAGauche = VoieLibreAGauche();
  Serial.print("DISTANCE A GAUCHE ");
  Serial.println(distanceAGauche);
  int distanceADroite = VoieLibreADroite();
  Serial.print("DISTANCE A DROITE ");
  Serial.println(distanceADroite);

  delay(2000);
  regarderdevant();
  
  if (distanceAGauche < distanceADroite) {
    tourneragauche();
    delay(1000);
    avancer();
  } 
  else {
    tourneradroite();
    delay(1000);
    avancer();
  }

  // Continuer à vérifier et avancer si la voie est libre
  while (true) {
    float distance1 = mesurerDistance1();
    float distance2 = mesurerDistance2();

    if (distance1 > 500 && distance2 > 500) {
      avancer(); // Si la voie est libre, avance
      break; // Sort de la boucle
    }
  }
}
