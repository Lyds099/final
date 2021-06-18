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

int main() {
   uart.set_baud(9600);

   char buf[256], outbuf[256];
   FILE *devin_uart = fdopen(&uart, "r");
   FILE *devout_uart = fdopen(&uart, "w");
   //xbee.write("ATMY <Remote MY>\r\n", 12);
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   printf("%s\n", buf);
   RPC::call(buf, outbuf);
   printf("%s\r\n", outbuf);
   }
}