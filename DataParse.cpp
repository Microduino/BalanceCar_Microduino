#include "DataParse.h"

DataParse::DataParse(uint8_t _cmd, uint8_t _length){
	cmd = _cmd;
	length = _length;
	memset(buffer, 0, length);
	step = STEP_WAIT_AA;	
}

uint8_t DataParse::getChecksum(uint8_t _len, uint8_t _cmd, uint8_t *_data) {
  //三个参数分别为： 数据长度  ，  指令代码  ，  实际数据数组
  uint8_t checksum = 0;
  checksum ^= (_len & 0xFF);
  checksum ^= (_cmd & 0xFF);
  for (int i = 0; i < _len; i++) checksum ^= (_data[i] & 0xFF);
  return checksum;
}


void DataParse::read(uint8_t *_buf, uint8_t _len){	
	if(_len > length){
		_len = length;
	}
	memcpy(_buf, buffer, _len);
}	


//数据处理函数�������ݴ�����
bool DataParse::parse(uint8_t tempChar)		{
	if(step == STEP_WAIT_AA){	        //等待接收0xAA 
		if(tempChar == STXA){
			step = STEP_WAIT_BB;  
		}
	}else if(step == STEP_WAIT_BB){	   //等待接收0xBB
		if(tempChar == STXB){
		    step = STEP_WAIT_CMD;
		}else{
			step = STEP_WAIT_AA;  
		}
   }else if(step == STEP_WAIT_CMD){		//等待接收命令
		if(tempChar == cmd){
			step = STEP_WAIT_DATA;
		}else{
			step = STEP_WAIT_AA;
		}
		index = 0; 
   }else if(step == STEP_WAIT_DATA){    //等待接收数据
      buffer[index++] = tempChar;
	  if(index >= length){
		  step = STEP_WAIT_SUM;
	  }
   }else if(step == STEP_WAIT_SUM){     //等待接收校验码
	   step = STEP_WAIT_AA;
	   bcc = getChecksum(length, cmd, buffer);
	   if(bcc == tempChar){
			return true;
	   }
   }
   return false;
}

