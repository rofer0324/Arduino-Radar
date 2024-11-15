import processing.serial.*; // Importar la librería Serial para comunicación con el Arduino

// Variables para manejar la comunicación serial
Serial myPort;
String[] inputData;
int angle1 = 0, distance1, angle2 = 180, distance2; // Ángulos iniciales para ambos sensores
int maxDistance = 200; // Máxima distancia que puede alcanzar el radar (radio del círculo)

void setup() {
  // Configuración de la ventana
  size(600, 600);
  background(0); // Fondo negro
  smooth();
  
  // Inicializar la comunicación serial
  String portName = Serial.list()[0]; // Asume que el puerto COM correcto es el primero en la lista
  myPort = new Serial(this, portName, 9600); // Configura el puerto y la velocidad
  
  // Espera por datos
  myPort.bufferUntil('\n'); // Leer hasta el salto de línea
}

void draw() {
  background(0); // Fondo negro
  
  // Dibujar el radar (círculo verde)
  stroke(0, 255, 0); // Color del contorno verde
  noFill();
  ellipse(width / 2, height / 2, 400, 400); // Círculo central
  
  // Dibujar las líneas de los cuadrantes (ejes)
  stroke(0, 255, 0);
  line(width / 2, height / 2, width, height / 2); // Línea horizontal
  line(width / 2, height / 2, width / 2, 0); // Línea vertical
  
  // Dibujar las líneas para separar los cuadrantes
  stroke(255, 255, 255, 50); // Líneas blancas semitransparentes para separar cuadrantes
  line(width / 2, height / 2, width / 2 + maxDistance, height / 2); // Línea horizontal del cuadrante 1 y 4
  line(width / 2, height / 2, width / 2, height / 2 + maxDistance); // Línea vertical del cuadrante 2 y 3
  
  // Convertir las distancias a posiciones en el radar
  float radian1 = radians(angle1); 
  float radian2 = radians(angle2);
  
  // Calcular las coordenadas para los puntos según los cuadrantes
  float x1 = width / 2 + cos(radian1) * min(distance1, maxDistance);
  float y1 = height / 2 + sin(radian1) * min(distance1, maxDistance);
  
  float x2 = width / 2 + cos(radian2) * min(distance2, maxDistance);
  float y2 = height / 2 + sin(radian2) * min(distance2, maxDistance);
  
  // Dibujar los círculos representando las distancias
  noStroke();
  fill(0, 255, 0, 50); // Círculo de distancia del primer sensor (transparente, verde claro)
  ellipse(width / 2, height / 2, maxDistance * 2, maxDistance * 2); // Círculo para el sensor derecho
  
  fill(0, 0, 255, 50); // Círculo de distancia del segundo sensor (transparente, azul claro)
  ellipse(width / 2, height / 2, maxDistance * 2, maxDistance * 2); // Círculo para el sensor izquierdo
  
  // Dibujar los puntos de los objetos detectados
  fill(255, 0, 0); // Color de los puntos rojo (sensor 1)
  noStroke();
  ellipse(x1, y1, 10, 10); // Punto sensor derecho
  
  fill(0, 0, 255); // Color de los puntos azul (sensor 2)
  noStroke();
  ellipse(x2, y2, 10, 10); // Punto sensor izquierdo
  
  // Dibujar los brazos (líneas) para cada sensor
  stroke(255, 0, 0); // Color rojo para el primer brazo (sensor 1)
  line(width / 2, height / 2, x1, y1); // Brazo del primer sensor
  
  stroke(0, 0, 255); // Color azul para el segundo brazo (sensor 2)
  line(width / 2, height / 2, x2, y2); // Brazo del segundo sensor
  
  // Dibujar las etiquetas
  fill(255, 0, 0); // Texto rojo para el sensor 1
  textSize(16);
  text("Sensor 1", x1 + 10, y1); // Etiqueta Sensor 1

  fill(0, 0, 255); // Texto azul para el sensor 2
  textSize(16);
  text("Sensor 2", x2 + 10, y2); // Etiqueta Sensor 2
  
  // Actualizar los ángulos para simular el recorrido completo
  angle1 = (angle1 + 1) % 360; // Incrementar el ángulo del primer sensor
  angle2 = (angle2 + 1) % 360; // Incrementar el ángulo del segundo sensor
}

void serialEvent(Serial myPort) {
  // Leer los datos del puerto serial
  String inputString = myPort.readStringUntil('\n');
  if (inputString != null) {
    inputString = trim(inputString); // Eliminar espacios
    inputData = split(inputString, ','); // Separar los valores por coma
    
    if (inputData.length == 4) {
      distance1 = int(inputData[1]); // Distancia del primer sensor
      distance2 = int(inputData[3]); // Distancia del segundo sensor
    }
  }
}
