/*
 * Copyright (c) 2022, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"


namespace {

#define PERIOD_MS 2000ms
}



DigitalOut myled(LED1);
// DigitalIn  mypin(BUTTON1);
InterruptIn button(BUTTON1);

using namespace std::chrono;

Timer t;
volatile int flag=0;

Ticker flipper;
volatile double f_tick=0.5;


void flip()
{
    myled = !myled;
}

void mystart(){
    myled = 1;
    t.reset();
    t.start();
    flag=0;
}

 void mystop(){
    myled = 0;
    t.stop();
    flag=1;
    
 }

 void incr_freq(){
    f_tick+=0.5;
    if(f_tick==3.5){f_tick=0.5;}
    flipper.detach();
    flipper.attach(&flip,f_tick);
    // printf("period=%.1f s\n\r",f_tick);

 }

int main()
{
    // while (true) {
    //     printf("Groupe 10! \n");
    //     ThisThread::sleep_for(PERIOD_MS / 2);
    // }

    // check that myled object is initialized and connected to a pin
    if (myled.is_connected()) {
        printf("myled is initialized and connected!\n\r");
    }

    // mypin.mode(PullNone);
    // int state = 0;
    // Blink LED
    // while (1) {
    //     // myled = 1;          // set LED1 pin to high
    //     // printf("myled = %d \n\r", (uint8_t)myled);
    //     // ThisThread::sleep_for(500);
        
      
    //     // state ^=1;
    //     // myled.write(state);    
    //     // printf("myled = %d \n\r", myled.read());
    //     // ThisThread::sleep_for(500);


    //     printf("mypin has value : %d \n\r", mypin.read());
    //     // myled = mypin; // toggle led based on value of button
    //     myled.write(mypin);  
    //     ThisThread::sleep_for(250);
    // }


    // button.rise(&flip);  // attach the address of the flip function to the rising edge
    // while (1) {          // wait around, interrupts will interrupt this!
    //     printf("no interrupt! \n");
    //     ThisThread::sleep_for(PERIOD_MS / 2);
    // }


    // button.rise(&mystart);  
    // button.fall(&mystop);
    // while (1) {          // wait around, interrupts will interrupt this!
    //     if(flag==1){
    //         printf("The time taken was %llu milliseconds\n", duration_cast<milliseconds>(t.elapsed_time()).count());
    //         flag=0;
    //     }
        
    // }


    button.rise(&incr_freq); 
    flipper.attach(&flip, f_tick); 

    // spin in a main loop. flipper will interrupt it to call flip
    while (1) {
        printf("myled = %d, F= %.1f \n\r", myled.read(), f_tick);
        ThisThread::sleep_for(500);
    }
}
