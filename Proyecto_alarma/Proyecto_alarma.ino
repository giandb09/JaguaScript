#include <LiquidCrystal_I2C.h>  // Librería para controlar la pantalla LCD
#include <Keypad.h>             // Librería para controlar el teclado matricial

#define LCD_ADDR 0x27           // Dirección I2C de la pantalla LCD
#define LCD_COLUMNS 16          // Número de columnas de la pantalla LCD
#define LCD_ROWS 2              // Número de filas de la pantalla LCD

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLUMNS, LCD_ROWS);  // Inicializa el objeto para controlar la pantalla LCD

const int pinRojo = 13;         // Definición del pin para el LED rojo
const int pinVerde = 12;        // Definición del pin para el LED verde
const int pinBuzzer = 10;       // Definición del pin para el buzzer
int pinMov = 11;                // Definición del pin para el sensor de movimiento
///
bool movimientoDetectado = false; // Variable para rastrear si se ha detectado movimiento
const String codigoCorrecto = "1234"; // El código correcto para detener la alarma
String codigoIngresado = ""; // Variable para almacenar el código ingresado

const byte FILAS = 4; // Cuatro filas en el teclado
const byte COLUMNAS = 4; // Cuatro columnas en el teclado

// Definición de las teclas en el teclado
char keys[FILAS][COLUMNAS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pinesFilas[FILAS] = {2, 3, 4, 5}; // Conexiones a los pines de las filas del teclado
byte pinesColumnas[COLUMNAS] = {6, 7, 8, 9}; // Conexiones a los pines de las columnas del teclado

Keypad keypad = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS); // Inicializa el objeto para controlar el teclado

unsigned long previousMillis = 0; // Almacena el último tiempo en que se realizó una acción
const long interval = 500; // Intervalo de 500 ms para parpadear el LED y buzzer

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial a 9600 bps
  lcd.begin(LCD_COLUMNS, LCD_ROWS); // Inicializa la pantalla LCD con 16 columnas y 2 filas
  lcd.init(); // Inicializa la comunicación con la pantalla LCD
  lcd.backlight(); // Enciende la luz de fondo de la pantalla LCD

  pinMode(pinRojo, OUTPUT); // Configura el pin del LED rojo como salida
  pinMode(pinVerde, OUTPUT); // Configura el pin del LED verde como salida
  pinMode(pinBuzzer, OUTPUT); // Configura el pin del buzzer como salida
  pinMode(pinMov, INPUT); // Configura el pin del sensor de movimiento como entrada

  digitalWrite(pinVerde, HIGH); // Enciende por defecto el LED verde

  // Muestra el mensaje inicial en la pantalla LCD
  lcd.setCursor(0, 0); // Establece el cursor en la primera fila y primera columna
  lcd.print("Hola!"); // Imprime el mensaje "Hola!"
  Serial.println("Sistema inicializado. Esperando movimiento...");
}

void loop() {
  unsigned long currentMillis = millis(); // Almacena el tiempo actual

  // Verifica si se detecta movimiento y no se ha detectado antes
  if (digitalRead(pinMov) == HIGH && !movimientoDetectado) {
    iniciarAlarma(); // Llama a la función para iniciar la alarma
  }

  // Si se ha detectado movimiento
  if (movimientoDetectado) {
    gestionarAlarma(); // Llama a la función para gestionar la alarma
    verificarTeclado(); // Llama a la función para verificar el teclado
    verificarSerial(); // Llama a la función para verificar el monitor serial
  }
}

void iniciarAlarma() {
  digitalWrite(pinVerde, LOW); // Apaga el LED verde
  movimientoDetectado = true; // Establece la bandera de movimiento detectado como verdadera

  // Muestra el mensaje en la pantalla LCD
  lcd.clear(); // Borra el contenido actual de la pantalla LCD
  lcd.setCursor(0, 0); // Establece el cursor en la primera fila y primera columna
  lcd.print("Introduzca la"); // Imprime el primer mensaje
  lcd.setCursor(0, 1); // Establece el cursor en la segunda fila y primera columna
  lcd.print("clave para parar"); // Imprime el segundo mensaje

  Serial.println("Movimiento detectado. Introduzca la clave para parar:");
}

