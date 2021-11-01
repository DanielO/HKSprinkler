struct DEV_Valve : Service::Valve {
  int pin;
  const char *nameptr;
  unsigned long lastMillis;
  unsigned long remMillis;
  unsigned long time;
  unsigned long startedAt;
  unsigned long stopAt;
  bool on;
  SpanCharacteristic *active;
  SpanCharacteristic *inUse;
  SpanCharacteristic *valveType;
  SpanCharacteristic *setDuration;
  SpanCharacteristic *name;

 DEV_Valve(int valvePin, int _valveType, const char *_name) : Service::Valve() {
    /* Create characteristics */
    active = new Characteristic::Active();
    inUse = new Characteristic::InUse();
    valveType = new Characteristic::ValveType(_valveType);
    setDuration = new Characteristic::SetDuration(2400, true); // default 40 minutes, save to NV
    setDuration->setRange(60, 3600, 60); // 1 minute to 1 hour
    remDuration = new Characteristic::RemainingDuration();
    name = new Characteristic::Name(_name);

    /* Init other variables */
    nameptr = _name;
    pin = valvePin;

    /* Init time keeping */
    lastMillis = millis();
    time = 1;
    remMillis = 0;

    // Setup IO
    on = false;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  void turnOn(void) {
    int duration;

    duration = setDuration->getVal();
    startedAt = time;
    stopAt = time + duration;
    active->setVal(true);
    inUse->setVal(true);
    remDuration->setVal(duration);
    on = true;
    Serial.print(nameptr);
    Serial.print(": Turning on for ");
    Serial.print(duration);
    Serial.print(" stop at ");
    Serial.println(stopAt);
  }

  void turnOff(void) {
    startedAt = stopAt = 0;
    on = false;
    Serial.print(nameptr);
    Serial.println(": Turning off");
    inUse->setVal(false);
    active->setVal(false);
  }

  boolean update(void) {
    Serial.print(nameptr);
    Serial.print(": update, active: ");
    Serial.print(active->getNewVal());
    Serial.print(" inUse: ");
    Serial.print(inUse->getNewVal());
    Serial.print(" valveType: ");
    Serial.print(valveType->getNewVal());
    Serial.print(" setDuration: ");
    Serial.println(setDuration->getNewVal());

    if (active->getNewVal() && !on)
      turnOn();

    if (!active->getNewVal() && on)
      turnOff();

    return(true);
  }

  void updateTime(void) {
    unsigned long m, delta, secs;

    /* Number of milliseconds since last loop */
    m = millis();
    delta = m - lastMillis;
    lastMillis = m;

    /* Handle wrap */
    if (delta > 0x8000000)
      delta -= 0xffffffff;

    /* Add on any remaining from last time */
    delta += remMillis;

    /* Number of seconds */
    secs = delta / 1000;

    /* Store remaining msec for next time */
    remMillis = delta - (secs * 1000);

    /* Update second counter */
    time += secs;
  }

  void loop(void) {
    updateTime();

    if (stopAt != 0 && time >= stopAt)
      turnOff();

    digitalWrite(pin, on);
  }
};
