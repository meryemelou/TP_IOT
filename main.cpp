/*
 * Copyright (c) 2022, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "bme280.h"

using namespace sixtron;


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





I2C i2c(I2C1_SDA, I2C1_SCL);
BME280 bme(&i2c, BME280::I2CAddress::Address1);

const int addr7bit = 0x76;      
const int addr8bit = 0x76 << 1;




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


    // button.rise(&incr_freq); 
    // flipper.attach(&flip, f_tick); 

    // // spin in a main loop. flipper will interrupt it to call flip
    // while (1) {
    //     printf("myled = %d, F= %.1f \n\r", myled.read(), f_tick);
    //     ThisThread::sleep_for(500);
    // }



/***************  I2C *****************/
   
    // bme.initialize();
    // bme.set_sampling(
    //     BME280::SensorMode::NORMAL,
    //     BME280::SensorSampling::OVERSAMPLING_X16,
    //     BME280::SensorSampling::OVERSAMPLING_X16,
    //     BME280::SensorSampling::OVERSAMPLING_X16
    // );

    // while (true) {
    //     float temperature = bme.temperature(); 
    //     float pressure = bme.pressure(); 
    //     float humidity = bme.humidity(); 
    //     printf("T = %.2f C | P = %.2f Pa | H = %.2f % \n", temperature, pressure, humidity);
       

    //     ThisThread::sleep_for(1000ms);
    // }

    
    static char data[18];
    data[0] = 0x88;
    i2c.write(addr8bit, data, 1);
    i2c.read(addr8bit, data, 6);
    uint16_t calib_dig_T1 = (data[1] << 8) | data[0];
    uint16_t calib_dig_T2 = (data[3] << 8) | (data[2]);
    uint16_t calib_dig_T3 = (data[5] << 8) | (data[4]);

    uint8_t osrs_t = 0b101;
    
    char cmd;
    int32_t var1, var2, t_fine;
    char temp_data[3];
    while (1) {
        cmd= 0xfa;
        i2c.write(addr8bit, &cmd, 1, true);
        i2c.read(addr8bit, temp_data, 3, false);


        uint32_t raw_temp = ((temp_data[0] << 12) | (temp_data[1] << 4) | (temp_data[2] >> 4));
        raw_temp &= 0xFFFFF;



         var1 = ((((raw_temp >> 3) - ((int32_t) calib_dig_T1 << 1)))
                    * ((int32_t) calib_dig_T2)) >> 11;

         var2 = (((((raw_temp >> 4) - ((int32_t) calib_dig_T1))
                                    * ((raw_temp >> 4) - ((int32_t) calib_dig_T1))) >> 12)
                    * ((int32_t) calib_dig_T3)) >> 14;

        t_fine = var1 + var2;

        float T = (float)((t_fine * 5 + 128) >> 8) / 100.0;
        
        printf("T = %.2f C\n", T);
        ThisThread::sleep_for(1000ms);
    }
    
}
