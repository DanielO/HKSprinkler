#include "HomeSpan.h"
#include "DEV_Valve.h"

struct DEV_Valve *valve;

void setup() {
  Serial.begin(115200);
  Serial.println("Setup");

  /* Setup our Home Span basics */
  homeSpan.setStatusPin(2);
  homeSpan.setControlPin(0);
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

  /* Create valve controls */
  new DEV_Valve(35, 1, "Pin 35");
  new DEV_Valve(32, 1, "Pin 32");
  new DEV_Valve(33, 1, "Pin 33");
  new DEV_Valve(25, 1, "Pin 25");

  Serial.println("Done");
}

void loop() {
  homeSpan.poll();
}
