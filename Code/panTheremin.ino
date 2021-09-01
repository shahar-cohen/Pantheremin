#include "MIDIUSB.h"
#include <Average.h>
#include <Array.h>

// CHANGE MANUAsLLY!!!
const int NUM_SENSORS = 3;
const int MAX_LOOPED_NOTES_PER_SENSOR = 5;
const int SENSOR_PIN[NUM_SENSORS] = {A0, A1, A2};
Average<float> avgReading[NUM_SENSORS] = {Average<float> (10), Average<float>(10), Average<float> (10)};
const int KNOCK_SENSOR_PIN = A8;
const int TIMEOUT = 300; //* Amount of time the potentiometer will be read after it exceeds the varThreshold[i]
const int varThreshold = 3; //* Threshold for the potentiometer signal variation

int counter[NUM_SENSORS];

typedef Array<int,MAX_LOOPED_NOTES_PER_SENSOR> sensorLoopedNotes;

//IR sensors
int sensorAvg[NUM_SENSORS];
int sensorPrevAvg[NUM_SENSORS];
boolean sensorValChanged[NUM_SENSORS]; // If the potentiometer is moving
int minSensorReading[NUM_SENSORS]; // the min reading you want the IR to have
int maxSensorReading[NUM_SENSORS];
const int MIN_READING = 200;  // used when all sensors are of the same sensitivity, consider changing these two
const int MAX_READING = 710;
const float SCALE_CURVE = pow(10, -0.1);

// Piezo knock sensor
const int KNOCK_THRESH = 10;
bool didSensorReturnToZero = true;

// Looped notes
sensorLoopedNotes loopedNotes[NUM_SENSORS];
int loopedNoteIdx[NUM_SENSORS];
int simpleLoop[NUM_SENSORS];

boolean notesAreLooping = false;
/* for each sensor there are maximum MAX_LOOPED_NOTES_PER_SENSOR looping at the same time. loopedNoteIdx[i] is the index into which the next 
looped note of sensor i would be inserted to the above array. */


//timing
unsigned long prevTime[NUM_SENSORS]; // Previously stored time
unsigned long timer[NUM_SENSORS]; // Stores the time that has elapsed since the timer was reset

//debounce
unsigned long lastDebounceTime[NUM_SENSORS];  // the last time the output pin was toggled
const unsigned long debounceDelay = 20;    //* the debounce time; increase if the output flickers

//notes
const byte NUM_NOTES = 8; // number of notes in the scales
const int MIN_NOTE_IDX = 0; // min and max index of the pentatonic scale below
const int MAX_NOTE_IDX = NUM_NOTES-1;
boolean notesArePlaying = false;
byte noteIdx[NUM_SENSORS];
byte notePrevIdx[NUM_SENSORS];
boolean note_is_playing[NUM_SENSORS];
byte noteOut[NUM_SENSORS];
byte prevNoteOut[NUM_SENSORS];
const int pentaNote[NUM_NOTES] = { -2, 0, 3, 5, 7, 10, 12, 15};//, 17, 19, 22, 24, 27, 29, 31, 34};   //minor pentatonic scale

/////////////////////////////////////////////
// MIDI STUFF
int MIDI_CH[NUM_SENSORS]; // MIDI channel for each sensor
int MIDI_LOOP_CH[NUM_SENSORS]; // looping channel for each sensor
const byte BASE_NOTE = 48; //* Lowest NOTE to be used - The root
const byte ALL_SOUNDS_OFF_CC = 120;  //turns all sounds off, including sustain pedal and tails
const byte ALL_NOTES_OFF_CC = 123;  //turns all notes off, leaving sustain and tails
const byte HOLD_NOTE_CC = 64;
const byte RELEASE_NOTE_CC = 63;
const byte CC = 7; //* cc to be used in the left sensor
