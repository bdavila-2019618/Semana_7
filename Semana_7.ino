/*Fundacion KINAL
 * Byron Dávila
 * 2019618
 * 5TO Electronica
 * EB5CM
 */
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <OneWire.h>                
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>  

#define pin_Led 3
#define rcwl 4
#define ROJO 5
#define AZUL 6
#define VERDE 7
#define BUZZER 8
#define Puerta 9
#define pin_servo 10
#define rel1 11
#define rel2 12
#define Foco1 A0 
#define Foco2 A1 

Servo servo;  //mi servo

Adafruit_NeoPixel neopixel(7,pin_Led, NEO_GRB + NEO_KHZ800);

#define direccion_lcd 0x27
#define filas 2
#define columnas 16
LiquidCrystal_I2C LCD_DAVILA(direccion_lcd, columnas, filas);

OneWire ourWire(2);                
DallasTemperature sensors(&ourWire);

//funciones
void puerta();
void MOVIMIENTO();
void salidas();
void temperatura();
int temperaturac();
void rele1();
void rele2();

//variables
int Celsius;
int mempuerta=0; 
int memfoco1=0;
int memfoco2=0;

byte ICE[] = {
  B00000,
  B01110,
  B11101,
  B11111,
  B11111,
  B10111,
  B01110,
  B00000
};
byte Agua[] = {
  B00010,
  B00110,
  B01110,
  B10011,
  B10111,
  B11111,
  B01110,
  B00000
};
byte Fuegito[] = {
  B00010,
  B00111,
  B00000,
  B00100,
  B01110,
  B11111,
  B00000,
  B00000
};
byte Persona[] = {
  B00100,
  B01110,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000
};
byte f_oof[] = {
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B01110,
  B01110,
  B00100
};
byte f_on[] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B01110,
  B01110,
  B00100
};

void setup() {
  salidas();
  digitalWrite(rel1, HIGH);
  digitalWrite(rel2, HIGH);
  servo.attach(pin_servo);
  sensors.begin();  
  LCD_DAVILA.init(); 
  LCD_DAVILA.backlight(); 
  LCD_DAVILA.createChar(0,ICE);
  LCD_DAVILA.createChar(1,Agua);
  LCD_DAVILA.createChar(2,Fuegito);
  LCD_DAVILA.createChar(3,Persona);
  LCD_DAVILA.createChar(4,f_oof);
  LCD_DAVILA.createChar(5,f_on);
  LCD_DAVILA.setCursor(0,1);
  LCD_DAVILA.print("P:close");
  LCD_DAVILA.setCursor(9,1);
  LCD_DAVILA.print("F:");
  LCD_DAVILA.write(4);
  LCD_DAVILA.setCursor(13,1);
  LCD_DAVILA.print("F:");
  LCD_DAVILA.write(4);
}

void loop() {
  Celsius = temperaturac();
  puerta();
  rele1();
  rele2();
  puerta();
  temperatura();
  rele1();
  rele2();
  MOVIMIENTO();
  rele1();
  rele2();
  puerta();
}

void salidas(){
  pinMode(pin_Led,OUTPUT);
  pinMode(rcwl,INPUT);
  pinMode(Puerta,INPUT);
  pinMode(AZUL,OUTPUT);
  pinMode(VERDE,OUTPUT);
  pinMode(ROJO,OUTPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(rel1,OUTPUT);
  pinMode(rel2,OUTPUT);
  pinMode(Foco1,INPUT);
  pinMode(Foco2,INPUT);

}

void temperatura(){
  if ( Celsius> 14 && Celsius<22){
      LCD_DAVILA.setCursor(0,0);
      LCD_DAVILA.print("Temp:");
      LCD_DAVILA.write(0);
      digitalWrite(ROJO, HIGH);
      digitalWrite(AZUL, HIGH);
      digitalWrite(VERDE, LOW);
      }  
  if ( Celsius>21 && Celsius<26){
      LCD_DAVILA.setCursor(0,0);
      LCD_DAVILA.print("Temp:");
      LCD_DAVILA.write(1);
      digitalWrite(ROJO, LOW);
      digitalWrite(AZUL, LOW);
      digitalWrite(VERDE, HIGH);
      } 
  if ( Celsius>25 && Celsius<46){
      LCD_DAVILA.setCursor(0,0);
      LCD_DAVILA.print("Temp:");
      LCD_DAVILA.write(2);
      digitalWrite(ROJO, HIGH);
      digitalWrite(AZUL, LOW);
      digitalWrite(VERDE, LOW);
      }  
}

int temperaturac(){
  sensors.requestTemperatures();   
  float temp= sensors.getTempCByIndex(0); 
  return temp;
}

void MOVIMIENTO(){
  bool mov = digitalRead(rcwl);
  if(mov==1){
    LCD_DAVILA.setCursor(9,0);
    LCD_DAVILA.print("Radar:");
    LCD_DAVILA.write(3);
    for(int i =0; i<8; i++){
      neopixel.setPixelColor(i,neopixel.Color(255,0,0));
      neopixel.show(); 
    }
    digitalWrite(BUZZER, HIGH);
    delay(250);
    for(int i =0; i<8; i++){
      neopixel.setPixelColor(i,neopixel.Color(124,252,0));
      neopixel.show(); 
    }
    LCD_DAVILA.setCursor(9,0);
    LCD_DAVILA.print("Radar: ");
    digitalWrite(BUZZER, LOW);
    delay(250);
   
  }
  if(mov==0){
    for(int i =0; i<8; i++){
      neopixel.setPixelColor(i,neopixel.Color(0,0,255));
      neopixel.show(); 
      LCD_DAVILA.setCursor(9,0);
      LCD_DAVILA.print("Radar: ");
    }
  }
}
void puerta(){
  if(digitalRead(Puerta) && mempuerta==0){
    servo.write(0);
    LCD_DAVILA.setCursor(0,1);
    LCD_DAVILA.print("P:Abierto ");
    delay(500);
    mempuerta=1;
  }
  if(digitalRead(Puerta) && mempuerta==1){
    servo.write(90);
    LCD_DAVILA.setCursor(0,1);
    LCD_DAVILA.print("P:NO ENTRAR ");
    delay(500);
    mempuerta=0;
  }
}

void rele1(){
  if (digitalRead(Foco1) && memfoco1==0){
    digitalWrite(rel1, LOW);
    LCD_DAVILA.setCursor(9,1);
    LCD_DAVILA.print("F:");
    LCD_DAVILA.write(5);
    delay(500);
    memfoco1=1;
  }
if (digitalRead(Foco1) && memfoco1==1){
    digitalWrite(rel1, HIGH);
    LCD_DAVILA.setCursor(9,1);
    LCD_DAVILA.print("F:");
    LCD_DAVILA.write(4);
    delay(500);
    memfoco1=0;
  }
}
 
void rele2(){
  if (digitalRead(Foco2) && memfoco2==0){
    digitalWrite(rel2, LOW);
    LCD_DAVILA.setCursor(13,1);
    LCD_DAVILA.print("F:");
    LCD_DAVILA.write(5);
    delay(500);
    memfoco2=1;
  }
if (digitalRead(Foco2) && memfoco2==1){
    digitalWrite(rel2, HIGH);
    LCD_DAVILA.setCursor(13,1);
    LCD_DAVILA.print("F:");
    LCD_DAVILA.write(4);
    delay(500);
    memfoco2=0;
  }
}
