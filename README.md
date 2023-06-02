##  Proximity based MIDI musical instrument

An arduino-based musical instrument with live generated parametric visuals.

https://github.com/shahar-cohen/Pantheremin/assets/39126169/9d450cb4-aabc-45f0-8d99-5710001c22b8

### Hardware
-Arduino Leonardo
-3 IR proximity sensors
-Piezoelectric vibration sensor

### Design

Parametric pattern design, created using Grasshopper on Rhino. The algorithmically generated triangle pattern creates a spacey feeling fitting the playing experience we wanted to convey.

### Communications

Using arduino, we converted the signal from the proximity IR sensors into MIDI messages. Those were channelled to Ableton Live for producing actual sounds, designed by us from available plugins.
A dedicated script converts incoming MIDI messages to OSC messages which are received by the Grasshopper add-on to Rhino. These software generate the visualization on the screen, and change the speed and color of the waves according to the sounds.
