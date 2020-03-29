#include "PCF2123.h"

//Create an instance of the RTC lib
PCF2123 time(A5);


void setup(){
	Serial.begin(9600);

	//Initilise the RTC
	time.begin();
}


void loop(){
	//Create a instance of the time struct
	Time t;
	//Fetch the current time from the RTC into the struct
	time.get_time(&t);

	//Print the current time
	Serial.println();
	Serial.print(t.Hour);
	Serial.print(":");
	Serial.print(t.Minute);
	Serial.print(":");
	Serial.println(t.Second);

	delay(5000);
}
