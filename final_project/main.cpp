#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "string.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D10, D9);
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx

BBCar car(pin5, pin6, servo_ticker);
int sub_task;
int steps;

int main() {
   pc.set_baud(9600);
   uart.set_baud(9600);
   sub_task = 1;
   steps = 0;
   xbee.write("Start: Follow the line", 23);
   while(1){
      if(uart.readable()){                                                    
            char recv[1];
            uart.read(recv, sizeof(recv));                              
            //pc.write(recv, sizeof(recv));
            if(sub_task==1 || sub_task==3){
            if(recv[0]=='l'){
               car.stop();
               car.turn(-100, 0.2);
               ThisThread::sleep_for(100ms);
               car.stop();
               steps += 1;
            }else if(recv[0]=='r'){
               car.stop();
               car.turn(-100, -0.2);
               ThisThread::sleep_for(100ms);
               car.stop();
               steps += 1;
            }else if(recv[0]=='o'){
               car.goStraight(-100);
               ThisThread::sleep_for(100ms);
               car.stop();
               steps += 1;
            }else{
               car.stop();
            } 
            }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
      }
    //   if(sub_task==1 && steps==100){
    //       sub_task = 2;
    //       steps = 0;
    //       xbee.write("End: Follow the line", 21);
    //       xbee.write("Start: Circle around the object", 32);
    //   }
    //   else if(sub_task==2){
    //       car.turn(-50, 0.6);
    //       sub_task = 3;
    //       xbee.write("End: Circle around the object", 30);
    //       xbee.write("Start: Calibrate the location", 30);
    //   }
    //   else if(sub_task==3 && steps==100){
    //       xbee.write("End: Calibrate the location", 28);
    //   }
   }
}