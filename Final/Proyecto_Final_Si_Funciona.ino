#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <DS1302.h>
#include <LiquidCrystal_I2C.h>

#define SENSOR_OUT A0 //PIN DEL SENSOR DE CORRINETE
#define RELE_2 2      //SENSOR DE CORRIENTE
#define RELE_3 3      //FOCO ILUMINADOR
#define SD_CS 4       //PIN DEL LECTOR DE MICRO SD
#define SD_SCK 13     //PIN DEL LECTOR DE MICRO SD
#define SD_MISO 12    //PIN DEL LECTOR DE MICRO SD
#define SD_MOSI 11    //PIN DEL LECTOR DE MICRO SD
#define RTC_CLK 6     //PIN DEL RTC
#define RTC_DAT 7     //PIN DEL RTC
#define RTC_RST 8     //PIN DEL RTC
#define LCD_SDA A4    //PIN ANALOGO DEL PANEL LCD
#define LCD_SCL A5    //PIN ANALOGO DEL PANEL LCD

int INICIO_DATA = 6;      // INICIO DE LAS MEDICIONES
int FIN_DATA = 18;        // FIN DE LAS MEDICIONES
int INICIO_ALUMBRADO = 0; //HORA DE INICIO DEL ALUMBRADO FORMATO 24 HORAS
int FRECUENCIA = 2;       // FRECUENCIA DEL REGISTRO EN MINUTOS
long lectura;
int minuto, newminuto;
byte massb = 0x0f;
byte menossb = 0xf0;
long totalRadiacion = 0;
long promedioRadiacion = 0;
int mediciones = 0;
bool condicion = true;    // VARIABLE A EVALUAR PARA QUE LA ILUMINACION SOLO SE EJECUTE UNA SOLA VEZ ( TRUE - FALSE )

DS1302 rtc(RTC_RST, RTC_DAT, RTC_CLK);  //CONFIGURACION DE LOS PUERTOS DEL RTC A TRAVES DE LA LIBRERIA
LiquidCrystal_I2C lcd(0x27, 16, 2);     //CONFIGURACION BASICA DEL TAMANIO DEL LCD

void setup() {
  pinMode(RELE_2, OUTPUT);    //CONFIGURAR EL PUERTO DEL PIN 2 COMO PUERTO DE SALIDA
  pinMode(RELE_3, OUTPUT);    //CONFIGURAR EL PUERTO DEL PIN 3 COMO PUERTO DE SALIDA
  Serial.begin(9600);         //CONFIGURAR EL PUERTO SERIAL DEL COMPUTADOR A 9600
  Wire.begin();               //CONFIGURACION DEL ARDUINO PARA USAR PUERTOS I2C
  pinMode(SENSOR_OUT, INPUT); //CONFIGURAR EL PUERTO ANALOGO A0  COMO PUERTO DE ENTRADA
  pinMode(SD_CS, OUTPUT);     //CONFIGURAR EL PUERTO 4 COMO SALIDA DEL LECTOR DE MEMORIAS
  digitalWrite (RELE_2, HIGH);//CONFIGURA EL PUERTO 2 DEL RELE POR PRIMERA VEZ A "NORMALMENTE CERRADO"
  digitalWrite (RELE_3, HIGH);//CONFIGURA EL PUERTO 3 DEL RELE POR PRIMERA VEZ A "NORMALMENTE CERRADO"
  if (!SD.begin(SD_CS)) {     //VERIFICA SI HAY SENIAL DESDE EL LECTOR DE TARJETA
    Serial.println("Fallo o tarjeta no presente");
  } else {
    Serial.println("Tarjeta SD OK");
  }
    lcd.init();               //INICIA EL LCD
    lcd.backlight();          //ENCIENDE LA LUZ DE FONDO DEL PANEL LCD
    
}

