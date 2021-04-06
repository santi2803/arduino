#include <Key.h>
#include <Keypad.h>

#include <LiquidCrystal.h>

#include <EEPROM.h>

#define TAM 4

/*===========================
Conection LCD to Arduino
=============================*/

/*
  LCD RS to PIN A0
  LCD E to PIN A1
  LCD D4 to PIN A2
  LCD D5 to PIN A3
  LCD D6 to PIN A4
  LCD D7 to PIN A5
*/
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

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
char password[] = "2020";
char codigo[4];
int opc;
char opcC[] = "1245";
bool ismenu = false;
bool pwmenu = false;
bool opc_member = false;
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

 /*==========================
 FUNCION NUEVO PIN
 ============================*/
 void new_pw() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Nuevo Pin");
 }

 /*===========================
 GUARDAR NUEVO PIN
 =============================*/
 /*
  void save_new_pin() {
  if(currentUser[1] == "S" && currentUser[2] == "A") {
    
  }
 }
 */

 /*==========================
 CONFIRMAR CONTRASEÑA CADA MIEMBRO
 ============================*/
  void check_pw_member() {
    int eeAddress = 0; //EEPROM address to start reading from     
    

    //eeAddress  += sizeof(float);
    Passwords res;
    EEPROM.get( eeAddress, res );
    Serial.println(res.pw_SA);
    Serial.println(res.pw_MA);
    
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
          ismenu=true;
          pwmenu=false;
          opc_member=true;
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
            ismenu=true;
            opc_member=true;
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
    lcd.setCursor(0,0);
    lcd.print("Bienvenido al");
    lcd.setCursor(0,1);
    lcd.print("menu disenador");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("1)SA    2)MA");
    lcd.setCursor(0,1);
    lcd.print("4)JA    5)Salir");
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
    lcd.print("2) Salir");

    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ingrese Opcion");
  }


  void menu_opc_member() {
     if (opc == opcC[0]) {
        lcd.clear();
        newpin = true;
        lcd.setCursor(0,0);
        lcd.print("Nuevo Pin");
     }

     if (opc == opcC[1]) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Adios");
        delay(2000);
        opc_member = false;
        ismenu = true;
        menu();
     }
  }

  
