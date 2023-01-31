#include <Keypad.h>
#include <LiquidCrystal.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
char wrd[6] = {};
int idx = 0;

char password[4] = "1334";  
int position = 0;
int Lock = 7;


//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LiquidCrystal lcd(5, 4, 38, 39, 40, 41);

byte rowPins[ROWS] = {30, 31, 32, 33}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {34, 35, 36, 37}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Password");

  pinMode(Lock, OUTPUT);
  LockedPosition(false);
 

}

void loop() {
  //lcd.clear();

  char key = keypad.getKey();
  lcd.setCursor(0, 1);
  if (key) {
    wrd[idx++] = key;
    Serial.println(key);
    //lcd.clear();

  }
  Serial.println("idx:" +  String(idx));
  lcd.print(wrd);
  Serial.println("Word:" +  String(wrd));
  if ( key == '#' && idx >= 4)
  {
    int f = 0;
    for (int i = 0; i < 4; i++)
    {
      if (password[i] != wrd[i])
      {
        f = 1;
        break;
      }
    }
    if (!f)
    {
      LockedPosition(true);
      Serial.println("successfully unlocked!");
      idx = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Success!");
      delay(3000);
      wrd[0] = '\0';
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Password");
      LockedPosition(false);

    }
    else{
       lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Incorrect!");
      delay(3000);
      wrd[0] = '\0';
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Password:");
    }
  }
  delay(1000);
}


void LockedPosition(int locked)
{
  if (locked)
  {
    digitalWrite(Lock, LOW);
  }
  else
  {
    digitalWrite(Lock, HIGH);
  }
}
