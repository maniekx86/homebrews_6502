#include <Wire.h>
#include <digitalWriteFast.h>

#define EEPROM_ADDRESS_A0 0x38
// #define EEPROM_CE_PIN A2
#define EEPROM_OE_PIN A1
#define EEPROM_WE_PIN A0

#define A8_PIN 2
#define A9_PIN 3
#define A10_PIN 4

#define A11_PIN 13
#define A12_PIN A2
#define A13_PIN A3

uint8_t pageSize = 16;
uint8_t currentMode = 0; // 0 -> 28C16, 1 -> 28C256

uint8_t dataBuf[256];

int lastMode = 0; // 0 - READ, 1 - WRITE (to remember pin modes)

// #define LED_PIN 13

int datalines[] = {5, 6, 7, 8, 9, 10, 11, 12};

void setup() {
  Wire.begin();
  TWBR = 10; // this "overclocks" the i2c to 444 KHz making page write work for 28C256
  Serial.begin(115200);
  //pinMode(EEPROM_CE_PIN, OUTPUT);
  pinMode(EEPROM_OE_PIN, OUTPUT);
  pinMode(EEPROM_WE_PIN, OUTPUT);
  pinMode(A8_PIN, OUTPUT);
  pinMode(A9_PIN, OUTPUT);
  pinMode(A10_PIN, OUTPUT);

  pinMode(A11_PIN, OUTPUT);
  pinMode(A12_PIN, OUTPUT);
  pinMode(A13_PIN, OUTPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(datalines[i], OUTPUT);
  }

  lastMode = 1;
  digitalWrite(EEPROM_OE_PIN, HIGH);
  digitalWrite(EEPROM_WE_PIN, HIGH);
  //digitalWrite(EEPROM_CE_PIN,HIGH);

  //digitalWrite(LED_PIN, OUTPUT);
  //digitalWrite(LED_PIN, HIGH);
}

void writePCF(uint8_t address, uint8_t data) {
  Wire.beginTransmission(address);
  Wire.write(data);
  Wire.endTransmission();
}

void setAddress(uint16_t address) {
  uint8_t addrLow = address & 0xFF;
  digitalWriteFast(A8_PIN, (address >> 8) & 0x01); // Set A8
  digitalWriteFast(A9_PIN, (address >> 9) & 0x01); // Set A9
  digitalWriteFast(A10_PIN, (address >> 10) & 0x01); // Set A10
  digitalWriteFast(A11_PIN, (address >> 11) & 0x01); // Set A11
  digitalWriteFast(A12_PIN, (address >> 12) & 0x01); // Set A12
  digitalWriteFast(A13_PIN, (address >> 13) & 0x01); // Set A13

  writePCF(EEPROM_ADDRESS_A0, addrLow);
}
void writeByte(uint16_t address, uint8_t data) {
  digitalWrite(EEPROM_OE_PIN, HIGH);
  //digitalWrite(EEPROM_CE_PIN, LOW);
  setAddress(address);

  for (int i = 0; i < 8; i++) {
    if (lastMode == 0) pinMode(datalines[i], OUTPUT);
    digitalWrite(datalines[i], (data >> i) & 0x01);
  }
  lastMode = 1;

  digitalWrite(EEPROM_WE_PIN, LOW);
  delay(1);

  digitalWrite(EEPROM_WE_PIN, HIGH);
  //digitalWrite(EEPROM_CE_PIN, HIGH);
  delay(11); // Wait for the write cycle to complete
}

uint8_t readByte(uint16_t address) {
  setAddress(address);

  uint8_t data = 0;

  if (lastMode == 1) {
    for (int i = 0; i < 8; i++) {
      pinMode(datalines[i], INPUT);
    }
    lastMode = 0;
  }

  digitalWriteFast(EEPROM_OE_PIN, LOW);
  //digitalWrite(EEPROM_CE_PIN, LOW);
  delayMicroseconds(2);

  for (int i = 0; i < 8; i++) {
    data |= digitalRead(datalines[i]) << i;
  }


  digitalWriteFast(EEPROM_OE_PIN, HIGH);
  //digitalWrite(EEPROM_CE_PIN, HIGH);

  return data;
}



void writePage(uint16_t page) {
  digitalWrite(EEPROM_OE_PIN, HIGH);
  if (lastMode == 0) {
    for (int i = 0; i < 8; i++) {
      pinMode(datalines[i], OUTPUT);
    }
    lastMode = 1;
  }

  uint8_t data;

  // we are going just to write to PCF as we dont need to modify A8-A13 as the page is 64 bytes for 28c256 and 16 bytes for 28c16 using less than 8 data pins
  setAddress(page * pageSize); // will set the upper bits from A8 that we need
  uint8_t addrLow = (page * pageSize) & 0xFF;

  //writePCF(EEPROM_ADDRESS_A0, addrLow);

  if (currentMode == 1) { // 28C256

    for (uint8_t d = 0; d < pageSize; d++) {
      /*
         Bottom bits should be zero as pageSize should be in 2^. We can just OR bytes for very fast math and setting keeping the upper PCF exapnder bits (to A7) not touched by addrLow
      */
      writePCF(EEPROM_ADDRESS_A0, d | addrLow);

      data = dataBuf[d];

      for (int i = 0; i < 8; i++) {
        digitalWriteFast(datalines[i], (data >> i) & 0x01);
      }

      digitalWriteFast(EEPROM_WE_PIN, LOW); // about 100-200 nS I guess
      digitalWriteFast(EEPROM_WE_PIN, HIGH);
    }
  } else { // 28C16

    for (uint8_t d = 0; d < pageSize; d++) {
      /*
         Bottom bits should be zero as pageSize should be in 2^. We can just OR bytes for very fast math and setting keeping the upper PCF exapnder bits (to A7) not touched by addrLow
      */
      writePCF(EEPROM_ADDRESS_A0, d | addrLow);

      data = dataBuf[d];

      for (int i = 0; i < 8; i++) {
        digitalWriteFast(datalines[i], (data >> i) & 0x01);
      }

      digitalWriteFast(EEPROM_WE_PIN, LOW); // about 100-200 nS I guess
      delayMicroseconds(1);
      digitalWriteFast(EEPROM_WE_PIN, HIGH);
    }
  }

  delay(11); // needs to write data

}

