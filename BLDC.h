/*
 * BLDC.h - Library for controlling BLDC motor.
 * Created by HidayatN, September 29, 2021.
 * for project motor controller funded by Kedaireka
 * 
 * The sequence of control signals of bldc motor based on hall effect sensor
 *          
 * step  hall_effect  HA  LA  HB  LB  HC  LC    P1  P2  P3
 *  1           101   1   0   0   1   0   0     +   -   NC
 *  2           100   1   0   0   0   0   1     +   NC  -
 *  3           110   0   0   1   0   0   1     NC  +   -
 *  4           010   0   1   1   0   0   0     -   +   NC
 *  5           011   0   1   0   0   1   0     -   NC  +
 *  6           001   0   0   0   1   1   0     NC  -   +       
*/

#ifndef BLDC_h
#define BLDC_h

#include "Arduino.h"

class BLDC 
{
    public:
        BLDC(int hall1, 
            int hall2, 
            int hall3, 
            int HA, int LA, 
            int HB, int LB,
            int HC, int LC);

        void controller(int ref); 

        //display on serial print monitor only (verification)
        int getDutyCycles(int val);        
        int getSpeed(void);
        int version(void);          //get version of the library code
        String getHall(void); 
    

    private:
        //pin input (hall effect sensor)
        int pin_hall_1;
        int pin_hall_2;
        int pin_hall_3;

        //pin high side pwm output
        int pin_HA;
        int pin_HB;
        int pin_HC;

        //pin low side digital output
        int pin_LA;
        int pin_LB;
        int pin_LC;

        //variable of hall effect sensor
        int hall_sensor_1 = 0;
        int hall_sensor_2 = 0;
        int hall_sensor_3 = 0;

        //veriable to hold the value of output
        int highA = 0;
        int lowA = 0;
        int highB = 0;
        int lowB = 0;
        int highC = 0;
        int lowC = 0;

        int speed_rpm;              //rpm calculator

        void setHall(void);         //get hall effect sensor changed
        int speedmeasurement(void); //calculation of bldc speed
        int setDutyCycles(int val);        //get duty cycles in range 0 - 255 (0 - 100%)
        
        void hallDecoder(void);    //decoder hall effect sensor
};

#endif
