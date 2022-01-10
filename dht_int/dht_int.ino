#include<Ticker.h>
#define DHT11PIN D2
#define DHT11PIN2 D3

Ticker tickerLed;

void ICACHE_RAM_ATTR ledISR();

int callAllInt =0, count = 0, dt_Index = 0, readHumid = 0, readTemp = 0, dt[82] = {0,}, before = micros();

void ICACHE_RAM_ATTR change(){
  //Serial.println(count);
  if(callAllInt > 20 && callAllInt < 25){
    callAllInt++;
    dt[dt_Index * 2] = micros();
  }
  else if(callAllInt == 25){
     dt[dt_Index * 2] = micros() - dt[dt_Index * 2];
     dt[dt_Index * 2 + 1] = micros();
     callAllInt = 26;
  }
  else if(callAllInt == 26){
     dt[dt_Index * 2 + 1] = micros() - dt[dt_Index * 2 + 1];  
     callAllInt = 25;
     count++;
     dt_Index++;
     dt[dt_Index * 2] = micros();
     if(count == 40){
        for(dt_Index = 0 ; dt_Index < 8 ; dt_Index++){
          readHumid = readHumid << 1;
          if(dt[dht_Index * 2 + 1] > 49){
            readHumid = readHumid + 1;  
          } 
          else readHumid = readHumid + 0;
        }
    
        for(dt_Index = 16 ; dt_Index < 24 ; dt_Index++){
          readTemp = readTemp << 1;
          if(dt[dt_Index * 2 + 1] > 49){
            readTemp = readTemp + 1;  
          } 
          else readTemp = readTemp + 0;
        }

        char buf[80];
        sprintf(buf, " Temp:%d, Humid:%d\r\n", readTemp, readHumid);
        Serial.println(buf);

        noInterrupts();
        tickerLed.attach_ms(6000,ledISR);
        
        callAllInt = 0;
        count = 0;
        dt_Index = 0; 
        readTemp = 0;
        readHumid = 0;
     }
  }
}

void ICACHE_RAM_ATTR ledISR(){
  if(callAllInt == 0){
    callAllInt++;
    digitalWrite(DHT11PIN, 1);
    pinMode(DHT11PIN, OUTPUT);
    tickerLed.attach_ms(1,ledISR);
  }
  else if(callAllInt == 1){
    callAllInt++;
    digitalWrite(DHT11PIN, 0);
  }
  else if(callAllInt > 1 && callAllInt < 20) callAllInt++;
  else if(callAllInt == 20){
    callAllInt++;
    noInterrupts();
    attachInterrupt(digitalPinToInterrupt(DHT11PIN), change, CHANGE);
    pinMode(DHT11PIN, INPUT_PULLUP);
  }
}

void setup() {
  Serial.begin(57500);
  delay(1000);

  tickerLed.attach_ms(1,ledISR);
}

void loop() {
}
