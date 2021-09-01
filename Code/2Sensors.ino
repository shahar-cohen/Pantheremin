void knockSensorHandler(){
  int i, knockRead;
  notesArePlaying = false;
  knockRead = map(analogRead(KNOCK_SENSOR_PIN), 1, 1023, 1, 127);
  if((knockRead > KNOCK_THRESH) && didSensorReturnToZero){
    didSensorReturnToZero = false;
    noteOn(7, 85, knockRead);
    MidiUSB.flush();
    delay(1);
    for (i = 0; i < NUM_SENSORS; i++){
      if(note_is_playing[i]){
        notesArePlaying = true;
        if(simpleLoop[i] > 0){
          noteOn(MIDI_LOOP_CH[i], simpleLoop[i], 0);
          MidiUSB.flush();
        }
        delay(1);
        simpleLoop[i] = noteOut[i];
        noteOn(MIDI_LOOP_CH[i], simpleLoop[i], 127);
        MidiUSB.flush();
        delay(1);
      }
    }
    if (!notesArePlaying){
      for (i = 0; i < NUM_SENSORS; i++){
        if(simpleLoop[i] > 0){
          delay(1);
          noteOn(MIDI_LOOP_CH[i], simpleLoop[i], 0);
          MidiUSB.flush();
          delay(1);
          controlChange(MIDI_LOOP_CH[i], 123, 0);
          MidiUSB.flush();
          //controlChange(MIDI_LOOP_CH[i], 120, 0);
          simpleLoop[i] = 0;
        }
      }
    }
  }
  
  if (knockRead <= 2){
    didSensorReturnToZero = true;
  }
}

//read ir value
void readIRSensor(int i) {
  int reading = analogRead(SENSOR_PIN[i]);
  avgReading[i].push(reading);

  sensorAvg[i] = avgReading[i].mean();

  noteIdx[i] = scale(minSensorReading[i], maxSensorReading[i], MIN_NOTE_IDX, MAX_NOTE_IDX, sensorAvg[i]);

  if (abs(sensorAvg[i] - sensorPrevAvg[i]) > varThreshold) {
    prevTime[i] = millis();
  }
  
  timer[i] = millis() - prevTime[i];

  if (timer[i] < TIMEOUT) { 
    sensorValChanged[i] = true;
  }
  else {
    sensorValChanged[i] = false;
  }
}

void irSensorHandler(int i) {
  if (sensorValChanged[i] == true) {
    if (noteIdx[i] != notePrevIdx[i]) {
      if (noteIdx[i] >= 0) {
        lastDebounceTime[i] = millis();

        noteOut[i] = BASE_NOTE + pentaNote[noteIdx[i]];
        
        if (noteOut[i] != prevNoteOut[i]){
          noteOn(MIDI_CH[i], noteOut[i], noteOut[i]);
          MidiUSB.flush();
          note_is_playing[i] = true;
          delay(1);            
          noteOn(MIDI_CH[i], prevNoteOut[i], 0);
          MidiUSB.flush();
          prevNoteOut[i] = noteOut[i];
        }
      }
    }
    if (sensorAvg[i] < minSensorReading[i]) {
      if (note_is_playing[i] == true) {
        noteOn(MIDI_CH[i], noteOut[i], 0);
        MidiUSB.flush();
        delay(1);            
        controlChange(MIDI_CH[i], 123, 0);
        MidiUSB.flush();
        note_is_playing[i] = false;
      }
    }
    notePrevIdx[i] = noteIdx[i];
    sensorPrevAvg[i] = sensorAvg[i];
  }
}
