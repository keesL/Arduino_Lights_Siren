// Simple lightbar and Piezo police siren
// (C) 2013 by Kees Leune <kees@leune.org>
// https://github.com/keesL/Arduino_Lights_Siren

// pin definitions
const int RED[] = {2, 4};   // pins with red LEDs
const int BLUE[] = {7, 8};  // pins with blue LEDs
const int AUDIO = 3;          // audio output pin

// LED states
const int STATE_OFF       = 1;
const int STATE_RED       = 2;
const int STATE_BLUE      = 4;

// Blinking pattern
// { state (see above), duration in ms }
const int PATTERN[] = {
  // slow red-blue
  STATE_RED, 250 ,
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
  STATE_BLUE, 250,
  
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
};

// some helper variables
const int PATTERNSIZE=sizeof(PATTERN) / sizeof(int);
const int numRed = sizeof(RED) / sizeof(int);
const int numBlue = sizeof(BLUE) / sizeof(int);

unsigned long nextPatternChange = 0;
unsigned int currentPatternOffset = 0;

// change all red LEDs to state 
void red(int state) {
  for (int i=0; i<numRed; i++) digitalWrite(RED[i], state);  
}

// change all blue LEDs to state
void blue(int state) {
  for (int i=0; i<numBlue; i++) digitalWrite(BLUE[i], state);
}

void setup()
{
  int i;
  for (i=0; i<numRed; i++) {
    pinMode(RED[i], OUTPUT);
  }
  for (i=0; i<numBlue; i++) {
    pinMode(BLUE[i], OUTPUT);
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // tone(AUDIO, 1500, 1000);
}

void loop() {
  unsigned long now = millis();
  int p[2];
  if (now > nextPatternChange) {
    currentPatternOffset = currentPatternOffset + 2;
    currentPatternOffset = currentPatternOffset % PATTERNSIZE;

    p[0] = PATTERN[currentPatternOffset];
    p[1] = PATTERN[currentPatternOffset+1];
    nextPatternChange = now + p[1];

    int r = LOW;
    int b = LOW;

    if ((p[0] & STATE_RED) == STATE_RED)       r = HIGH;
    if ((p[0] & STATE_BLUE) == STATE_BLUE)     b = HIGH;    
    
    blue(b);
    red(r);
  }    
}

