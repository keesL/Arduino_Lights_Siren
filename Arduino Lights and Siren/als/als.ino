// Lightbar, siren and horn for Arduino
// (C) 2013 by Kees Leune <kees@leune.org>
// https://github.com/keesL/Arduino_Lights_
#include <EEPROM.h>
#include <math.h>

// pin definitions
const int RED1            = 2;
const int RED2            = 3;
const int BLUE1           = 6;
const int BLUE2           = 7;
const int maxLEDpin       = 8;

const int PATTERN_SWITCH  = 8;
const int SIREN           = 9;
const int SIREN_TOGGLE    = 10;
const int SIREN_SWITCH    = 11;
const int BUZZER_SWITCH   = 12;

// LED states
const int STATE_OFF       = 1;
const int STATE_RED       = 2;
const int STATE_BLUE      = 4;
const int STATE_RED1      = 8;
const int STATE_RED2      = 16;
const int STATE_BLUE1     = 32;
const int STATE_BLUE2     = 64;

// patterns
const int PATTERN_OFF     = 0;
const int PATTERN_ONE     = 1;
const int PATTERN_TWO     = 2;
const int PATTERN_THREE   = 3;
const int PATTERN_FOUR    = 4;
const int NUMPATTERNS     = 5;

// sirens
const int SIREN_OFF       = 0;
const int SIREN_ONE       = 1;
const int SIREN_TWO       = 2;
const int SIREN_THREE     = 3;
const int SIREN_FOUR      = 4;
const int SIREN_FIVE      = 5;
const int NUMSIRENS       = 6;

// EEPROM
const int EEPROM_AUDIO = 0;

int *active_pattern;        // pointer to the array containing the active array
int pattern_size;           // number of elements in the pattern

int curr_siren = SIREN_ONE;
int prev_siren = SIREN_OFF;
unsigned long prev_siren_change = 0;

int prev_pattern = PATTERN_OFF;
int curr_pattern = PATTERN_ONE;
unsigned long prev_pattern_change = 0;

boolean buzzer = false;
unsigned long last_buzzer_on = 0;
const int BUZZER_DURATION = 10;

// Blinking pattern
// { state (see above), duration in ms }
const int PATTERN0[] = {
  STATE_OFF, 500
};

const int PATTERN1[] = {
  // rapid red
  STATE_RED, 75,
  STATE_OFF, 15,
  STATE_RED, 75,
  STATE_OFF, 15,
  STATE_RED, 75,
  STATE_OFF, 15,
  STATE_RED, 75,
  STATE_OFF, 15,
  STATE_RED, 75,
  STATE_OFF, 15,

  // rapid blue
  STATE_BLUE, 75,
  STATE_OFF, 15,
  STATE_BLUE, 75,
  STATE_OFF, 15,
  STATE_BLUE, 75,
  STATE_OFF, 15,
  STATE_BLUE, 75,
  STATE_OFF, 15,
  STATE_BLUE, 75,
  STATE_OFF, 15,

  // rapid red
  STATE_RED, 75,
  STATE_OFF, 15,
  STATE_RED, 75,
  STATE_OFF, 15,
  STATE_RED, 75,
  STATE_OFF, 15,
  STATE_RED, 75,
  STATE_OFF, 15,
  STATE_RED, 75,
  STATE_OFF, 15,

  // rapid blue
  STATE_BLUE, 75,
  STATE_OFF, 15 ,
  STATE_BLUE, 75,
  STATE_OFF, 15,
  STATE_BLUE, 75,
  STATE_OFF, 15,
  STATE_BLUE, 75,
  STATE_OFF, 15,
  STATE_BLUE, 75,
  STATE_OFF, 15,

  // slow red-blue
  STATE_RED, 250,
  STATE_BLUE, 250,
  STATE_RED, 250,
  STATE_BLUE, 250,
  STATE_RED, 250,
  STATE_BLUE, 250,
  STATE_RED, 250,
  STATE_BLUE, 250,
  STATE_RED, 250,
  STATE_BLUE, 250,
  STATE_RED, 250,
  STATE_BLUE, 250,
  STATE_RED, 250,
  STATE_BLUE, 250,
  STATE_RED, 250,
  STATE_BLUE, 250,
  STATE_RED, 250,
  STATE_BLUE, 250,
  STATE_RED, 250,
  STATE_BLUE, 250,
  STATE_RED, 250,
  STATE_BLUE, 250,
  STATE_RED, 250,
  STATE_BLUE, 250,
  STATE_RED, 250,
  STATE_BLUE, 250
};

