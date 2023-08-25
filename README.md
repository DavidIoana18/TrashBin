# TrashBin
  The project is composed of 2 circuits, one consisting of an Arduino Mega 2560 board and its compatible components, and the other consisting of industrial components. The connection between these 2 circuits is made by relays.

When you approach the trash, the ultrasonic sensor detects movement, and the first hatch is opened with the help of the servomotor. Depending on which waste is inserted, the second hatch is rotated 90° to the right or to the left by the second servomotor, if the waste is metal or plastic, respectively.

Waste is detected using capacitive and inductive industrial sensors. The trash container has 2 compartments, each equipped with an ultrasonic sensor. It signals when the compartment is full, at which point the first bin hatch will no longer open.

The Blynk IoT application was used to display the level and amount of waste in the two compartments.
