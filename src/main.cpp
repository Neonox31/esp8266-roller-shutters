#include <Homie.h>

#define FW_NAME "rollers-firmware"
#define FW_VERSION "1.0.0"

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

const int PIN_RELAY_1 = D1;
const int PIN_RELAY_2 = D2;
const int CONTACT_TIMEOUT = 2000;

HomieNode cmvNode("rollers", "switch");

void idle() {
  digitalWrite(PIN_RELAY_1, LOW);
  digitalWrite(PIN_RELAY_2, LOW);
  Serial.println("Roller shutters are idle");
}

void open() {
  digitalWrite(PIN_RELAY_1, HIGH);
  digitalWrite(PIN_RELAY_2, LOW);
  Homie.setNodeProperty(cmvNode, "state", "open");
  Serial.println("Roller shutters are opened");
}

void close() {
  digitalWrite(PIN_RELAY_1, LOW);
  digitalWrite(PIN_RELAY_2, HIGH);
  Homie.setNodeProperty(cmvNode, "state", "close");
  Serial.println("Roller shutters are closed");
}

bool cmvStateHandler(String value) {
  if (value == "open") {
    open();
    delay(CONTACT_TIMEOUT);
    idle();
  } else if (value == "close") {
    close();
    delay(CONTACT_TIMEOUT);
    idle();
  } else {
    return false;
  }

  return true;
}

void setup() {
  // Set to normally closed by default
  pinMode(PIN_RELAY_1, OUTPUT);
  digitalWrite(PIN_RELAY_1, LOW);
  pinMode(PIN_RELAY_2, OUTPUT);
  digitalWrite(PIN_RELAY_2, LOW);

  Homie.setFirmware(FW_NAME, FW_VERSION);
  cmvNode.subscribe("state", cmvStateHandler);
  Homie.registerNode(cmvNode);
  Homie.setup();
}

void loop() {
  Homie.loop();
}