void loop() {

    Bienvenida();   //MUESTRA POR EL LCD UN MENSAJE DE BIENVENIDA 
    
    Time now =  rtc.getTime();    //OBTIENE LA HORA DEL RTC Y LA GUARDA EN UNA ESTUCTURA ACORDE A LA MISMA
    
    if (now.hour < FIN_DATA && now.hour >= INICIO_DATA) {   //EVALUA EL PERIODO PARA POSTETIORMENTE CREAR LOS REGISTROS
      condicion = true;           //CAMBIA A VERDADERO LA VARIABLE CONDICION
      minuto = 0;                 //REINICIA LA VARIABLE MINUTO
      while (minuto < FRECUENCIA) //BUCLE PARA CREAR UN DELAY BASADO EN LA FRECUENCIA
      {
        for(int min=0;min<60;min++){    //BUCLE INTERNO PARA QUE PASE UN MINUTO
          delay(1000);
        }
        minuto++;
      }
      lectura = leersensor();     //INVOCACION DE LA FUNCION PARA OBTENER LOS DATOS DEL SENSOR DE CORRIENTE Y GUARDARLO EN LA VARIABLE LECTURA
      totalRadiacion += lectura;  //ALMACENA TODAS LAS MEDICIONES DE LOS REGISTROS EN UN ACUMULADOR
      mediciones++;               //ALMACENA TODAS LAS MEDICIONES DE LOS REGISTROS EN UN CONTADOR

      File dataFile = SD.open("solar.txt", FILE_WRITE);   //ABRE EL ARCHIVO DE LOS REGISTROS COMO ESCRITURA Y PERMITE INVOCAR AL MISMO DESPUES
      if (dataFile) {             //VERIFICA SI EL ARCHIVO SE ABRIO CORRECTAMENTE, MUESTRA Y GUARDA LOS DATOS DE LA LECTURA
        dataFile.print(now.date,DEC);
        dataFile.print("/");
        dataFile.print(now.mon,DEC);
        dataFile.print("/");
        dataFile.print(now.year,DEC);
        dataFile.print(" -- ");
        dataFile.print(now.hour,DEC);
        dataFile.print(":");
        dataFile.print(now.min,DEC);
        dataFile.print("   I: ");
        dataFile.print(lectura);
        dataFile.println();
        dataFile.close();
        Serial.println(" Reporte OK");
        lcd.setCursor(0,0);
        lcd.print("               ");
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,0);
        lcd.print("Reporte");
        lcd.setCursor(0,1);
        lcd.print(now.hour,DEC);
        lcd.print(":");
        lcd.print(now.min,DEC);
        lcd.setCursor(9,1);
        lcd.print(" I:");
        lcd.print(lectura);
      } else {            //EN CASO DE QUE NO SE ABRIO MUESTRA EL MENSAJE
        lcd.setCursor(0,0);
        lcd.print("               ");
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,0);
        lcd.print("Error al abrir");
        lcd.setCursor(0,1);
        lcd.print("Ingrese tarjeta");
      }
      delay(10000);         //ESPERA DE 10 SEGUNDOS MIENTRSAS SE MUESTRA EL ULTIMO REGISTRO
    }else{
      if(now.hour == INICIO_ALUMBRADO){   //VERIFICA LA IGUALDAD ENTRE LA HORA ACTUAL Y LA HORA EN LA CUAL SE ENCENDERAN LAS LUCES
        if(condicion){                    //EVALUA CONDICION
          promedioRadiacion = totalRadiacion / mediciones;    //SACA UN PROMEDIO ENTRE EL ACUMULADOR Y EL CONTADOR
          Iluminacion(promedioRadiacion); //ENVIA EL PROMEDIO A LA FUCNION PARA ENCENDER LAS LUCES
          totalRadiacion = 0;             //REINICIA LA VARIABLE DEL ACUMULADOR TOTAL DE RADIACION DEL DIA
          mediciones = 0;                 //REINICIA EL CONTADOR DE MEDICIONES DEL DIA
          promedioRadiacion = 0;          //REINICIA LA VARIABLE PROMEDIOCONTADOR
          condicion = false;              //CAMBIA LA VARIABLE CONDICION PARA QUE LA ILUMINACION SOLO SE EJECUTE UNA SOLA VEZ
        }   
      }else{
        delay(3600000);         //ESPERANDO UNA HORA PARA VOLVER A EVALUAR
      }
    } 
}


