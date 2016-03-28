#include "Microduino_Protocol.h"

Protocol::Protocol(HardwareSerial *ser){
	commonInit();
	pHwSerial = ser;
}

Protocol::Protocol(SoftwareSerial *ser){
	commonInit();
	pSwSerial = ser;
}

void Protocol::commonInit(void){
	pSwSerial = NULL;
	pHwSerial = NULL;
	dataParse = NULL;
}

void Protocol::begin(uint16_t _baud, uint8_t _cmd, uint8_t _len){
	baud = _baud;
	if(pSwSerial){
		pSwSerial->begin(baud);
	}else{
		pHwSerial->begin(baud);
	}
	dataParse = new DataParse(_cmd, _len);
}


bool Protocol::available(){
	uint8_t inChar = 0x00;
	if(pSwSerial){
		if(pSwSerial->available() > 0){
			inChar = pSwSerial->read();
			if(dataParse->parse(inChar)){
//				pSwSerial->clear();		
				pSwSerial->stopListening();
				pSwSerial->listen();
				return true;
			}
		}
	}else{
		if(pHwSerial->available() > 0){
			inChar = pHwSerial->read();
			if(dataParse->parse(inChar)){
//				pHwSerial->clear();		
				pHwSerial->end();
				pHwSerial->begin(baud);
				return true;
			}
		}
	}	
	return false;	
}


void Protocol::read8(uint8_t *_buf, uint8_t _len){
	
	dataParse->read(_buf, _len);
}

void Protocol::read16(uint16_t *_buf, uint8_t _len){
	uint8_t ouputData[_len*2];
	read8(ouputData, _len*2);	
	for(int i=0; i < _len; i++){
		_buf[i] = MAKE_WORD(ouputData[2*i+1], ouputData[2*i]);
	}
}

