#include "userDef.h"
#include <RF24Network.h>
#include <RF24.h>
#include "Microduino_Protocol.h"

#define NRF   0
#define BLE   1

// nRF24L01(+) radio attached using Getting Started board
RF24 radio(9, 10);
RF24Network network(radio);

Protocol bleProtocol(&Serial1);

const uint16_t this_node = 1;  //设置本机ID
const uint16_t other_node = 0;

boolean mode = 0;
uint16_t channel_data[NUM_ROBOT];

//--------------------------------
struct send_a{ //发送
  uint32_t node_ms;   //节点运行时间
};

struct receive_a{  //接收
  uint32_t ms;
  uint16_t rf_CH0;
  uint16_t rf_CH1;
  uint16_t rf_CH2;
  uint16_t rf_CH3;
  uint16_t rf_CH4;
  uint16_t rf_CH5;
  uint16_t rf_CH6;
  uint16_t rf_CH7;
};


boolean protocolSetup(){ 
   radio.begin(); 
   if(radio.isPVariant()){ 
      network.begin(NRF_CHANNEL, this_node);
      mode = NRF;
   }else{ 
      bleProtocol.begin(BLE_SPEED, CMD_ROBOT, NUM_ROBOT);  
      mode = BLE;
   } 
} 

boolean nrfRead(uint16_t * _channel)
{
  network.update();
  // Is there anything ready for us?
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    receive_a rec;
    network.read(header, &rec, sizeof(rec));

    _channel[0] = rec.rf_CH0; 
    _channel[1] = rec.rf_CH1; 
    _channel[2] = rec.rf_CH2; 
    _channel[3] = rec.rf_CH3; 
    _channel[4] = rec.rf_CH4; 
    _channel[5] = rec.rf_CH5; 
    _channel[6] = rec.rf_CH6; 
    _channel[7] = rec.rf_CH7; 
    
    {
      send_a sen = { millis() };  //把这些数据发送出去，对应前面的发送数组
      RF24NetworkHeader header(0);
      boolean ok = network.write(header, &sen, sizeof(sen));
      return ok;
    }
  }
}


boolean protocolRead(uint16_t *_channel, uint8_t num){
    if(!mode){
       return nrfRead(_channel);
    }else if(bleProtocol.available()){
       bleProtocol.read16(_channel, num);
       return true;
    }      
    return false;
}



