/*************************************************************
Motor Shield Stepper Demo
by Randy Sarafan

For more information see:
http://www.instructables.com/id/Arduino-Motor-Shield-Tutorial/

*************************************************************/

// Definicion de tiempos de espera
#define DOSSEGUNDOS 2000
int delaylegnth = 500;

// Parametros del sensosor de luz
#define PIN_LDR 0

// Modos de barrido
#define CALIBRAR 1
#define MEDIR 2

// Opeciones del stepper
#define _PWM 255
#define MAX_CYCLES 3
#define MEASSURES_PER_CYCLE 4

// Array temporal para almacenar las medidas en cada ciclo de paso
int tmp[MEASSURES_PER_CYCLE];

// Arrays que registran la de medida de intensidad de la luz
int i_0_array[MAX_CYCLES * MEASSURES_PER_CYCLE];
int i_array[MAX_CYCLES * MEASSURES_PER_CYCLE];

// Variable de lectura de comandos
char c = 'N';

/*
 *    --- SETUP ---
 */
void setup() {

  // Se inicializa el Serial  
  Serial.begin(9600);
  
  // Se establecen los pines de direccion del stepper
  pinMode(12, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
  pinMode(13, OUTPUT); //CH B -- HIGH = forwards and LOW = backwards???
  
  // Se establecen los pines de freno del stepper
  pinMode(9, OUTPUT); //brake (disable) CH A
  pinMode(8, OUTPUT); //brake (disable) CH B

  digitalWrite(9, HIGH); //DISABLE CH A
  digitalWrite(8, HIGH); //DISABLE CH B
  
  // Espera a Serial
  while(!Serial);
  delay(100);
  
}

/*
 *    --- LOOP ---
 */
void loop() {

  //  Espera hasta que reciba una 'S' por Serial
  wait_Serial();
  
  //  Espera 2 segundos
  delay(DOSSEGUNDOS);
  //  Avanza MAX_CYCLES pasos en modo CALIBRAR
  x_steps_ford(MAX_CYCLES, CALIBRAR);

  // Retorna a la posicion inicial
  return_carriage();

  Serial.println(" [OK] ... Calibrado terminado ");
  Serial.println(" *** Inciando modo de medida *** ");
  
  //  Espera hasta que reciba una 'S' por Serial
  wait_Serial();

  //  Espera 2 segundos
  delay(DOSSEGUNDOS);

  //  Avanza MAX_CYCLES pasos en modo MEDIR
  x_steps_ford(MAX_CYCLES, MEDIR);

  //  Espera 2 segundos
  delay(DOSSEGUNDOS);  
  
  // Retorna a la posicion inicial
  return_carriage();

  // Marca el final de la medida
  Serial.println(" [OK] ... Medicion terminada ");
  Serial.println(" --- --- --- --- --- --- --- --- --- ");

  // Vuelca los datos por serial en formato CSV
  data_to_serial_in_csv_format();

  // Termina el programa. Para reiniciarlo pulsar el RESET
  while(true);

}

/*
 *  
 */
void data_to_serial_in_csv_format() {

  // Incial el envio de los datos en formato CSV
  Serial.println("[CSV-START]");

  // Cabecera del fichero
  Serial.println("i_0,i");
  
  // Volcado de cada muestra de intensidad de luz tomada,
  //   tanto para el calibrado como para la medida
  for (int i = 0; i < MAX_CYCLES * MEASSURES_PER_CYCLE; i++) {
    Serial.print(i_0_array[i]);
    Serial.print(",");
    Serial.print(i_array[i]);
    Serial.println();
  }
  
  // Finaliza el envio de los datos en formato CSV
  Serial.println("[CSV-END]");
  
}

/*
 *  Mueve el stepper hacia atras a la posicion inicial
 */
void return_carriage() {

  //  Espera 2 segundos
  delay(DOSSEGUNDOS);

  //  Retrocede MAX_CYCLES pasos
  x_steps_back(MAX_CYCLES);

}

/*  
 *    Espera hasta que reciba una 'S' por Serial
 */
void wait_Serial() {
  
  // Inicializamos el caracter de comando a 'N' para bloquear
  //    el flujo de ejecucion hasta 
  c = 'N';
  
  // Mensaje de como iniciar la rutina
  Serial.println(" [S + <enter>] para continuar ");
  
  // Bucle de espera inicial
  while ( c != 'S') {
        // Lee de Serial solo cuando hay datos disponibles
        if (Serial.available() > 0) {
                // Lee el caracter de entrada
                c = Serial.read();
        }
  }
  // Muestra el mensaje de inicio
  Serial.println("Empezando ..."); 
  
}

/*
 *  Avanaz x pasos hacia adelante midiendo los valores de
 *    calibrado (modo CALIBRAR). Guardando estos valores 
 *    en el array i_0. O bien, midiendo valores sobre 
 *    la muestra de interes (modo MEDIDA)
 */
void x_steps_ford(int n_cycles, int modo) {
  
  // Bucle del barrido
  for (int i=0; i < n_cycles; i++) {
    // Mueve un ciclo de paso y toma 4 medidas
    move_and_get_light_intensity(i, modo);
  }
  
}

#define DEBUG 1

/*
 *  Obtine la intensidad de la luz a partir del valor analogico
 *    del LDR. Alamecen el valor en los arrays i_0 o i en funcion
 *    del modo de medida (CALIBRAR o MEDIR)
 */ 
void move_and_get_light_intensity(int i, int modo) {
  
    // Ejecuta un ciclo de paso y toma 4 medidas en tmp
    step_ford();
  
    // Agregamos los datos de lectura al array correspondiente
    //  en funcion del modo de lectura
    for (int offset = 0; offset < MEASSURES_PER_CYCLE; offset++) {
      
#ifdef DEBUG
      Serial.print("i=");
      Serial.print(i*MEASSURES_PER_CYCLE+offset);
      Serial.print("   v=");
      Serial.print(tmp[offset]);
      Serial.println();
#endif
      
      // Lee el valor del LDR y lo guarda en el array de calibrado
      if (modo == CALIBRAR)
          i_0_array[i*MEASSURES_PER_CYCLE+offset] = tmp[offset];
      // o en el array de intesidad de la luz (segun el modo)
      else
          i_array[i*MEASSURES_PER_CYCLE+offset] = tmp[offset];
    }
    
}

/*
 *  Avanza un ciclo completo de paso hacia adelante
 *    _PWM marca la intensidad con la que se mueve el paso
 *    delaylenght es el retraso en ms entre cada medio paso
 */
void step_ford() {
 
  digitalWrite(9, LOW);  //ENABLE CH A
  digitalWrite(8, HIGH); //DISABLE CH B

  digitalWrite(12, HIGH);   //Sets direction of CH A
  analogWrite(3, _PWM);   //Moves CH A
  
  // Toma medida en el medio paso 1
  tmp[0] = analogRead(PIN_LDR);
  
  delay(delaylegnth);
  
  digitalWrite(9, HIGH);  //DISABLE CH A
  digitalWrite(8, LOW); //ENABLE CH B

  digitalWrite(13, HIGH);   //Sets direction of CH B
  analogWrite(11, _PWM);   //Moves CH B

  // Toma medida en el medio paso 2
  tmp[1] = analogRead(PIN_LDR);
  
  delay(delaylegnth);
  
  digitalWrite(9, LOW);  //ENABLE CH A
  digitalWrite(8, HIGH); //DISABLE CH B

  digitalWrite(12, LOW);   //Sets direction of CH A
  analogWrite(3, _PWM);   //Moves CH A

  // Toma medida en el medio paso 3
  tmp[2] = analogRead(PIN_LDR);
  
  delay(delaylegnth);
    
  digitalWrite(9, HIGH);  //DISABLE CH A
  digitalWrite(8, LOW); //ENABLE CH B

  digitalWrite(13, LOW);   //Sets direction of CH B
  analogWrite(11, _PWM);   //Moves CH B
 
  // Toma medida en el medio paso 4
  tmp[3] = analogRead(PIN_LDR);
  
  delay(delaylegnth);
  
}

/*
 *  Retrocede x pasos
 */
void x_steps_back(int x) {
  
  // Bucle del recorrido
  for (int i = 0; i < x; i++) {
    // Avanza un ciclo de step hacia atras
    step_back();
  }
  
}

/*
 *  Avanza un ciclo completo de paso hacia atras
 *    _PWM marca la intensidad con la que se mueve el paso
 *    delaylenght es el retraso en ms entre cada medio paso
 */
void step_back() {
  
  digitalWrite(9, LOW);  //ENABLE CH A
  digitalWrite(8, HIGH); //DISABLE CH B

  digitalWrite(12, LOW);   //Sets direction of CH A
  analogWrite(3, _PWM);   //Moves CH A
  
  delay(delaylegnth);
    
  digitalWrite(9, HIGH);  //DISABLE CH A
  digitalWrite(8, LOW); //ENABLE CH B

  digitalWrite(13, HIGH);   //Sets direction of CH B
  analogWrite(11, _PWM);   //Moves CH B
  
  delay(delaylegnth);
  
  digitalWrite(9, LOW);  //ENABLE CH A
  digitalWrite(8, HIGH); //DISABLE CH B

  digitalWrite(12, HIGH);   //Sets direction of CH A
  analogWrite(3, _PWM);   //Moves CH A
  
  delay(delaylegnth);
  
  digitalWrite(9, HIGH);  //DISABLE CH A
  digitalWrite(8, LOW); //ENABLE CH B

  digitalWrite(13, LOW);   //Sets direction of CH B
  analogWrite(11, _PWM);   //Moves CH B
  
  delay(delaylegnth);
  
}

