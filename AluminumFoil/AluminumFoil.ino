/* CAPSENSE TEST SKETCH
 * Connect any desired number of foil capacitive switches to RECEIVE pins.
 * We use a common SEND pin
 * We use CapacitiveSensor objects array to work with the sensors
 * Connect 1MegaOhm (or more) resistor between common SEND and EACH RECEIVE
 * WHAT HAPPENS when we press a switch:
 *                                     a) Turn on a LED when any switch is pressed
 *                                     b) Turn off the LED when no switch is pressed
 *                                     c) Change capsense threshold through Serial
 *                                     d) Keep a counter for each switch, increase +1 on keyDown
 *                                     e) Output value of all the counters when any one of them increases
 */

#include <CapacitiveSensor.h>

#define SENDPIN 12    //Common Send pin
#define NSENSORS 6    //How many sensors are we using?
#define RECEIVEPIN_0 2    //We connect the foil on these pins.
#define RECEIVEPIN_1 3
#define RECEIVEPIN_2 4
#define RECEIVEPIN_3 5
#define RECEIVEPIN_4 6
#define RECEIVEPIN_5 7
#define LEDPIN 13
#define DEBOUNCING 100 //Debouncing time in ms

CapacitiveSensor sensors[NSENSORS] = { //Create CapacitiveSensor objects, definind each switch
  CapacitiveSensor(SENDPIN, RECEIVEPIN_0),
  CapacitiveSensor(SENDPIN, RECEIVEPIN_1),
  CapacitiveSensor(SENDPIN, RECEIVEPIN_2),
  CapacitiveSensor(SENDPIN, RECEIVEPIN_3),
  CapacitiveSensor(SENDPIN, RECEIVEPIN_4),
  CapacitiveSensor(SENDPIN, RECEIVEPIN_5)
};

unsigned int threshold = 1000;
unsigned int counter[NSENSORS] = {0, 0, 0, 0, 0, 0};
bool keyDown[NSENSORS] = {false, false, false, false, false, false}; //Keep last keyDown status
unsigned long lastKeyDown[NSENSORS] = {0, 0, 0, 0, 0, 0}; //Keep millis() of last keyDown event

void setup()
{
  for (byte i=0; i<NSENSORS; i++) { //Setup each switch
    sensors[i].set_CS_AutocaL_Millis(0xFFFFFFFF);
  }

  Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);
  Serial.println("Arduino CapacitiveSensor test LOADED");
  Serial.println();
}

void loop()
{

  if (Serial.available() > 0) { //Read serial input, modify threshold
    threshold = Serial.readStringUntil('\n').toInt();
    Serial.print("Threshold value changed to ");
    Serial.println(threshold);
  }

  unsigned long currentMillis = millis();
  bool anyKeyDown = false;
  bool refreshCounter = false;

  for (byte i=0; i<NSENSORS; i++) {
    if (sensors[i].capacitiveSensor(30) >= threshold) { //Key Down
      anyKeyDown = true;
      if (!keyDown[i] && (currentMillis - lastKeyDown[i] >= DEBOUNCING)) { //OnKeyDown (wasn't down previously)
        counter[i] = counter[i] + 1;
        refreshCounter = true;
        lastKeyDown[i] = currentMillis;
      }
      keyDown[i] = true;
    } else { //Key Up
      keyDown[i] = false;
    }
  }

  if (refreshCounter) {
    for (byte i=0; i<NSENSORS; i++) {
      Serial.print("C");
      Serial.print(i+1);
      Serial.print("=");
      Serial.print(counter[i]);
      if (i < NSENSORS - 1) {
        Serial.print(" | ");
      }
    }
    Serial.println();
  }

  if (anyKeyDown) {
    digitalWrite(LEDPIN, HIGH);
  } else {
    digitalWrite(LEDPIN, LOW);
  }

}
