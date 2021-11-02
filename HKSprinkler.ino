#include "HomeSpan.h"
#include "DEV_Valve.h"

struct DEV_Valve *valve;

void setup() {
  Serial.begin(115200);
  Serial.println("Setup");

  /* Setup our Home Span basics */
  homeSpan.setMaxConnections(12);
  homeSpan.begin(Category::Sprinklers, "HomeSpan Sprinkler");

  /* Setup the accessory and required characteristics */
  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Name("Sprinklers");
  new Characteristic::Manufacturer("HomeSpan");
  new Characteristic::SerialNumber("000-AAA");
  new Characteristic::Model("01A");
  new Characteristic::FirmwareRevision("0.1");
  new Characteristic::Identify();

  new Service::HAPProtocolInformation();
  new Characteristic::Version("1.0.0");

  new DEV_Valve(10, 1, "Front");
  new DEV_Valve(11, 1, "Side");
  new DEV_Valve(12, 1, "Beds");

  Serial.println("Done");
}

void loop() {
  homeSpan.poll();
}
