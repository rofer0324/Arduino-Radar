#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

// Pines de los servos
const int servoPin1 = 9; // Servo derecho
const int servoPin2 = 10; // Servo izquierdo

// Pines de los sensores ultrasónicos
const int trigPin1 = 34; // Trig para sensor derecho
const int echoPin1 = 36; // Echo para sensor derecho
const int trigPin2 = 46; // Trig para sensor izquierdo
const int echoPin2 = 44; // Echo para sensor izquierdo

// Pines para el LED RGB
const int redPin = 2;
const int greenPin = 3;
const int bluePin = 4;

// Pin del buzzer
const int buzzerPin = 40;

// Configuración de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Variables para los servos
Servo servo1; 
Servo servo2; 
int angle = 0; // Ángulo inicial

void setup() {
  Serial.begin(9600);

  // Configuración de los servos
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);

  // Configuración de los pines de los sensores ultrasónicos
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Configuración de los pines del LED RGB y el buzzer
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Inicialización de la pantalla OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("No se encuentra la pantalla OLED"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // Barrido hacia adelante (0 a 180 grados)
  for (angle = 0; angle <= 180; angle += 10) {
    servo1.write(angle); // Mover servo derecho
    servo2.write(180 - angle); // Mover servo izquierdo en dirección opuesta
    delay(80);

    int distance1 = getDistance(trigPin1, echoPin1); // Distancia sensor derecho
    int distance2 = getDistance(trigPin2, echoPin2); // Distancia sensor izquierdo

    // Enviar datos a Processing en el formato esperado
    Serial.print(angle);
    Serial.print(",");
    Serial.print(distance1);
    Serial.print(",");
    Serial.print(180 - angle);
    Serial.print(",");
    Serial.println(distance2);

    // Mostrar en pantalla OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Angulo Der: ");
    display.println(angle);
    display.print("Dist Der: ");
    display.print(distance1);
    display.println(" cm");
    display.print("Angulo Izq: ");
    display.println(180 - angle);
    display.print("Dist Izq: ");
    display.print(distance2);
    display.println(" cm");
    display.display();

    // Control del LED y el buzzer
    if (distance1 < 10 || distance2 < 10) {
      setLEDColor(255, 0, 0); // Rojo para muy cerca
      tone(buzzerPin, 1000); // Buzzer emite un tono alto
    } else if (distance1 < 20 || distance2 < 20) {
      setLEDColor(0, 0, 255); // Azul para distancia media
      tone(buzzerPin, 500); // Buzzer emite un tono medio
    } else {
      setLEDColor(0, 255, 0); // Verde para lejos
      noTone(buzzerPin); // Apaga el buzzer
    }
  }

  // Barrido hacia atrás (180 a 0 grados)
  for (angle = 180; angle >= 0; angle -= 10) {
    servo1.write(angle); // Mover servo derecho
    servo2.write(180 - angle); // Mover servo izquierdo en dirección opuesta
    delay(80);

    int distance1 = getDistance(trigPin1, echoPin1);
    int distance2 = getDistance(trigPin2, echoPin2);

    // Enviar datos a Processing
    Serial.print(angle);
    Serial.print(",");
    Serial.print(distance1);
    Serial.print(",");
    Serial.print(180 - angle);
    Serial.print(",");
    Serial.println(distance2);

    // Mostrar en pantalla OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Angulo Der: ");
    display.println(angle);
    display.print("Dist Der: ");
    display.print(distance1);
    display.println(" cm");
    display.print("Angulo Izq: ");
    display.println(180 - angle);
    display.print("Dist Izq: ");
    display.print(distance2);
    display.println(" cm");
    display.display();

    // Control del LED y el buzzer
    if (distance1 < 10 || distance2 < 10) {
      setLEDColor(255, 0, 0);
      tone(buzzerPin, 1000);
    } else if (distance1 < 20 || distance2 < 20) {
      setLEDColor(0, 0, 255);
      tone(buzzerPin, 500);
    } else {
      setLEDColor(0, 255, 0);
      noTone(buzzerPin);
    }
  }
}

// Función para obtener la distancia medida por el sensor ultrasónico
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

// Función para establecer el color del LED RGB
void setLEDColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
