Project: Arduino Lights and Sirens
Author : Kees Leune <kees@leune.org>
Web    : http://techtoolshed.blogspot.com
Git    : https://github.com/keesL/Arduino_Lights_Siren/tree/master
Date   : January, 2013
Status : In progress

Project description
-------------------
This project sets out to build a simple toy for a 4yo boy whose
life revolves around fire trucks, sirens and flashing lights.

I'm going to build a simple project box that will have a bunch 
of switches

1) (toggle) master power
2) (toggle) turn on flashing lights
3) (momentary) play siren
4) (monentary) play buzzer

The flashing lights will be red and blue LEDs that flash in a 
pattern that resembles a police/fire truck light bar. The siren
will be a simple one; a triangular wave that rises to its peak
at about 1/3 of the cycle and then resettles back to base in
the remaining 2/3 of the cycle. 

The buzzer will be stock buzzer.



The whole thing will be prototyped on an Arduino Uno, and then
moved into production on a standalone perf board using a separate
ATmega328-PU microcontroller powered by a 9V battery (ran through
a 7805 voltage regulator.



Total cost of the project: endless hours of tinkering, days of 
being annoyed by the sounds of the siren and buzzer when completed
and about $15 in parts.