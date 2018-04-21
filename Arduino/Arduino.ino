#include <LiquidCrystal.h>
#include <UIPEthernet.h> // Used for Ethernet
#include <stdlib.h>

const int numOfSensors = 1;
struct Sensor {
  char* measurementName;
  int portNumber;
  char* sensorName;
};

struct Sensor SENSORS[numOfSensors] = {
  {"light", 0, "0"}
};

// **** ETHERNET SETTING ****
byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x31 };                                                          
EthernetUDP udp;

LiquidCrystal lcd(9, 8, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  Serial.println("Serial Monitor Inited");

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("LCD Inited");
  delay(1000);
  lcd.clear();

  // start the Ethernet connection and the server:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("FAILED Ethernet connection");
    lcd.println("FAILED Ethernet");
  }

  Serial.print("Ethernet client: ");
  Serial.println(Ethernet.localIP());
  lcd.print("Ip:");
  lcd.println(Ethernet.localIP());
  delay(2000);
}

void loop() {
  int reading;
  char readingCharBuffer[12];

  int success = udp.beginPacket(IPAddress(18,236,166,41), 8125);
  Serial.println("UDP beginPacket started");
  if (success == 0) {
    Serial.println("UDP beingPacket failed");
    lcd.println("UDP beingPacket failed");
  } else {
    for(int i = 0; i < numOfSensors; i++) {
      reading = analogRead(SENSORS[i].portNumber);
      itoa(reading, readingCharBuffer, 10);
      udp.write(SENSORS[i].measurementName);
      udp.write(".");
      udp.write(SENSORS[i].sensorName);
      udp.write(":");
      udp.write(readingCharBuffer);
      udp.write("|g");
      udp.write("\n");
    }
    udp.endPacket();
    Serial.println("Sent udp packet");
    lcd.println("Metric sent");
    delay(1000);
    lcd.clear();
    delay(9000);
  }
}