long leersensor() {
  int i, num_muestras = 100;
  long voltaje;
  long corriente = 0;
  long acum = 0;
  digitalWrite(RELE_2, LOW);      //ENCIENDE EL RELE Y CREA UN CIRCUITO CERRADO ENTRE EL PANEL SOLAR Y EL SENSOR DE CORREINTE
  delay(5000);                    //REALIZA UNA ESPERA DE 5 SEGUNDOS
  for (i = 0; i < num_muestras; i++) {      //BUCLE PARA REALIZAR 100 MUESTRAS DEL SENSOR
    voltaje = analogRead(SENSOR_OUT) - 512;
    acum += (voltaje * 512 / 5 );
    delay(20);      // Al tener 10 milisegundos y tener 100 muestras tardara 1 segundo en hacer el bucle.
  }
  digitalWrite(RELE_2, HIGH);     //APAGA EL RELE PARA QUE LA CORRIENTE DEL PANEL PASE A LA BATERIA
  corriente = acum / num_muestras;//REALIZA UN PROMEDIO DE TODAS LAS MUESTRAS
  return corriente;               //RETORNA EL PROMEDIO
}

void Bienvenida(){      //MENSAJES DE BIENVENIDA
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  BIENVENIDOS  ");  
    lcd.setCursor(0,1);
    lcd.print("  A LA FERIA ");
    delay(5000);        //DELAY DE 5 SEGUNDOS
    lcd.setCursor(0,0);
    lcd.print("  XVI JORNADA ");  
    lcd.setCursor(0,1);
    lcd.print("  CIENTIFICA ");
    delay(5000);        //DELAY DE 5 SEGUNDOS
    lcd.setCursor(0,0);
    lcd.print("  DISPOSITIVO ");  
    lcd.setCursor(0,1);
    lcd.print("     SOLAR   ");
    delay(5000);        //DELAY DE 5 SEGUNDOS
    lcd.setCursor(0,0);
    lcd.print("   PLANTA DE  ");  
    lcd.setCursor(0,1);
    lcd.print("   PITAHAYA  ");
    delay(5000);        //DELAY DE 5 SEGUNDOS
}

void Iluminacion(long promedioRadiacion){     //EVALUA LA CONDICION PARA ENCENDER LAS LUCES DE LA PLANTA DEPENDIENDO DE LA RADIACION
  if(promedioRadiacion < 50){                 //RANGO 1
    lcd.setCursor(0,0);
    lcd.print("               ");
    lcd.setCursor(0,1);
    lcd.print("               ");
    lcd.setCursor(0,0);
    lcd.print(" RANGO BAJO");
    lcd.setCursor(0,1);
    lcd.print(" TIEMPO: 6HRS");
    digitalWrite (RELE_2, LOW);
    delay(21600000);//6horas
    //delay(2000);
    digitalWrite (RELE_2, HIGH);
    delay(1000);
  }
  if(promedioRadiacion >= 50 && promedioRadiacion <= 75){ //RANGO 2
    lcd.setCursor(0,0);
    lcd.print("               ");
    lcd.setCursor(0,1);
    lcd.print("               ");
    lcd.setCursor(0,0);
    lcd.print(" RANGO MEDIO");
    lcd.setCursor(0,1);
    lcd.print(" TIEMPO: 4HRS");
    digitalWrite (RELE_3, LOW);
    delay(14400000);//4horas
    digitalWrite (RELE_3, HIGH);
    delay(1000);
  }
  if(promedioRadiacion > 75){     //RANGO 3
    lcd.setCursor(0,0);
    lcd.print("               ");
    lcd.setCursor(0,1);
    lcd.print("               ");
    lcd.setCursor(0,0);
    lcd.print(" RANGO ALTO");
    lcd.setCursor(0,1);
    lcd.print(" TIEMPO: 2HRS");
    digitalWrite (RELE_3, LOW);
    delay(7200000);//2horas
    digitalWrite (RELE_3, HIGH);
    delay(1000);
  }
}

