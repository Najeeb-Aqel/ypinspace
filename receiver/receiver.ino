#include <VirtualWire.h>

const int dataPin=9;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
vw_setup(2000);
vw_set_rx_pin(dataPin);
vw_rx_start();

}

void loop() {
  // put your main code here, to run repeatedly:

  char buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen=VW_MAX_MESSAGE_LEN;
  //Serial.println("scanning");

  if (vw_get_message((uint8_t*) buf,&buflen)){
    int i;
    Serial.print("Received: ");
    for(i=0;i<buflen;i++){
      Serial.print(char(buf[i]));
      
    }
    Serial.println('\r');
  }

}