bool blinking;
bool ledState = true;
unsigned long int blinkTime = 0;
unsigned long int blinkDur = 0;


void loop() {
  blinking = false;
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'D') { // detect
      Serial.write('$');
    }

    if (command == 'M') {
      while (!Serial.available());
      char mode = Serial.read();
      if (mode == '0') { // 28C16
        pageSize = 16;
        currentMode = 0;
      }
      if (mode == '1') { // 28C256
        pageSize = 64;
        currentMode = 1;
      }
    }

    if (command == 'P') { // page write
      // 28C256 have 512 pages 64 bytes each (or 2x 256 pages). As programmer can only address 16 KB we need to program two times 16 KB of data by user switching mechanical switch lol
      // 28C16 have 128 pages 16 bytes each
      while (!Serial.available());
      char page = Serial.read();

      uint8_t readen = 0;
      while (readen < pageSize) {
        if (Serial.available()) {
          dataBuf[readen] = Serial.read();
          readen++;
        }
      }

      writePage(page);

      Serial.write('P');

    }

    if (command == 'F') { // page read v2
      while (!Serial.available());
      uint8_t page = Serial.read();
      uint16_t address = page * pageSize;
      dataBuf[0] = 'F';
      for (int i = 0; i < pageSize; i++) {
        dataBuf[i + 1] = readByte(address | i);
      }

      Serial.write(dataBuf, pageSize + 1);
    }

    if (command == 'R') {
      while (!Serial.available());
      uint8_t addr1 = Serial.read();
      while (!Serial.available());
      uint8_t addr2 = Serial.read();
      //digitalWrite(EEPROM_CE_PIN,LOW);
      uint8_t rByte = readByte((addr1 * 256) + addr2);
      Serial.write('R');
      Serial.write(rByte);
      blinkDur = millis() + 500;
    }
    /*if(command == 'F') { // fast read
      while(!Serial.available()); // 128 pages 16 bytes of data
      uint8_t page = Serial.read();
      uint16_t startAddr = page * 16;
      uint8_t sData[17];
      sData[0] = 'F';
      //digitalWrite(EEPROM_CE_PIN,LOW);
      for(int i=0;i<16;i++) {
        sData[i+1] = readByte(startAddr+i);
      }
      Serial.write(sData,17);
      blinkDur = millis()+500;
      }*/
    if (command == 'W') {
      while (!Serial.available());
      uint8_t addr1 = Serial.read();
      while (!Serial.available());
      uint8_t addr2 = Serial.read();
      while (!Serial.available());
      uint8_t wdata = Serial.read();
      //digitalWrite(EEPROM_CE_PIN,LOW);
      writeByte((addr1 * 256) + addr2, wdata);
      Serial.write('W');
      blinkDur = millis() + 500;
    }
    if (command == 'V') {
      delay(5);
      char subCmd = Serial.read();
      if (subCmd == 'A') {
        for (uint16_t address = 0; address < 256; address = address + 2) {
          writeByte(address, 0xAA);
          writeByte(address + 1, 0x55);
        }
      }
      if (subCmd == 'B') {
        for (uint16_t address = 0; address < 512; address++) {
          writeByte(address, address % 256);
        }
      }

      if (subCmd == 'X') {
        for (uint16_t address = 0; address < 8192; address = address + 2) {
          writeByte(address, 0xAA);
          writeByte(address + 1, 0x55);
        }
      }
      if (subCmd == 'Y') {
        for (uint16_t address = 0; address < 16384; address++) {
          writeByte(address, address % 256);
        }
      }

      Serial.println("WRITTEN");
    }

    if (command == 'C') {
      // Read data from EEPROM and send it to serial in hex
      int b = 0;
      for (uint16_t address = 0; address < 2048; address++) {
        uint8_t data = readByte(address);
        Serial.print("0x");
        if (data < 16) {
          Serial.print("0");
        }
        Serial.print(data, HEX);
        Serial.print(" ");
        b++;
        if (b == 16) {
          b = 0;
          Serial.println();
        }
      }
      Serial.println();
    }

    if (command == 'X') {
      // Read data from EEPROM and send it to serial in hex (16K)
      int b = 0;
      for (uint16_t address = 0; address < 16384; address++) {
        uint8_t data = readByte(address);
        Serial.print("0x");
        if (data < 16) {
          Serial.print("0");
        }
        Serial.print(data, HEX);
        Serial.print(" ");
        b++;
        if (b == 16) {
          b = 0;
          Serial.println();
        }
      }
      Serial.println();
    }
  }

  blinking = (millis() < blinkDur);

  if (blinking) {
    if (millis() >= blinkTime) {
      blinkTime = millis() + 250;
      ledState = !ledState;
      //digitalWrite(LED_PIN,ledState);
    }
  } else {
    if (ledState == false) {
      //      digitalWrite(LED_PIN,HIGH);
      ledState = true;
    }
    //digitalWrite(EEPROM_CE_PIN,HIGH);

  }
}
