#include <Servo.h>// library of servo controlling

// anti_crash poc, using the distance measrment by Ultrasonic Sensor HC-SR04 contorling the engine and height servo.
Servo servo_height;  // create servo object to control a servo
Servo servo_engine;
#define RELAY_ON 0
#define RELAY_OFF 1
#define Relay_1  10  // Arduino Digital I/O pin number
const int trigPin = 9;
const int echoPin = 10;
const int flag = 1;// with this flag it will possible to connect or disconnect the system

long duration;
int distanceCm1, distanceCm2;

int distance_check()// measuring the distance in front of the model
{
  int distanceCm;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;
  return distanceCm;
}


void setup() {
  Serial.begin(9600);
  digitalWrite(Relay_1, RELAY_OFF);
  servo_height.attach(11);  // attaches the servo that controls hiehgt on pin 11 to the servo object
  servo_engine.attach(12);  // attaches the servo that controls engine on pin 11 to the servo object
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(Relay_1, OUTPUT);
  pinMode(flag, INPUT);
}

void loop() {

  int flag_check = digitalRead(flag);

  while (flag == 1)
  {
    int distanceCm1 = distance_check();
    delayMicroseconds(20);
    int distanceCm2 = distance_check();
    
    while (distanceCm1 > distanceCm2 && distanceCm2 < 3000 && flag_check == 1)
    {
      Serial.print(distanceCm1);
      Serial.print('\n');
      digitalWrite(Relay_1, RELAY_ON);// set the Relay ON
      Serial.println("relay on");
      servo_height.write(170);
      delay(500);
      servo_engine.write(170);
      delay(500);

      distanceCm1 = distance_check();
      delayMicroseconds(20);
      distanceCm2 = distance_check();
      flag_check = digitalRead(flag);
    }

    digitalWrite(Relay_1, RELAY_OFF);// set the Relay OFF
    Serial.println("relay off");

    if (flag_check !=   1)
    {
      Serial.println("system out");
    }
  }
}
