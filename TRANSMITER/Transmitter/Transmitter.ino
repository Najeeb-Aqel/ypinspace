#include <VirtualWire.h>
#include <SoftwareSerial.h>

const int dataPin = 12;

void setup() {
  vw_setup(2000);
  vw_set_tx_pin(dataPin);
  Serial.begin(9600);
}

void loop() {
char data[6] = {'0','0','0','1','1','1'};

vw_send((uint8_t *) data, sizeof(data));
vw_wait_tx();
Serial.println("packet sent \n");

delay(200);

}