const int PATTERN2[] = {
  STATE_RED1, 250,
  STATE_BLUE1, 250,
  STATE_RED2, 250,
  STATE_BLUE2, 250,
  STATE_OFF, 150,
  STATE_RED1 | STATE_BLUE1, 250,
  STATE_OFF, 150,
  STATE_RED2 | STATE_BLUE2, 250,
  STATE_OFF, 150,
  STATE_RED2 | STATE_BLUE1, 250,
  STATE_OFF, 150,
  STATE_RED1 | STATE_BLUE2, 250,
  STATE_OFF, 150,
  STATE_RED | STATE_BLUE, 250,
  STATE_OFF, 150
};

const int PATTERN3[] = {
  STATE_RED1, 250,
  STATE_RED2, 250,
  STATE_BLUE1, 250,
  STATE_BLUE2, 250,
  STATE_BLUE1, 250,
  STATE_RED2, 250,
};

const int PATTERN4[] = {
  STATE_RED1 | STATE_BLUE2, 250,
  STATE_RED2 | STATE_BLUE1, 250
};

// basic settings for the siren
const int DEVIATION = 300;
const int BASE = 800;

// EEPROM variable to indicate audio lock
int audio;

int x = 0;
bool do_init = false;
unsigned long nextPatternChange = 0;
unsigned int currentPatternOffset = 0;


/*******************************************
 * START OF CODE
 *******************************************/

// change the blinking pattern
void change_pattern() {
  unsigned long now = millis();

  if (now - prev_pattern_change < 500) {
    return;
  }

  if (curr_pattern != prev_pattern) {
    tone(SIREN, 1500, 100);
    delay(100);

    prev_pattern = curr_pattern;
    curr_pattern = curr_pattern + 1;
    curr_pattern = curr_pattern % NUMPATTERNS;
    prev_pattern_change = now;

    switch (curr_pattern) {
    case PATTERN_OFF:
      active_pattern = (int *) PATTERN0;
      pattern_size = sizeof(PATTERN0) / (2*sizeof(int));
      break;

    case PATTERN_ONE:
      active_pattern = (int *) PATTERN1;
      pattern_size = sizeof(PATTERN1) / (2*sizeof(int));
      break;

    case PATTERN_TWO:
      active_pattern = (int *) PATTERN2;
      pattern_size = sizeof(PATTERN2) / (2*sizeof(int));
      break;

    case PATTERN_THREE:
      active_pattern = (int *) PATTERN3;
      pattern_size = sizeof(PATTERN3) / (2*sizeof(int));
      break;

    case PATTERN_FOUR:
      active_pattern = (int *) PATTERN4;
      pattern_size = sizeof(PATTERN4) / (2*sizeof(int));
      break;
    }

    currentPatternOffset = 0;
    nextPatternChange = now;
  }
}

void change_siren() {
  unsigned long now = millis();

  if (now - prev_siren_change < 500)
    return;
  if (curr_siren != prev_siren) {
    tone(SIREN, 1000, 100);
    delay(100);

    prev_siren = curr_siren;
    curr_siren = curr_siren + 1;
    curr_siren = curr_siren % NUMSIRENS;
    prev_siren_change = now;
  }
}

void siren() {
  float frequency;

  // play siren
  if (audio) {
    // reset the siren loop
    switch (curr_siren) {

      // typical US police siren
    case SIREN_ONE:
      if (x <= 666 ) // 2000/3
        frequency = (int) (BASE + 3.0 * DEVIATION / 2000 * x);
      else 
        frequency = (int) (BASE + DEVIATION - (x - 666) * 3.0 * DEVIATION / 4000 );

      x = (x + 1) % 2000;
      tone(SIREN, frequency);
      break;

      // clean sine wave
    case SIREN_TWO:
      frequency = (int) (BASE + DEVIATION*sin(2.0 * PI * x / 2000));
      x = (x + 1) % 2000;
      tone(SIREN, frequency);
      break;

      // two tone
    case SIREN_THREE:
      if (x < 2000) 
        frequency = 375;
      else
        frequency = 500;
      x = (x + 1) % 4000;
      tone(SIREN, frequency);
      break;

      // phaser siren
    case SIREN_FOUR:
      frequency = (int) (BASE + DEVIATION*sin(2.0 * PI * x / 250));
      x = (x + 1) % 250;
      tone(SIREN, frequency);
      break;       

      // old fashioned three-tone Dutch ambulance
    case SIREN_FIVE:
      if (x < 2500) 
        frequency = 392;
      else if (x < 5000)
        frequency = 659;
      else if (x < 7500)
        frequency = 523;
      else 
        frequency = 659;
      x = (x+1) % 10000;
      tone(SIREN, frequency);
      break;
    }
  }
}

