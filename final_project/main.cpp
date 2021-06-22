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
int ping_cunt=0;

int main() {
   
   uart.set_baud(9600);
   pc.set_baud(9600);
   sub_task = 1;
   steps = 0;
   
   char buf[64];
   char buf_start[6] = "start";
   char buf_end[4] = "end";
   char buf_line[5] = "line";
   char buf_circle[7] = "circle";
   char buf_location[9] = "location";
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
               car.turn(-100, 0.7);
               ThisThread::sleep_for(50ms);
               car.stop();
               steps += 1;
            }else if(recv[0]=='r'){
               car.stop();
               car.turn(-100, -0.7);
               ThisThread::sleep_for(50ms);
               car.stop();
               steps += 1;
            }else if(recv[0]=='o'){
               car.goStraight(-100);
               ThisThread::sleep_for(50ms);
               car.stop();
               steps += 1;
            }else{
               car.stop();
            }
            if(ping_cunt<15 && sub_task==3) ping_cunt++;
            else if(ping_cunt==15 && sub_task==3){
               ping_cunt = 0;
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
            } 
            }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
      }
      if(sub_task==1 && steps==400){
          sub_task = 2;
          steps = 0;
          cxbee.write(buf_end, sizeof(buf_end));
          xbee.write(buf_line, sizeof(buf_line));
          xbee.write(buf_start, sizeof(buf_start));
          xbee.write(buf_circle, sizeof(buf_circle));
      }
      else if(sub_task==2){
          car.turn(-100, 0.7);
          ThisThread::sleep_for(6000ms);
          car.stop();
          sub_task = 3;
          xbee.write(buf_end, sizeof(buf_end));
          xbee.write(buf_circle, sizeof(buf_circle));
          xbee.write(buf_start, sizeof(buf_start));
          xbee.write(buf_location, sizeof(buf_location));
      }
      else if(sub_task==3 && steps==300){
          sub_task = 0;
          xbee.write(buf_end, sizeof(buf_end));
          xbee.write(buf_location, sizeof(buf_location));
      }
   }
}