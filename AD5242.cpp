/*
AD5252.cpp Library for communicating with AD5252 Digital Potentiometer
Last update 9/3/2015
Sean Kirkpatrick
*/

#include "AD5242.h"
#define combine(high,low) ( ( (uint16_t)(high << 8) ) | (uint16_t)(low) )
#define lowbyte(value) ( (uint8_t)(value) )
#define highbyte(value) ( (uint8_t)(value>>8) )

#define Output0Bit 4
#define Output1Bit 3
#define ShutdownBit 5
#define ResetBit 6
#define AddressBit 7

AD5242::AD5242()
{
	Address = 0x2C;
	CommandByte = 0x00;
	WiperPosition = 0x00;
	Output0 = false;
	Output1 = false;
	IsShutdown = false;
}

AD5242::AD5242(uint8_t address)
{
	Address = address;
	CommandByte = 0x00;
	WiperPosition = 0x00;
	Output0 = false;
	Output1 = false;
	IsShutdown = false;
}

AD5242::~AD5242()
{
}

bool AD5242::isConnected()
{
	int Status = 5;
	Wire.beginTransmission(Address);
	Status = Wire.endTransmission(I2C_STOP);
	if (Status == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AD5242::setAddress(uint8_t address)
{
	Address = address;
}

uint8_t AD5242::getAddress()
{
	return Address;
}

uint8_t AD5242::getRDAC(uint8_t Channel)
{
	Channel = constrain(Channel, 0, 1);
	switch(Channel)
	{
		case 1:
			bitWrite(CommandByte,AddressBit,0);
			break;
		case 0:
			bitWrite(CommandByte,AddressBit,1);
			break;
		default:
			break;
	}
	return SingleByteI2CRead();
}

void AD5242::setOutput(uint8_t Channel, bool OutputSetting)
{
	Channel = constrain(Channel, 0, 1);
	switch(Channel)
	{
		case 0:
			Output0 = OutputSetting;
			bitWrite(CommandByte,Output0Bit,OutputSetting);
			break;
		case 1:
			Output1 = OutputSetting;
			bitWrite(CommandByte,Output1Bit,OutputSetting);
			break;
		default:
			break;
	}
	SendI2CCommand();
}

void AD5242::setShutdown(bool Value)
{
	bitWrite(CommandByte,ShutdownBit,Value);
	SendI2CCommand();
}

void AD5242::setMidScale(bool Value)
{
	bitWrite(CommandByte,ResetBit,Value);
	SendI2CCommand();
}

bool AD5242::getOutput(uint8_t Channel)
{
	Channel = constrain(Channel, 0, 1);
	switch(Channel)
	{
		case 0:
			return Output0;
		case 1:
			return Output1;
		default:
			return 0;
			break;
	}
}

void AD5242::setRDAC(uint8_t Channel, uint8_t Target)
{
	Channel = constrain(Channel, 0, 1);
	switch(Channel)
	{
		case 0:
			bitWrite(CommandByte,AddressBit,0);
			break;
		case 1:
			bitWrite(CommandByte,AddressBit,1);
			break;
		default:
			break;
	}
	WiperPosition = Target;
	SendI2CCommand();
}

uint8_t AD5242::SingleByteI2CRead()
{
	bool MoveOn = false;
	const int MaxAttempts = 16;
	int CurrentAttempt = 0;
	int SendSuccess = 5;
	while (!MoveOn)
	{
		Wire.beginTransmission(Address);
		Wire.write(CommandByte);
		SendSuccess = Wire.endTransmission(I2C_STOP,I2CTimeout*2);
		if(SendSuccess != 0)
		{
			Wire.finish();
			Wire.resetBus();
			CurrentAttempt++;
			if (CurrentAttempt > MaxAttempts)
			{
				MoveOn = true;
			}
		}
		else
		{
			MoveOn = true;
		}
	}
	Wire.requestFrom(Address, 1, I2C_STOP, I2CTimeout*2);
	if (Wire.available())
	{
		return Wire.readByte();
	}
	else
	{
		return 0x00;
	}
}

void AD5242::SendI2CCommand()
{
	bool MoveOn = false;
	const int MaxAttempts = 16;
	int CurrentAttempt = 0;
	int SendSuccess = 5;
	while (!MoveOn)
	{
		Wire.beginTransmission(Address);
		Wire.write(CommandByte);
		Wire.write(WiperPosition);
		SendSuccess = Wire.endTransmission(I2C_STOP,I2CTimeout*2);
		if(SendSuccess != 0)
		{
			Wire.finish();
			Wire.resetBus();
			CurrentAttempt++;
			if (CurrentAttempt > MaxAttempts)
			{
				MoveOn = true;
			}
		}
		else
		{
			MoveOn = true;
		}
	}
}