void setup()
{
  int i;

  pinMode(RED1, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(BLUE1, OUTPUT);
  pinMode(BLUE2, OUTPUT);

  pinMode(SIREN, OUTPUT);
  pinMode(SIREN_TOGGLE, INPUT);
  pinMode(BUZZER_SWITCH, INPUT);
  pinMode(PATTERN_SWITCH, INPUT);
  pinMode(SIREN_SWITCH, INPUT);

  // check if audio lock is engaged. default to off 
  audio = EEPROM.read(EEPROM_AUDIO);
  if (audio == 255) {
    audio = 1;
  }

  // do we need to toggle the audio lock 
  if (digitalRead(BUZZER_SWITCH) == HIGH) {
    audio = audio ? 0 : 1;
    digitalWrite(RED1, HIGH);
    digitalWrite(BLUE1, HIGH);
    EEPROM.write(EEPROM_AUDIO, audio);
    delay(250);
    digitalWrite(RED1, LOW);
    digitalWrite(BLUE1, LOW);
  }

  prev_pattern = PATTERN_ONE;
  curr_pattern = PATTERN_OFF;
  active_pattern = (int *) PATTERN0;
  pattern_size = sizeof(PATTERN0) / (2 * sizeof(int));
  prev_siren = SIREN_ONE;
  curr_siren = SIREN_OFF;
  do_init = true;
}

void loop() {
  unsigned long now = millis();
  int ledState[maxLEDpin];
  int *p;

  // signal on successful startup or reset
  if (do_init) {
    do_init = false;
    for (int i=0; i<2; i++) {
      tone(SIREN, 750, 250);
      digitalWrite(RED1, HIGH);
      digitalWrite(BLUE1, HIGH);
      delay(250);
      digitalWrite(RED2, HIGH);
      digitalWrite(BLUE2, HIGH);
      tone(SIREN, 1250, 250);
      delay(250);
      noTone(SIREN);
      digitalWrite(RED1, LOW);
      digitalWrite(RED2, LOW);
      digitalWrite(BLUE1, LOW);
      digitalWrite(BLUE2, LOW);
    }
  }

  // light state change
  if (now > nextPatternChange) {
    p = (int *) &active_pattern[2*currentPatternOffset];
    nextPatternChange = now + p[1];

    ledState[RED1]  = LOW;
    ledState[RED2]  = LOW;
    ledState[BLUE1] = LOW;
    ledState[BLUE2] = LOW;

    if ((p[0] & STATE_RED) == STATE_RED)     { 
      ledState[RED1]  = HIGH; 
      ledState[RED2] = HIGH;
    };
    if ((p[0] & STATE_BLUE) == STATE_BLUE)   { 
      ledState[BLUE1] = HIGH; 
      ledState[BLUE2] = HIGH;
    };
    if ((p[0] & STATE_RED1) == STATE_RED1)   { 
      ledState[RED1]  = HIGH; 
    }
    if ((p[0] & STATE_RED2) == STATE_RED2)   { 
      ledState[RED2]  = HIGH; 
    }
    if ((p[0] & STATE_BLUE1) == STATE_BLUE1) { 
      ledState[BLUE1] = HIGH; 
    }
    if ((p[0] & STATE_BLUE2) == STATE_BLUE2) { 
      ledState[BLUE2] = HIGH; 
    }

    digitalWrite(RED1, ledState[RED1]);
    digitalWrite(RED2, ledState[RED2]);
    digitalWrite(BLUE1, ledState[BLUE1]);
    digitalWrite(BLUE2, ledState[BLUE2]);

    currentPatternOffset = currentPatternOffset + 1;
    currentPatternOffset = currentPatternOffset % pattern_size;
  }

  // do we need to switch the light pattern?
  if (digitalRead(PATTERN_SWITCH) == HIGH) {
    change_pattern();
  }

  // do we need to switch the siren?
  if (digitalRead(SIREN_SWITCH) == HIGH) {
    change_siren();
  }

  // is audio enabled?
  if (audio) {
    if (digitalRead(BUZZER_SWITCH) == HIGH)  {
      tone(SIREN, 200, 10);
    } 
    else {
      // do we need to switch the light pattern?
      if (digitalRead(SIREN_TOGGLE) == HIGH) {
        siren();
      } 
      else
        noTone(SIREN);
    }
  }
}

// EOF
