#ifndef _MICRODUINO_PROTOCOL_H_
#define _MICRODUINO_PROTOCOL_H_

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "DataParse.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#define CMD_ROBOT 0xC8
#define NUM_ROBOT 16

//将两个字节组合成一个WORD
#define MAKE_WORD(HI,LO)		(((((uint16_t)((uint8_t)HI)))<<8)| ((uint16_t)((uint8_t)LO)))

class Protocol{
  public:
  
	Protocol(HardwareSerial *ser);
	Protocol(SoftwareSerial *ser);
	
    void begin(uint16_t _baud, uint8_t _cmd, uint8_t _len);
	bool available(void);
    void read8(uint8_t *_buf, uint8_t _len);
	void read16(uint16_t *_buf, uint8_t _len);

  private:
  
  	SoftwareSerial *pSwSerial;
	HardwareSerial *pHwSerial;
	DataParse *dataParse;
	uint16_t baud;

	void commonInit(void);
};


#endif
