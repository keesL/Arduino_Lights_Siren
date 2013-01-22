const int numRed = 2;       // number of red LEDs
const int numBlue = 2;      // number of blue LEDs
const int RED[] = {2, 4};   // pins with red LEDs
const int BLUE[] = {7, 8};  // pins with blue LEDs

const int AUDIO=3;          // audio output pin

const int STATE_OFF       = 1;
const int STATE_RED       = 2;
const int STATE_BLUE      = 4;


const int PATTERN[][2] = {
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
const int PATTERNSIZE=sizeof(PATTERN)/(2*sizeof(int));

unsigned long nextPatternChange = 0;
unsigned int currentPatternOffset = 0;

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

  tone(AUDIO, 1500, 1000);
}

void red(int state) {
  for (int i=0; i<numRed; i++) digitalWrite(RED[i], state);  
}

void blue(int state) {
  for (int i=0; i<numBlue; i++) digitalWrite(BLUE[i], state);
}

void loop() {
  unsigned long now = millis();
  int p[2];
  if (now > nextPatternChange) {
    currentPatternOffset = currentPatternOffset + 1;
    currentPatternOffset = currentPatternOffset % PATTERNSIZE;

    p[0] = PATTERN[currentPatternOffset][0];
    p[1] = PATTERN[currentPatternOffset][1];
    nextPatternChange = now + p[1];

    int r = LOW;
    int b = LOW;

    if ((p[0] & STATE_RED) == STATE_RED)       r = HIGH;
    if ((p[0] & STATE_BLUE) == STATE_BLUE)     b = HIGH;    
    
    blue(b);
    red(r);
  }    
}

