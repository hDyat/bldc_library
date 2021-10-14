/* The sequences of control signals of bldc motor based on hall effect sensor position
 *          
 * step  hall_effect  HA  LA  HB  LB  HC  LC    P1  P2  P3
 *  1           101   1   0   0   1   0   0     +   -   NC
 *  2           100   1   0   0   0   0   1     +   NC  -
 *  3           110   0   0   1   0   0   1     NC  +   -
 *  4           010   0   1   1   0   0   0     -   +   NC
 *  5           011   0   1   0   0   1   0     -   NC  +
 *  6           001   0   0   0   1   1   0     NC  -   +
*/

#include "arduino.h"
#include "BLDC.h"

BLDC::BLDC(int hall1, int hall2, int hall3, 
                      int HA, int LA, int HB, 
                      int LB, int HC, int LC)
{
    this->pin_hall_1 = hall1;
    this->pin_hall_2 = hall2;
    this->pin_hall_3 = hall3;
    
    this->pin_HA = HA;
    this->pin_HB = HB;
    this->pin_HC = HC;

    this->pin_LA = LA;
    this->pin_LB = LB;
    this->pin_LC = LC;

    this->hall_sensor_1 = 0;
    this->hall_sensor_2 = 0;
    this->hall_sensor_3 = 0;
    
    this->speed_rpm = 0;

    pinMode(pin_hall_1, INPUT);
    pinMode(pin_hall_2, INPUT);
    pinMode(pin_hall_3, INPUT);

}

void BLDC::setHall()
{
    this->hall_sensor_1 = digitalRead(this->pin_hall_1);
    this->hall_sensor_2 = digitalRead(this->pin_hall_2);
    this->hall_sensor_3 = digitalRead(this->pin_hall_3);
}

String BLDC::getHall()
{
    BLDC::setHall();
    String hall1_value = String(this->hall_sensor_1);
    String hall2_value = String(this->hall_sensor_2);
    String hall3_value = String(this->hall_sensor_3);

    return hall1_value + hall_sensor_2 + hall_sensor_3;
}

void BLDC::hallDecoder(void)
{
    BLDC::setHall();
    //                       HA,LA,HB,LB,HC,LC 
//  int six_commutation[] = {0, 0, 0, 0, 0, 0};

    if(this->hall_sensor_1 == HIGH && this->hall_sensor_2 == LOW && this->hall_sensor_3 == HIGH)
    {
        this->highA = 1;
        this->lowA = 0;
        this->highB = 0;
        this->lowB = 1;
        this->highC = 0;
        this->lowC = 0;
    }else if(this->hall_sensor_1 == HIGH && this->hall_sensor_2 == LOW && this->hall_sensor_3 == LOW)
    {
        this->highA = 1;
        this->lowA = 0;
        this->highB = 0;
        this->lowB = 0;
        this->highC = 0;
        this->lowC = 1;
    }
        
    else if(this->hall_sensor_1 == HIGH && this->hall_sensor_2 == HIGH && this->hall_sensor_3 == LOW)
    {
        this->highA = 0;
        this->lowA = 0;
        this->highB = 1;
        this->lowB = 0;
        this->highC = 0;
        this->lowC = 1;
    }
        
    else if(this->hall_sensor_1 == LOW && this->hall_sensor_2 == HIGH && this->hall_sensor_3 == LOW)
    {
        this->highA = 0;
        this->lowA = 1;
        this->highB = 1;
        this->lowB = 0;
        this->highC = 0;
        this->lowC = 0;
    }
       
    else if(this->hall_sensor_1 == LOW && this->hall_sensor_2 == HIGH && this->hall_sensor_3 == HIGH)
    {
        this->highA = 0;
        this->lowA = 1;
        this->highB = 0;
        this->lowB = 0;
        this->highC = 1;
        this->lowC = 0;
    }
        
    else if(this->hall_sensor_1 == LOW && this->hall_sensor_2 == LOW && this->hall_sensor_3 == HIGH)
    {
        this->highA = 0;
        this->lowA = 0;
        this->highB = 0;
        this->lowB = 1;
        this->highC = 1;
        this->lowC = 0;
    }
    else 
    {
        this->highA = 0;
        this->lowA = 0;
        this->highB = 0;
        this->lowB = 0;
        this->highC = 0;
        this->lowC = 0;
    } 
}

int BLDC::setDutyCycles(int val)
{
    int mappedSensorValue = map(val, 0, 1023, 0, 255);
    return mappedSensorValue;
}

int BLDC::getDutyCycles(int val)
{
    return map(BLDC::setDutyCycles(val), 0, 255, 0 , 100);
}

void BLDC::controller(int ref)
{
    int dutyCycles = BLDC::setDutyCycles(ref);
    BLDC::hallDecoder();

    if(this->highA)
        analogWrite(pin_HA, dutyCycles);
    else
        analogWrite(pin_HA, 0);
    
    if(this->highB)
        analogWrite(pin_HB, dutyCycles);
    else
        analogWrite(pin_HB, 0);
    
    if(this->highC)
        analogWrite(pin_HC, dutyCycles);
    else
        analogWrite(pin_HC, 0);

    digitalWrite(pin_LA, this->lowA);
    digitalWrite(pin_LB, this->lowB);
    digitalWrite(pin_LC, this->lowC);
}

int BLDC::version(void) 
{
    return 1;
}



