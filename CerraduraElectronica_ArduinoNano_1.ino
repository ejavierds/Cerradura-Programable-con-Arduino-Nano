//CERRADURA ELECTRÓNICA CON ARDUINO NANO
//COMPILADO FEBRERO 2024. BARQUISIMETO, ESTADO LARA, VENEZUELA


// Incluye las librerías necesarias
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// Define el número de filas y columnas del teclado
const byte ROWS = 4;
const byte COLS = 3;

// Define los símbolos del teclado en una matriz
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

// Pines del Arduino Nano que se conectan con las filas y columnas del teclado
byte rowPins[ROWS] = { 2, 3, 4, 5 };
byte colPins[COLS] = { 6, 7, 8 };

// Crea un objeto de la clase Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Crea un objeto de la clase LiquidCrystal_I2C con la dirección 0x27, además de 16 columnas y 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Crea un objeto de la clase Servo
Servo servo;

// Define el pin del Arduino que se conecta con el servo
const int servoPin = 9;

// Define una variable para almacenar la entrada del usuario
String entrada = "";

// Define una variable para indicar si el sistema está bloqueado o desbloqueado
bool bloqueo = true;

// Define una variable para indicar si el usuario quiere cambiar la contraseña
bool cambio = false;

// Define la contraseña inicial
String clave = "1234";

//const int pinBuzzer = 10; // Número del pin de Arduino al que conectamos el buzzer activo


// Método Principal
void setup() 
{
  // Inicializa la LCD
  lcd.init();
  lcd.backlight();
  
  // Inicializa el servomotor y se coloca en la posición inicial
  servo.attach(servoPin);
  servo.write(0);

  //pinMode(pinBuzzer, OUTPUT);

  //Se invoca al mensaje de inicio
  mensaje_inicio();
}

void loop() {
  
  /*digitalWrite(pinBuzzer, HIGH); // Enciende el buzzer (5V)
  delay(1000); // Espera 1 segundo
  digitalWrite(pinBuzzer, LOW); // Apaga el buzzer (0V)
  delay(4000); // Espera 4 segundos*/
  
  // Leer la tecla pulsada por el usuario
  char tecla = keypad.getKey();
  // Si se ha pulsado alguna tecla
  if (tecla) 
  {
    // Si se ha pulsado el símbolo #
    if (tecla == '#') 
    {
      // Si el sistema está desbloqueado
      if (!bloqueo) 
      {
        // Activa el modo de cambio de contraseña
        cambio = true;
        // Borra la entrada anterior
        entrada = "";
        // Muestra un mensaje en la LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cambio de clave");
        lcd.setCursor(0, 1);
        lcd.print("Nueva clave:");
      }
      // Si el sistema está bloqueado
      else 
      {
        // Borra la entrada anterior
        entrada = "";
        // Muestra un mensaje en el LCD
        mensaje_incorrecta();
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cambio de Clave");
        lcd.setCursor(0, 1);
        lcd.print("Nueva Clave:" + clave);
      }
    }
    // Si se ha pulsado el símbolo *
    else if (tecla == '*') 
    {
      // Si el sistema está desbloqueado y se quiere cambiar la contraseña
      if (!bloqueo && cambio) {
        // Si la entrada tiene 4 dígitos
        if (entrada.length() == 4) 
        {
          // Cambia la contraseña por una nueva
          clave = entrada;
          // Desactiva el modo de cambio de contraseña
          cambio = false; //OJO
          // Mostrar un mensaje en el LCD
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Clave Cambiada");
          lcd.setCursor(0, 1);
          lcd.print("Nueva Clave:" + clave);
        }
        // Si la entrada no tiene 4 dígitos
        else 
        {
          // Borra la entrada anterior
          entrada = "";
          // Muestra un mensaje en el LCD
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Clave Incorrecta");
          lcd.setCursor(0, 1);
          lcd.print("Nueva Clave:" + clave);
        }
      }
      // Si el sistema está bloqueado o no se quiere cambiar la contraseña
      else 
      {
        // Borra la entrada anterior
        entrada = "";
        // Muentra un mensaje en la LCD
        ingreso_clave();
      }
    }
    // Si se ha pulsado un número
    else 
    {
      // Añade el número a la entrada
      entrada += tecla;
      // Mostrar el número en el LCD
      lcd.print(tecla);
      // Si la entrada tiene 4 dígitos
      if (entrada.length() == 4) {
        // Si la entrada coincide con la contraseña
        if (entrada == clave) {
          // Si el sistema está bloqueado
          if (bloqueo) {
            // Desbloquea el sistema
            bloqueo = false;
            // Mueve el servo a la posición de desbloqueo
            servo.write(90);
            // Muestra un mensaje en el LCD
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Clave correcta");
            lcd.setCursor(0, 1);
            lcd.print("Sistema abierto");
            //delay(2000);
            lcd.clear();
            //menu_dosteclas();
            lcd.setCursor(0, 0);
            lcd.print("*=>Bloquear");
            lcd.setCursor(0, 1);
            lcd.print("#=>Cambiar Clave");
          }
          // Si el sistema está desbloqueado
          else {
            // Bloquea el sistema
            bloqueo = true;
            // Mueve el servo a la posición de bloqueo
            servo.write(0);
            // Muestra un mensaje en la LCD
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Clave correcta");
            lcd.setCursor(0, 1);
            lcd.print("Sistema cerrado");
            //delay(2000);
            lcd.clear();
            //menu_dosteclas();
            lcd.setCursor(0, 0);
            lcd.print("*=>Desbloquear");
            lcd.setCursor(0, 1);
            lcd.print("#=>Cambiar Clave");
            //ingreso_clave();
          }
        }
        // Si la entrada no coincide con la contraseña
        else {
          // Borra la entrada anterior
          entrada = "";
          // Mostrar un mensaje en el LCD
          mensaje_incorrecta();
        }
      }
    }
  }

}

// Método para el mensaje de bienvenida en la LCD con nombre del proyecto
void mensaje_inicio() {
  lcd.setCursor(0, 0);
  lcd.print(" USis Lock v1.1");
  lcd.setCursor(0, 1);
  lcd.print("  Barquisimeto  ");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Bienvenido(a)! ");
  lcd.setCursor(0, 1);
  lcd.print("================");
  delay(3000);
  ingreso_clave();
}

// Para el ingreso de la contraseña
void ingreso_clave() 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" USis Lock v1.1");
  lcd.setCursor(0, 1);
  lcd.print("Ingrese su clave");
}

// Método de un mensaje de clave errada que se repite
void mensaje_incorrecta() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Clave Incorrecta");
  lcd.setCursor(0, 1);
  lcd.print("Intente de nuevo");
  delay(1000);
  ingreso_clave();
}

/*// Método para orientar en un menú de uso de los símbolos * y #
void menu_dosteclas() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*=>Usar Clave");
  lcd.setCursor(0, 1);
  lcd.print("#=>Cambiar Clave");
  //delay(1000);
  //char tecla = keypad.getKey();
}*/