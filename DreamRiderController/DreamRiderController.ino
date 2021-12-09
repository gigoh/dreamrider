/*
  DreamRiderController

  This code creates a BLE peripheral with the standard battery service and
  level characteristic. The A0 pin is used to calculate the battery level.

  The circuit:
  - Arduino MKR WiFi 1010

  You can use DreamRider app to interact with the services and
  characteristics created in this sketch.
*/

#include <ArduinoBLE.h>

#define SPOKES 14
#define DIAMETER 0.622
#define PULSE_MIN_INTERVAL 32L
#define PULSE_TIMEOUT 1000L
#define REPORT_INTERVAL 16L

const byte steeringPin = A0;
const byte speedPin = A1;

 // BLE Battery Service
BLEService batteryService("180F");

// BLE Battery Level Characteristic
BLEShortCharacteristic batteryLevelChar("2A19",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes
BLEFloatCharacteristic speedFloat("2A20",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

long previousMillis = 0;  // last time the battery level was checked, in ms

float currentSpeed = 0;
long last = 0;
bool noInterrupt = false;

void setup() {
  Serial.begin(115200);    // initialize serial communication
//  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected
  pinMode(speedPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(speedPin), calculateSpeed, FALLING);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  /* Set a local name for the BLE device
     This name will appear in advertising packets
     and can be used by remote devices to identify this BLE device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  BLE.setLocalName("DreamRider");
  BLE.setAdvertisedService(batteryService); // add the service UUID
  batteryService.addCharacteristic(batteryLevelChar); // add the battery level characteristic
  batteryService.addCharacteristic(speedFloat);
  BLE.addService(batteryService); // Add the battery service
  batteryLevelChar.writeValue(0); // set initial value for this characteristic
  speedFloat.writeValue(0);

  /* Start advertising BLE.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);

    // reports the values every REPORT_INTERVAL ms
    // while the central is connected:
    while (central.connected()) {
      long currentMillis = millis();
      if (currentMillis - last > PULSE_TIMEOUT) {
        last = currentMillis;
        currentSpeed = 0;
      }
      if (currentMillis - previousMillis >= REPORT_INTERVAL) {
        previousMillis = currentMillis;
        updateSpeed();
        updateBatteryLevel();
      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void updateBatteryLevel() {
  /* Read the current voltage level on the A0 analog input pin.
     This is used here to simulate the charge level of a battery.
  */
  static int previousBatteryLevel = 0;
  int battery = analogRead(A0);
  int batteryLevel = map(battery, 0, 1023, -512, 511);
  if(previousBatteryLevel != batteryLevel) {
    previousBatteryLevel = batteryLevel;
    batteryLevelChar.writeValue(batteryLevel);  // and update the battery level characteristic  
  }
}

void updateSpeed() {
  static float previousSpeed = 0;
  if(previousSpeed != currentSpeed) {
    previousSpeed = currentSpeed;
    speedFloat.writeValue(currentSpeed);
  }
}

void calculateSpeed() {
  long now = millis();
  long pulseLength = now - last;
  if (pulseLength < PULSE_MIN_INTERVAL || pulseLength > PULSE_TIMEOUT) {
    return;
  }
  currentSpeed = (DIAMETER * PI / SPOKES) * 1000.0 / pulseLength;
  last = now;
}
