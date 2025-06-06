#include <EEPROM.h>
#include <LiquidCrystal.h>

/* ---------- LCD pins (same as your Proteus drawing) --- */
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

/* ---------- I/O pins ---------------------------------- */
constexpr byte RELAY_PIN     = 8;       // cuts mains
constexpr byte PULSE_PIN     = 9;       // 3200-imp/kWh LED, active-LOW
constexpr byte LOAD_LED_PIN  = 10;      // serial 'A/B'
constexpr byte LOAD_FAN_PIN  = 11;      // serial 'C/D'

constexpr byte VOLTAGE_PIN   = A0;      // divider  ~0-4.5 V
constexpr byte CURRENT_PIN   = A1;      // ACS712   ~0-5 V

/* ---------- Wi-Fi & Adafruit IO ----------------------- */
#include <AdafruitIO_WiFi.h>

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *feedPower   = io.feed("power");
AdafruitIO_Feed *feedUnits   = io.feed("units");
AdafruitIO_Feed *feedBalance = io.feed("balance");

/* ---------- Meter / tariff constants ----------------- */
constexpr float ADC_mV_STEP        = 5000.0 / 1023.0;   // mV per LSB
constexpr float VOLTAGE_GAIN       = 60.0 * 4.5 / 1023.0;
constexpr float CURR_ZERO_mV       = 2500.0;
constexpr float CURR_SENS_mV_A     = 185.0;

constexpr uint8_t EEPROM_BAL_ADDR  = 1;
constexpr uint8_t DEFAULT_RUPEES   = 40;  // 8 units  (₹5 each)
constexpr float RS_PER_UNIT        = 5.0;
constexpr uint8_t LOW_BALANCE_RS   = 15;

float vRms = 0, iRms = 0, pInst = 0, energyWh = 0;

/* ---------- Timing ----------------------------------- */
constexpr unsigned long SAMPLE_MS      = 1000;
constexpr unsigned long ADAFRUIT_MS    = 30000;
unsigned long tLastSample   = 0;
unsigned long tLastPublish  = 0;

bool lastPulse = HIGH;   // for edge detect

/* ===================================================== */
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LOAD_LED_PIN, OUTPUT);
  pinMode(LOAD_FAN_PIN, OUTPUT);
  pinMode(PULSE_PIN, INPUT_PULLUP);

  // init balance once
  if (EEPROM.read(EEPROM_BAL_ADDR) == 0xFF)
    EEPROM.write(EEPROM_BAL_ADDR, DEFAULT_RUPEES);

  lcd.print(F("Connecting WiFi"));
  io.connect();                    // blocks until connected
  lcd.setCursor(0, 1); lcd.print(F("Adafruit IO OK"));

  delay(1500);
  showBalance();
  tLastSample  = millis();
  tLastPublish = millis();
}

/* ===================================================== */
void loop() {
  io.run();   // keep MQTT link alive  (non-blocking)

  /* ------------ Serial load control ------------------ */
  if (Serial.available()) {
    switch (Serial.read()) {
      case 'A': digitalWrite(LOAD_LED_PIN, HIGH);  break;
      case 'B': digitalWrite(LOAD_LED_PIN, LOW);   break;
      case 'C': digitalWrite(LOAD_FAN_PIN, HIGH);  break;
      case 'D': digitalWrite(LOAD_FAN_PIN, LOW);   break;
    }
  }

  /* ------------ 1 s acquisition ---------------------- */
  if (millis() - tLastSample >= SAMPLE_MS) {
    tLastSample += SAMPLE_MS;
    acquireVI();                 // updates vRms, iRms, pInst
    energyWh += pInst * (SAMPLE_MS / 3600000.0);

    lcd.setCursor(0, 0);
    lcd.print(F("P:"));
    lcd.print(pInst, 0);
    lcd.print(F("W "));
    lcd.print(F("E:"));
    lcd.print(energyWh, 1);
    lcd.print(F("Wh "));

    handlePulse();
    checkBalance();
  }

  /* ------------ Adafruit IO publish ------------------ */
  if (millis() - tLastPublish >= ADAFRUIT_MS) {
    tLastPublish += ADAFRUIT_MS;
    sendToAdafruit();
  }
}

/* ==================== helpers ======================== */
void acquireVI() {
  int rawV = analogRead(VOLTAGE_PIN);
  int rawI = analogRead(CURRENT_PIN);

  vRms = rawV * VOLTAGE_GAIN;

  float mV  = rawI * ADC_mV_STEP;
  iRms = (mV - CURR_ZERO_mV) / CURR_SENS_mV_A;
  if (iRms < 0) iRms = 0;

  pInst = vRms * iRms;
}

void handlePulse() {
  bool now = digitalRead(PULSE_PIN);
  if (lastPulse == HIGH && now == LOW) {      // falling edge
    deductOneUnit();
    sendToAdafruit();                         // instant update
  }
  lastPulse = now;
}

void deductOneUnit() {
  uint8_t bal = EEPROM.read(EEPROM_BAL_ADDR);
  if (bal >= RS_PER_UNIT) bal -= RS_PER_UNIT;
  else bal = 0;
  EEPROM.write(EEPROM_BAL_ADDR, bal);
  energyWh = 0;             // reset counter
  showBalance();
}

void checkBalance() {
  uint8_t bal = EEPROM.read(EEPROM_BAL_ADDR);

  if (bal == 0) {
    digitalWrite(RELAY_PIN, LOW);
    lcd.setCursor(0, 1);
    lcd.print(F("SUPPLY CUT      "));
  } else if (bal < LOW_BALANCE_RS) {
    lcd.setCursor(0, 1);
    lcd.print(F("Low Bal Rs: "));
    lcd.print(bal);
    lcd.print(F("  "));
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, HIGH);
  }
}

void showBalance() {
  uint8_t bal = EEPROM.read(EEPROM_BAL_ADDR);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Units: "));
  lcd.print(bal / RS_PER_UNIT, 1);
  lcd.setCursor(0, 1);
  lcd.print(F("Bal Rs: "));
  lcd.print(bal);
  lcd.print(F("   "));
}

/* ---------- Adafruit IO publish helper --------------- */
void sendToAdafruit() {
  if (io.status() != AIO_CONNECTED) return;     // don’t block

  uint8_t bal = EEPROM.read(EEPROM_BAL_ADDR);
  float   unitsLeft = bal / RS_PER_UNIT;

  feedPower  ->save(pInst);
  feedUnits  ->save(unitsLeft);
  feedBalance->save(bal);
}
