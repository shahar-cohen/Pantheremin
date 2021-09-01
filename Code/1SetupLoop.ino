

void setup() {
  Serial.begin(9600);
  for(int i=0; i< NUM_SENSORS; i++){
    sensorValChanged[i] = true;
    MIDI_CH[i] = i;
    MIDI_LOOP_CH[i] =  NUM_SENSORS + i; // if there are 3 channels, MIDI_CH = {1,2,3} and MIDI_LOOP_CH = {4,5,6}
    minSensorReading[i] = MIN_READING; // the min reading you want the IR to have
    maxSensorReading[i] = MAX_READING;
  }
}


void loop() {
  for(int i=0; i < NUM_SENSORS; i++){
    readIRSensor(i); // read sensor i
    irSensorHandler(i); // process reading of sensor i
  }
  knockSensorHandler();
  delay(20);
}
