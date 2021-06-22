#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "string.h"
DigitalInOut ping(D12);
Timer t;

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D10, D9);
BufferedSerial uart(D1,D0); //tx,rx

BBCar car(pin5, pin6, servo_ticker);
int sub_task;
int steps;
int ping_cunt=0;

int main() {
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   uart.set_baud(9600);
   sub_task = 2;
   steps = 0;
 
   sprintf(buf, "23");
   fputc(buf, devout);
   sprintf(buf, "Start: Follow the line");
   fputc(buf, devout);
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
            if(ping_cunt<10 && sub_task==3) ping_cunt++;
            else if(ping_cunt==10 && sub_task==3){
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
    //   if(sub_task==1 && steps==100){
    //       sub_task = 2;
    //       steps = 0;
    //       xbee.write("End: Follow the line", 21);
    //       xbee.write("Start: Circle around the object", 32);
    //   }
      else if(sub_task==2){
          //car.turn(-100, 0.7);
          ThisThread::sleep_for(6000ms);
          car.stop();
          sub_task = 5;
          xbee.write("End: Circle around the object", 30);
          xbee.write("Start: Calibrate the location", 30);
      }
    //   else if(sub_task==3 && steps==100){
    //       xbee.write("End: Calibrate the location", 28);
    //   }
   }
}