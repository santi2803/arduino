#include <AT24CX.h>

#include <Wire.h>

#include <RTClib.h>

#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>

#define TAM 4
const int eepromAddress = 0x50;

RTC_DS1307 rtc;
AT24C32 EepromRTC;

/*===========================
Conection LCD to Arduino
=============================*/

/*
  LCD RS to PIN A0
  LCD E to PIN A1
  LCD D4 to PIN A2
  LCD D5 to PIN A3
  LCD D6 to PIN 12
  LCD D7 to PIN 13
*/
LiquidCrystal lcd(A0, A1, A2, A3, 12, 13);

/*==========================
 * Configuration keyboard
============================*/

/*

*/
const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},  
};
/*
  Filas y columnas a las que van conectado el teclado
*/
byte rowPins[rows] = {9,8,7,6};
byte colPins[cols] = {5,4,3,2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

// Global variables
int digito = 0;
char password[] = "2222";
int reporte[8];
char codigo[4];
int opc, contador, dato, posicion, uid;
char opcC[] = "123456";
bool isAdmin = false;
bool ismenu = false;
bool pwmenu = false;
bool opc_member = false;
short pos_libre;
bool newpin = false;
//char currentUser[3]="";
int pos;

// Arreglo con nombre
const char* nombre[TAM] = {"Santiago Ruiz", "Miguel Salamanca", "Juan Avellaneda"};

struct Passwords{
   char pw_SA[4];
   char pw_MA[4];
   char pw_JA[4];
};

void setup() {
  // put your setup code here, to run once:
  int eeAdress = 0;
  Passwords pws = {
    "1234",
    "4321",
    "1324"
  };
  EEPROM.put( eeAdress, pws );
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();

  //byte variable_byte=235;
  /*
  for (int i = 0; i<280; i++) {
    EepromRTC.write(i,0);
  }
  byte a = EepromRTC.read(1);
  Serial.print("Dato byte: ");Serial.println(a);
  contador = 0;
  posicion = 0;
  */
  

  
  
  
  
  //i2c_eeprom_write_page(eepromAddress, posicion,(byte *)0, sizeof(255));
  //char somedata[] = "datos grabados en la EEPROM";
   // Escribir ejemplo en memoria
  //i2c_eeprom_write_page(eepromAddress, 0, (byte *)somedata, sizeof(somedata)); 

  //rtc.adjust(DateTime(__DATE__, __TIME__));
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Bienvenidos");
  lcd.setCursor(0,1);
  lcd.print("Arquitectura");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  // print the number of seconds since reset:

  lcd.print("Grupo 7");
  lcd.setCursor(0,1);
  lcd.print(nombre[0]);
  delay(2000);
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print(nombre[1]);
  lcd.setCursor(0,1);
  lcd.print(nombre[2]);
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ingrese pin");

}

void loop() {
  // put your main code here, to run repeatedly:
  teclado();
}

/*==============================
 * Reloj
================================*/
void reloj() {
    DateTime now = rtc.now();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Date: ");
    lcd.print(now.day());
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.year(), DEC);
    
    lcd.setCursor(0,1);
    lcd.print("Hour: ");
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.print(now.second(), DEC);
    delay(1000);
}