void gestionarAlarma() {
  unsigned long currentMillis = millis(); // Almacena el tiempo actual

  // Si el intervalo de tiempo ha pasado, alterna el estado del LED y buzzer
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Actualiza el último tiempo

    if (digitalRead(pinRojo) == HIGH) { // Si el LED rojo está encendido
      digitalWrite(pinRojo, LOW); // Apaga el LED rojo
      noTone(pinBuzzer); // Apaga el buzzer
    } else {
      digitalWrite(pinRojo, HIGH); // Enciende el LED rojo
      tone(pinBuzzer, 3000); // Enciende el buzzer a 1000 Hz
    }
  }
}

void verificarTeclado() {
  char tecla = keypad.getKey(); // Lee la tecla presionada
  if (tecla) { // Si se presiona una tecla
    Serial.print("Tecla presionada: ");
    Serial.println(tecla);

    if (tecla == '#') { // Si se presiona '#', verifica el código
      validarCodigo(); 
    } else if (tecla == '*') { // Si se presiona '*', borra el código ingresado
      borrarCodigo(); 
    } else {
      codigoIngresado += tecla; // Agrega la tecla presionada al código ingresado
    }
  }
}

void verificarSerial() {
  if (Serial.available() > 0) { // Si hay datos disponibles en el monitor serial
    char input = Serial.read(); // Lee el carácter ingresado
    if (input == '\n' || input == '\r') { // Si se presiona Enter
      validarCodigo(); // Verifica el código ingresado
    } else {
      codigoIngresado += input; // Agrega el carácter ingresado al código
    }
  }
}

void validarCodigo() {
  Serial.print("Código ingresado: ");
  Serial.println(codigoIngresado);

  if (codigoIngresado == codigoCorrecto) { // Si el código es correcto
    detenerAlarma();
  } else {
    claveIncorrecta();
  }
}

void borrarCodigo() {
  codigoIngresado = ""; // Borra el código ingresado
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Introduzca la");
  lcd.setCursor(0, 1);
  lcd.print("clave para parar");
  Serial.println("Código borrado.");
}

void detenerAlarma() {
  movimientoDetectado = false; // Indica que ya no se ha detectado movimiento
  digitalWrite(pinVerde, HIGH); // Enciende el LED verde
  digitalWrite(pinRojo, LOW); // Apaga el LED rojo
  noTone(pinBuzzer); // Apaga el buzzer
  
  lcd.clear(); // Borra la pantalla LCD
  lcd.setCursor(0, 0); // Coloca el cursor en la primera fila, primera columna
  lcd.print("Clave correcta!!"); // Muestra un mensaje de clave correcta
  Serial.println("Clave correcta! Alarma desactivada."); // Muestra un mensaje en el monitor serial
  delay(2000); // Espera 2 segundos

  lcd.clear(); // Borra la pantalla LCD
  lcd.setCursor(0, 0); // Coloca el cursor en la primera fila, primera columna
  lcd.print("Hola!"); // Muestra el mensaje inicial en la pantalla LCD

  codigoIngresado = ""; // Restablece el código ingresado
}

void claveIncorrecta() {
  lcd.clear(); // Borra la pantalla LCD
  lcd.setCursor(0, 0); // Coloca el cursor en la primera fila, primera columna
  lcd.print("Clave incorrecta"); // Muestra un mensaje de clave incorrecta
  Serial.println("Clave incorrecta. Intentelo de nuevo."); // Muestra un mensaje en el monitor serial

  codigoIngresado = ""; // Restablece el código ingresado
  delay(2000); // Espera 2 segundos

  lcd.clear(); // Borra la pantalla LCD
  lcd.setCursor(0, 0); // Coloca el cursor en la primera fila, primera columna
  lcd.print("Introduzca la"); // Muestra el mensaje
  lcd.setCursor(0, 1); // Coloca el cursor en la segunda fila, primera columna
  lcd.print("clave para parar"); // Muestra el mensaje
}

