#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "string.h"
DigitalInOut ping(D12);
Timer t;
float val;

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D10,D9);
BufferedSerial uart(D1,D0); //tx,rx
BufferedSerial pc(USBTX,USBRX); //tx,rx

BBCar car(pin5, pin6, servo_ticker);
int sub_task;
int steps;
int tag;

int main() {
   
   uart.set_baud(9600);
   pc.set_baud(9600);
   sub_task = 1;
   steps = 0;
   tag = 0;
   
   char buf[64];
   char buf_start[7] = "start\n";
   char buf_end[5] = "end\n";
   char buf_line[6] = "line\n";
   char buf_circle[8] = "circle\n";
   char buf_location[10] = "location\n";
   xbee.write(buf_start, sizeof(buf_start));
   xbee.write(buf_line, sizeof(buf_line));
   while(1){
      if(uart.readable()){                                                  
            char recv[1];
            uart.read(recv, sizeof(recv));                              
            //pc.write(recv, sizeof(recv));
            if(sub_task==1 || sub_task==3){
            if(recv[0]=='l'){
               car.stop();
               car.turn(-75, 0.5);
               ThisThread::sleep_for(50ms);
               car.stop();
               steps += 1;
            }else if(recv[0]=='r'){
               car.stop();
               car.turn(-75, -0.5);
               ThisThread::sleep_for(50ms);
               car.stop();
               steps += 1;
            }else if(recv[0]=='o'){
               car.goStraight(-75);
               ThisThread::sleep_for(50ms);
               car.stop();
               steps += 1;
            }else{
               car.stop();
            }
            }//1 3
            if(sub_task==4 && recv[0]=='t'){
               tag += 1;
               ping.output();
               ping = 0; wait_us(200);
               ping = 1; wait_us(5);
               ping = 0; wait_us(5);
               ping.input();
               while(ping.read() == 0);
               t.start();
               while(ping.read() == 1);
               val = t.read();
               sprintf(buf, "Ping = %lf\r\n", val*17700.4f);
               pc.write(buf, sizeof(buf));
               t.stop();
               t.reset();                                                                                                              
            }//4                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
      }//read
      if(sub_task==1 && steps==120){
          sub_task = 2;
          steps = 0;
          xbee.write(buf_end, sizeof(buf_end));
          xbee.write(buf_line, sizeof(buf_line));
          xbee.write(buf_start, sizeof(buf_start));
          xbee.write(buf_circle, sizeof(buf_circle));
          car.stop();
          ThisThread::sleep_for(4000ms);
      }
      else if(sub_task==2){
          car.turn(-100, -0.4);
          ThisThread::sleep_for(13000ms);
          
          sub_task = 3;
          xbee.write(buf_end, sizeof(buf_end));
          xbee.write(buf_circle, sizeof(buf_circle));
          xbee.write(buf_start, sizeof(buf_start));
          xbee.write(buf_line, sizeof(buf_line));
          car.stop();
          ThisThread::sleep_for(4000ms);
      }
      else if(sub_task==3 && steps==60){
          sub_task = 4;
          steps = 0;
          xbee.write(buf_end, sizeof(buf_end));
          xbee.write(buf_line, sizeof(buf_line));
          xbee.write(buf_start, sizeof(buf_start));
          xbee.write(buf_location, sizeof(buf_location));
          car.stop();
          ThisThread::sleep_for(4000ms);
      }
      else if(sub_task==4 && tag==4){
         sub_task = 5;
         xbee.write(buf_end, sizeof(buf_end));
         xbee.write(buf_location, sizeof(buf_location));
      }
   }
}