/*
 Name:		Test.ino
 Created:	4/20/2018 2:03:41 PM
 Author:	anonymouls
*/
#include <SoftwareSerial.h>


const int Speed = 14400;
const unsigned short SensorPin = 2;
const unsigned short LedPin = 11;

volatile unsigned char Iterator = 11;

const unsigned short RXBTPIN = 11;
const unsigned short TXBTPIN = 10;

short DigitControlPins[8] = { 12,8,7,5,6,11,13};
short DigitControlDisplays[2] = { 9,10 };

//SoftwareSerial BTSerial(RXBTPIN, TXBTPIN);

class DigitControl {
private:
	// 0	1	2	3	4	5	6	7	8		9
	// A	B	C	D	E	F	G	P	VCC1	VCC2
	short* SegmentPins;
	short* DisplayPins;
	bool IsInverted = true;
	short  Displays = 1;
	int Frequency = 30;
	byte numbers[10] = { 
		//ABCDEFGP
		0b11111100,//0
		0b01100000,//1
		0b11011010,//2
		0b11110010,//3
		0b01100110,//4
		0b10110110,//5
		0b10111110,//6
		0b11100000,//7
		0b11111110,//8
		0b11110110 //9
	};
	int CurrentNumber = 0;
	short Iteration = 0;


	void PrintNumber(int Number) {
		int i = 0;
		for(int i = 0;i<7;i++) {
			bool Value = bitRead(numbers[Number], 7-i);
			if (IsInverted) Value = !Value;
			digitalWrite(SegmentPins[i], Value);
		}
	}
	void SwitchDisplay(int Display) {
		for (int i = 0; i < sizeof(DigitControlDisplays) / sizeof(short); i++) {
			digitalWrite(DisplayPins[i], LOW);
		}
		digitalWrite(DisplayPins[Display], HIGH);
	}
	void EnterNumber() {
		int A = -1;
		int B = -1;
		if ((CurrentNumber > 9) && (CurrentNumber < 99)) {
			A = CurrentNumber / 10;
			B = CurrentNumber % 10;
			if (Iteration % 2 == 0) {
				SwitchDisplay(0);
				PrintNumber(A);
			}
			else {
				SwitchDisplay(1);
				PrintNumber(B);
			}
			Iteration++;
		}
		else {
			SwitchDisplay(1);
			PrintNumber(CurrentNumber);
		}
		CheckValues();
	}
	void CheckValues() {
		if (Iteration > 240) Iteration = 0;
	}
public:
	DigitControl(short* SegmentPins, short* DisplayPins) {
		this->SegmentPins = SegmentPins;
		this->DisplayPins = DisplayPins;
	}
	// Make universal?
	void SetCurrentNumber(int InputNumber) {
		CurrentNumber = InputNumber;
		EnterNumber();
	}
};

DigitControl DC(DigitControlPins, DigitControlDisplays);

void setup() {
	Serial.begin(Speed);
	pinMode(2, INPUT);
	attachInterrupt(digitalPinToInterrupt(SensorPin), SensorInterruption, CHANGE);
}

void SensorInterruption() {
	++Iterator;
	Serial.println(Iterator);
}

int Test = 0;

void loop() {
	//while (1) {
	//	////DC.SetCurrentNumber(Iterator++);
	//	//delay(50);
	int ch = digitalRead(2);
	Serial.println(ch);
	//}
}
