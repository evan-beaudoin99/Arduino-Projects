#include <Keypad.h>

const byte ROWS = 4; // number of rows
const byte COLS = 4; // number of columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'#', '0', '*', 'D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5}; // row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; // column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int pinLength = 4;
const char pin[pinLength] = {'0', '8', '0', '6'};

const int relayPin = 13;

char input[pinLength];
int index = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting program...");

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

}

void loop()
{
  char key = keypad.getKey();

  if (key != NO_KEY) {
    input[index] = key;
    index++;

    if (index == pinLength) {
      Serial.print("PIN: ");
      printArray(input, pinLength);
      Serial.println();

      if (checkPin(input, pinLength)) {
        Serial.println("Correct");
        digitalWrite(relayPin, HIGH);
        delay(5000);
        digitalWrite(relayPin, LOW);
      } else {
        Serial.println("Wrong");
      }

      index = 0;
    }
  }
}

void printArray(const char *arr, int size)
{
  for (int i = 0; i < size; i++) {
    Serial.print(arr[i]);
  }
}

bool checkPin(const char *arr, int size)
{
  if (size != pinLength) {
    return false;
  }
  for (int i = 0; i < size; i++) {
    if (pin[i] != arr[i]) {
      return false;
    }
  }
  return true;
}
