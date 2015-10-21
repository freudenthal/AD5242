/*
AD5252.h Library for communicating with AD5252 Digital Potentiometer
Last update 10/21/2015
John Freudenthal
*/

#ifndef AD5242_h	//check for multiple inclusions
#define AD5242_h

#include "Arduino.h"
#include "i2c_t3.h"

#define I2CTimeout 1000

class AD5242
{
	public:
		AD5242();
		AD5242(uint8_t address);
		~AD5242();
		bool isConnected();
		uint8_t getRDAC(uint8_t Channel);
		bool getOutput(uint8_t Channel);
		void setRDAC(uint8_t Channel, uint8_t Target);
		void setOutput(uint8_t Channel, bool OutputSetting);
		void setAddress(uint8_t address);
		uint8_t getAddress();
		void setMidScale(bool Value);
		void setShutdown(bool Value);
	private:
		uint8_t Address;
		uint8_t CommandByte;
		uint8_t WiperPosition;
		bool Output0;
		bool Output1;
		void SendI2CCommand();
		uint8_t SingleByteI2CRead();
		void SendI2CCommand();
};
#endif