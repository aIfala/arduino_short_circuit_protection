// circuit: https://falstad.com/circuit/circuitjs.html?ctz=CQAgjCAMB0l3BWcMBMcUHYMGZIA4UA2ATmIxAUgoqoQFMBaMMAKACcRs8AWEbuTjz6FeVMJHaDeKflM6RRnXCzCEUc3Ly5bCeEOoAmdAGYBDAK4AbAC4NLdA+CjOYkVsaWENQ7Z3VhoJFYAczluETC8KmiWAHcwgW5VPgEJACU5GSpfMGJ1aJTOaGwXKECWAAc+ZP4qbiiU7NLxeHgWUPq6xIakrxiAGU9MjHVfMRAzSwBnOmoJeN8svnr9WQkp5b0l7hXsXWcISZm4zc593z29CQAPfTAkfj1sMBKkiF4wdUsAe1iAZQAlkYAMLfAB21jY30sAAUAWCWLduGQUsQKHlquoPuoAG7Q6ymYJ0NJ0UxGNhwhFAA

#define lowSideControlPin 22
#define voltageReaderPin 15
#define voltageThrehold 10
#define protectUntilRetryTime 5000
unsigned int adcRead;
unsigned int samplingDuration;
unsigned long lowSideDisableTime = 0;
bool lowSideState = true;
void setup() {
  Serial.begin(115200);
  pinMode(lowSideControlPin, OUTPUT);
  digitalWrite(lowSideControlPin, true);
  //analogSetCycles(1); // reduce adc read time
}

// adc resolution is 12 bits, since max voltage = 3.3V, each step is 3.3/(2^12)=0.0008V
void loop() {
  samplingDuration = micros();
  adcRead = analogRead(voltageReaderPin);
  samplingDuration = micros() - samplingDuration;
  if (adcRead > voltageThrehold) {
    Serial.println("Short detected");
    lowSideDisableTime = millis() + protectUntilRetryTime;
    Serial.print("Sampling duration: "); 
    Serial.print(samplingDuration);
    Serial.println("microsecond");
    Serial.print(" Read Value: ");
    Serial.println(adcRead);
  }
  if (millis() < lowSideDisableTime) {
    if (lowSideState != false) {
      lowSideState = false;
      digitalWrite(lowSideControlPin, false);
    }
  } else {
    if (lowSideState != true) {
      lowSideState = true;
      digitalWrite(lowSideControlPin, true);
    }
  }
}
