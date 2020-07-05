/*
 Info:
	Dark Calibration Is Unnecessary
*/

#include <SoftwareSerial.h>	// Bluetooth Serial Writer/Reader
#include <CyberLib.h>		// Analog Read Speed Booster

// Operational Values
int DefaultLightValue = -1;
int DefaultLightOffset = -1;
int DefaultDarkValue = -1;
int DefaultDarkOffset = -1;
unsigned short Iterator = 0;
bool Interrupted = false;

// Physical Values
const int LedPin = 13;
const int ResetIteratorButton = 2;
const int CalibrateIterator = 200;
const int CalibrateFuse = 10;
const unsigned long Speed = 200000;

// Utility Functions
bool CheckAppox(int ValueA, int ValueB, int Offset) {
	int A = abs(ValueA - ValueB);
	if ((A > Offset + 1) && (A != 0)) return true; else return false;
}
#define abs(x) ((x)>0?(x):-(x))

// Execute Functions
int CalibrateLightValue() {
	int Result = A4_Read;
	delay(1000);
	for (int i = 0; i < CalibrateIterator; i++) {
		Result += A4_Read;
		Result /= 2;
	}
	return Result;
}
int CalibrateLightOffset() {
	int Result = abs(A4_Read - DefaultLightValue);
	delay(1000);
	for (int i = 0; i < CalibrateIterator; i++) {
		Result += abs(A4_Read - DefaultLightOffset);
		Result /= 2;
	}
	return Result;
}
int CalibrateDarkValue() {
	int Result = A4_Read;
	delay(1000);
	for (int i = 0; i < CalibrateIterator; i++) {
		Result += A4_Read;
		Result /= 2;
	}
	return Result;
}
int CalibrateDarkOffset() {
	int Result = abs(A4_Read - DefaultDarkValue);
	delay(1000);
	for (int i = 0; i < CalibrateIterator; i++) {
		Result += abs(A4_Read - DefaultDarkValue);
		Result /= 2;
	}
	return Result;
}

// Struct Functions
void CalibrateAll() {
	digitalWrite(LedPin, HIGH);
	Serial.println("Calibrating System Values");
	Serial.print("Default Light: ");		DefaultLightValue = CalibrateLightValue() + CalibrateFuse;	 Serial.println(DefaultLightValue);
	Serial.print("Default Light Offset: "); DefaultLightOffset = CalibrateLightOffset() + CalibrateFuse; Serial.println(DefaultLightOffset);
//	digitalWrite(LedPin, LOW);
//	Serial.print("Default Dark: ");			DefaultDarkValue = CalibrateDarkValue() + CalibrateFuse;	 Serial.println(DefaultDarkValue);
//	Serial.print("Default Dark Offser: ");	DefaultDarkOffset = CalibrateDarkOffset() + CalibrateFuse;	 Serial.println(DefaultLightOffset);
}

void setup() {
	Serial.begin(Speed);
	pinMode(LedPin, OUTPUT);
	CalibrateAll();
	digitalWrite(LedPin, HIGH);
}

void loop() {
	int CurrentValue = A4_Read;
	if (CheckAppox(DefaultLightValue, CurrentValue, DefaultLightOffset) && !Interrupted)
//			&& CheckAppox(DefaultDarkValue, CurrentValue, DefaultDarkOffset))
	{
		Serial.print("Shots: ");	Serial.print(++Iterator);
		Serial.print("\tCurrent Value: ");  Serial.print(CurrentValue);
		Serial.print("\t Delta: ");       Serial.print(abs(DefaultLightValue - CurrentValue));
		Serial.print("\tDDV: ");  Serial.print(DefaultLightValue);
		Serial.print("\tDDO: ");  Serial.println(DefaultLightOffset);
		Interrupted = true;
	}
	else if (!CheckAppox(DefaultLightValue, CurrentValue, DefaultLightOffset) && Interrupted)
//				&& CheckAppox(DefaultDarkValue, CurrentValue, DefaultDarkOffset))
	{
		Serial.println("RESET");
		Interrupted = false;
	}
}