/*=========================
FUNCION DEL TECLADO
===========================*/
void teclado() {
  char tecla = keypad.getKey();
  if (tecla != NO_KEY) {
    if (!ismenu){
      codigo[digito] = tecla; // condigo[0] = "2" condigo[1] = "A"
      lcd.setCursor(digito + 5,2);
      lcd.print('*');
      digito++;
      if (digito == 4) {
        if(!newpin) {
          if (!pwmenu) {
            check_password();
          } else {
           check_pw_member();
          }
        } else {
          new_pw();
        }
      }
      } else {
        opc = tecla;
        lcd.setCursor(digito + 5,2);
        lcd.print(tecla);
        if (digito == 0) {
          if (!opc_member) {
              opc_menu();
          } else {
              menu_opc_member();
          }
        }
      }
    }
 }

  /*=============================
  LEER y ESCRIBIR MEMORIA EXTERNA
  ==============================*/
  /*
  void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data )
  {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8));   // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(data);
    Wire.endTransmission();
   
    delay(5);
  }
  */
  void i2c_eeprom_write_page(int pos, byte* data, byte length) {
   byte c;
   for (c = 0; c < length; c++)
      EepromRTC.write(pos, data[c]);

   delay(5);
}

  /*========================
   * LEER Y CREAR REPORTE
  ==========================*/
  void getReportes() {
    const char* opt[] = {"REPORTE", "Day", "Month", "Year", "Hour", "Minute", "Second", "Id"};
    int add = 0;
    int opts = 0;
    byte value = EepromRTC.read(add);
    while (value != 0) {
      // Print to lcd
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(opt[opts]);
      lcd.print(" ");
      lcd.print(value);
      delay(1000);

      lcd.clear();
      // Print to serial
      Serial.print(opt[opts]);
      Serial.print(" ");
      Serial.print((int)value);
      //Serial.println(" ");
      Serial.print(" ");
      add++;
      opts++;
      value = EepromRTC.read(add);     
      if (add == 8 || add == 16 || add == 24 || add == 32 || add == 40 || add == 48 || add == 56 || add == 64 || add == 72 || add == 80 || add == 88 || add == 96 || add == 104) {
        opts = 0;
      }
    }
    Serial.println(" sa ");
    delay(100);
    lcd.print("Fin...");
  }
  
  /*
  void reporte(int id) {
    DateTime now = rtc.now();
    reporte[0] = contador;
    reporte[1] = now.day();
    reporte[2] = now.month();
    reporte[3] = now.year();
    reporte[4] = now.hour();
    reporte[5] = now.minute();
    reporte[6] = now.second();
    reporte[7] = id;
  }
  */

  void buscar_pos_libre() {
    posicion = 0;
    pos_libre = false;
    contador = 0;
    do {
      
      dato = EepromRTC.read(posicion);
      
      if (dato == 0) {
        pos_libre = true;
      } else {
        posicion += 8;      
      }
      contador++;
    } while (pos_libre == false);
  }

 /*==========================
 FUNCION NUEVO PIN
 ============================*/
 void new_pw() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Nuevo Pin");
 }


 /*==========================
 CONFIRMAR CONTRASEÑA CADA MIEMBRO
 ============================*/
  void check_pw_member() {
    int eeAddress = 0; //EEPROM address to start reading from     
    

    //eeAddress  += sizeof(float);
    Passwords res;
    EEPROM.get( eeAddress, res );
    /*================================================
    Verify pin santiago
    ==================================================*/
    if(opc == opcC[0]) {
      if (codigo[0] == res.pw_SA[0] && codigo[1] == res.pw_SA[1] && codigo[2] == res.pw_SA[2] && codigo[3] == res.pw_SA[3]) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Correct");
        digito = 0;
        codigo[0]="";
        codigo[1]="";
        codigo[2]="";
        codigo[3]="";
        pwmenu=false;
        ismenu=true;
        uid = 29;
        isAdmin = true;
        opc_member=true;
        //currentUser="SA"
        delay(2000);
        lcd.clear();
        menu_member();
      } else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Incorrect");
        digito = 0;
        codigo[0]="";
        codigo[1]="";
        codigo[2]="";
        codigo[3]="";
        pwmenu=true;
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Pin Santiago");
      }
    }
     /*================================================
    Verify pin salamanca
    ==================================================*/
    if (opc == opcC[1]) {
        if (codigo[0] == res.pw_MA[0] && codigo[1] == res.pw_MA[1] && codigo[2] == res.pw_MA[2] && codigo[3] == res.pw_MA[3]) {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Correct");
          digito = 0;
          codigo[0]="";
          codigo[1]="";
          codigo[2]="";
          codigo[3]="";
          uid = 30;
          ismenu=true;
          pwmenu=false;
          opc_member=true;
          isAdmin = false;
          //currentUser="MA"
          delay(2000);
          lcd.clear();
          menu_member();
        } else {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Incorrect");
          digito = 0;
          codigo[0]="";
          codigo[1]="";
          codigo[2]="";
          codigo[3]="";
          pwmenu=true;
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Pin Miguel");
      }
    }
     /*================================================
    Verify pin avellaneda
    ==================================================*/

    if (opc == opcC[2]) {
          if (codigo[0] == res.pw_JA[0] && codigo[1] == res.pw_JA[1] && codigo[2] == res.pw_JA[2] && codigo[3] == res.pw_JA[3]) {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Correct");
            digito = 0;
            codigo[0]="";
            codigo[1]="";
            codigo[2]="";
            codigo[3]="";
            pwmenu=false;
            uid = 6;
            ismenu=true;
            opc_member=true;
            isAdmin = false;
            //currentUser="JA"
            delay(2000);
            lcd.clear();
            menu_member();
          } else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Incorrect");
            digito = 0;
            codigo[0]="";
            codigo[1]="";
            codigo[2]="";
            codigo[3]="";
            pwmenu=true;
            delay(2000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Pin Avellaneda");
      }
    }
    /*================================================
    Verify admin and get reportes
    ==================================================*/
    if(opc == opcC[4]) {
      if (codigo[0] == res.pw_SA[0] && codigo[1] == res.pw_SA[1] && codigo[2] == res.pw_SA[2] && codigo[3] == res.pw_SA[3]) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Correct");
        digito = 0;
        codigo[0]="";
        codigo[1]="";
        codigo[2]="";
        codigo[3]="";
        pwmenu=false;
        opc_member = false;
        ismenu = true;
        getReportes();
        delay(3000);
        lcd.clear();
        menu();
      } else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Incorrect");
        digito = 0;
        codigo[0]="";
        codigo[1]="";
        codigo[2]="";
        codigo[3]="";
        pwmenu=true;
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Pin admin");
      }
    }
    /*===============================
     * Change hour and date
    =================================*/
    
    
  }
  /*========================
  CONFIRMAR CONTRASEÑA DISEÑADPR
  ==========================*/
  void check_password() {
    
      if (codigo[0] == password[0] && codigo[1] == password[1] && codigo[2] == password[2] && codigo[3] == password[3]) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Correct");
        digito = 0;
        ismenu = true;
        codigo[0]="";
        codigo[1]="";
        codigo[2]="";
        codigo[3]=""; 
        delay(2000);
        lcd.clear();
        menu();
      } else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Incorrect");
        digito = 0;
        codigo[0]="";
        codigo[1]="";
        codigo[2]="";
        codigo[3]="";
        delay(3000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ingrese Pin");      
      }
      
  }

  /*===========================
  MOSTRAR MENU
  =============================*/

  void menu() {
    for (int i = 0; i<=11; i++) {
      reloj();
    }
    delay(100);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Bienvenido al");
    lcd.setCursor(0,1);
    lcd.print("menu disenador");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("1)SA    2)MA");
    lcd.setCursor(0,1);
    lcd.print("3)JA    4)ch h/d");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("5)V RE   6)Salir");
    delay(2000);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ingrese opcion");
    
  }

  /*===========================
  MOSTRAR OPCIONES DEL MENU
  =============================*/
  
  void opc_menu() {
    ismenu=false;
    pwmenu=true;
    if (opc == opcC[0]) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Pin Santiago");
    }

    if (opc == opcC[1]) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Pin Miguel");
    }

     if (opc == opcC[2]) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Pin Avellaneda");
     }

     if (opc == opcC[3]) {
         // Cambiar fecha y hora
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Pin admin");
     }

     if (opc == opcC[4]) {
         // Ver reportes
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Pin admin");
     }

     if (opc == opcC[5]) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Adios");
        ismenu=false;
        pwmenu=false;
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ingrese pin"); 
     }
  }

  /*=====================
  Mostrar menu miembro
  =======================*/
  
  void menu_member() {
    lcd.setCursor(0,0);
    lcd.print("1).Cambiar PW");
    lcd.setCursor(0,1);
    lcd.print("2).RE  3)Salir"); 

    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ingrese Opcion");
  }


  void menu_opc_member() {
      if (opc == opcC[0]) {
        lcd.clear();
        newpin = true;
        ismenu = false;
        lcd.setCursor(0,0);
        lcd.print("Nuevo Pin");
      }

      if (opc == opcC[1]) {
          lcd.clear();
          lcd.setCursor(0,0);
          buscar_pos_libre();
          if (pos_libre == true) {
            //Serial.println(posicion);
            Serial.println(contador);
            DateTime now = rtc.now();
            reporte[0] = contador;
            reporte[1] = now.day();
            reporte[2] = now.month();
            reporte[3] = now.year();
            reporte[4] = now.hour();
            reporte[5] = now.minute();
            reporte[6] = now.second();
            reporte[7] = uid;
            
            // i2c_eeprom_write_page(posicion,(byte *)reporte, sizeof(reporte));
            for (int i = 0; i < 8; i++) {   
              EepromRTC.write(posicion,(byte *)reporte[i]);               
              delay(5);
              posicion = posicion + 1;
            }
            
            lcd.print("Creando reporte"); 
            delay(500);
            lcd.clear();
          } else {
            if (posicion <= 240) {
              lcd.print("Memoria llena");
              delay(1000);
            }
          }
          opc_member=true;
          menu_member();
      }

      if (opc == opcC[2]) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Adios");
        delay(2000);
        opc_member = false;
        ismenu = true;
        menu();
      }
     
     
  }

  
