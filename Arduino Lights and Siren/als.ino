// Simple lights and siren for Arduino
// (C) 2013 Kees Leune <kees@leune.org>
// All rights reserved.

// label the pins we use for easier reference
const int RED[] = {2, 4};
const int BLUE[] = {7, 8};
const int AUDIO = 3;          

// constants for easier code
const int numRed = sizeof(RED)/sizeof(int);
const int numBlue = sizeof(BLUE)/sizeof(int);	

// light states. Can be combined as bit flag
const int STATE_OFF       = 1;
const int STATE_RED       = 2;
const int STATE_BLUE      = 4;

// light pattern
const int LIGHT_PATTERN[][2] = {
  {    STATE_RED, 250      }  ,
  {    STATE_BLUE, 250     }  ,
  {    STATE_RED, 250      }  ,
  {    STATE_BLUE, 250      }  ,
  {    STATE_RED, 250      }  ,
  {    STATE_BLUE, 250      }  ,
  {    STATE_RED, 250      }  ,
  {    STATE_BLUE, 250      }  ,
  {    STATE_RED, 250      },
  {    STATE_BLUE, 250      }  ,
  {    STATE_RED, 250      }  ,
  {    STATE_BLUE, 250      }  ,
  {    STATE_RED, 250      }  ,
  {    STATE_BLUE, 250      }  ,
  {    STATE_RED, 250      }  ,
  {    STATE_BLUE, 250      }  ,
  {    STATE_RED, 250      }  ,
  {    STATE_BLUE, 250      }  ,
  {    STATE_RED, 250      }  ,
  {    STATE_BLUE, 250      }  ,
  {    STATE_RED, 250      }  ,
  {    STATE_BLUE, 250     },
  {    STATE_RED, 250 },
  {    STATE_BLUE, 250     },
  {    STATE_RED, 250 },
  {    STATE_BLUE, 250     },
  {    STATE_RED, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_RED, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_RED, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_RED, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_RED, 75},
  {    STATE_OFF, 15},
  {    STATE_BLUE, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_BLUE, 75},
  {    STATE_OFF, 15 },
  {    STATE_BLUE, 75},
  {    STATE_OFF, 15 },
  {    STATE_BLUE, 75},
  {    STATE_OFF, 15},
  {    STATE_BLUE, 75},
  {    STATE_OFF, 15},
  {    STATE_RED, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_RED, 75      }  ,
  {    STATE_RED, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_RED, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_RED, 75},
  {    STATE_OFF, 15},
  {    STATE_BLUE, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_BLUE, 75},
  {    STATE_OFF, 15 },
  {    STATE_BLUE, 75},
  {    STATE_OFF, 15 },
  {    STATE_BLUE, 75},
  {    STATE_OFF, 15},
  {    STATE_BLUE, 75},
  {    STATE_OFF, 15},
  {    STATE_RED, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_RED, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_RED, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_RED, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_RED, 75},
  {    STATE_OFF, 15},
  {    STATE_BLUE, 75      }  ,
  {    STATE_OFF, 15 },
  {    STATE_BLUE, 75},
  {    STATE_OFF, 15 },
  {    STATE_BLUE, 75},
  {    STATE_OFF, 15 },
  {    STATE_BLUE, 75},
  {    STATE_OFF, 15},
  {    STATE_BLUE, 75},
};
const int LIGHT_PATTERN_SIZE=sizeof(LIGHT_PATTERN)/(2*sizeof(int));

unsigned long nextLightPatternChange = 0;
unsigned int currentLightPatternOffset = 0;

// set up the board at power up or reset
void setup()
{
  int i;
  for (i=0; i<numRed; i++) {
    pinMode(RED[i], OUTPUT);
  }
  for (i=0; i<numBlue; i++) {
    pinMode(BLUE[i], OUTPUT);
  }
  // for some reason, pin 13 on the board annoys me. Turn it off
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void all_red(int state) {
  for (int i=0; i<numRed; i++) digitalWrite(RED[i], state);  
}

void all_blue(int state) {
  for (int i=0; i<numBlue; i++) digitalWrite(BLUE[i], state);
}

void loop() {
  unsigned long now = millis();
  int p[2];
  if (now > nextLightPatternChange) {
    currentLightPatternOffset = currentLightPatternOffset + 1;
    currentLightPatternOffset = currentLightPatternOffset % PATTERNSIZE;

    p[0] = LIGHT_PATTERN[currentLightPatternOffset][0];
    p[1] = LIGHT_PATTERN[currentLightPatternOffset][1];
    nextLightPatternChange = now + p[1];

    int r = LOW;
    int b = LOW;

    if ((p[0] & STATE_RED) == STATE_RED)   r = HIGH;
    if ((p[0] & STATE_BLUE) == STATE_BLUE) b = HIGH;    
    
    all_blue(b);
    all_red(r);
  }    
}