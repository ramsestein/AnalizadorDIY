
#include <LiquidCrystal.h>

const int sensor_emisor = 2;
const int led_verde = 9;
const int led_rojo = 11;
const int RS = 3;
const int E = 4;
const int val1 = 5;
const int val2 = 6;
const int val3 = 7;
const int val4 = 8;
const int sensor_receptor = A1;

LiquidCrystal lcd (RS,E,val1,val2,val3,val4);

int valor1 = 0;
int valor2 = 0;
int cont = 0;
double cloro = 0;
double res1 = 0;
double res2 = 0;
double res3 = 0;
double margen = 0;

void setup (){
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(sensor_emisor, OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_rojo,OUTPUT);
}

void loop (){
 digitalWrite(led_rojo,HIGH);
 digitalWrite(led_verde,LOW);
 lcd.clear();
 lcd.print("Put in");
 //Toma de datos
 digitalWrite(sensor_emisor,HIGH);
 valor1 = analogRead(sensor_receptor);
 Serial.println("Voltaje on");
 Serial.println(valor1);
 delay(500);
 digitalWrite(sensor_emisor,LOW);
 valor2 = analogRead(sensor_receptor);
 Serial.println("Voltaje off");
 Serial.println(valor2);
 delay(500);
 //Reconocer el agua
 if (valor1 < 1020){
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Calculando...");
   for (cont = 0; cont <8; cont++){
     digitalWrite(sensor_emisor,HIGH);
     valor1 = analogRead(sensor_receptor);
     Serial.println("Voltaje on, en bucle");
     delay(500);
     digitalWrite(sensor_emisor,LOW);
     valor2 = analogRead(sensor_receptor);
     Serial.println("Voltaje off, en bucle");
     delay(500);
   }
     while(valor1<1020){
       Serial.println("Entro en while");
       digitalWrite(led_rojo,LOW);
       digitalWrite(led_verde,HIGH);
       digitalWrite(sensor_emisor,HIGH);
       valor1 = analogRead(sensor_receptor);
       Serial.println("Voltaje on, en bucle while");
       Serial.println(valor1);
       if (valor1 >= 900){
         res1 = 900;
       }else if (valor1 <= 500){
         res1 = 500;
       }else{
         res1 = valor1;
       }
       lcd.clear();
       Serial.println(res1);
       lcd.setCursor(0,0);
       lcd.print("Cloro:");
       lcd.setCursor(0,1);
       cloro = map(res1,900,500,10,500);
       cloro = cloro/100;
       lcd.print(cloro);
       Serial.println(" Cloro");
       Serial.println(cloro);
       delay(500);
       digitalWrite(sensor_emisor,LOW);
       Serial.println("Voltaje off, en bucle while");
       delay(500);
   }  
 }
}
