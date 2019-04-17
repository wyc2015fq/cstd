# 四线八拍步进电机（28BYJ48）Arduino 库stepper - V__KING__的专栏 - CSDN博客





2014年01月13日 15:19:59[v__king__](https://me.csdn.net/V__KING__)阅读数：4062








```
/* 
long step_delay=0;
long last_step_time=0;
int step_number=0;

int Pin0=8,Pin1=9,Pin2=10,Pin3=11;
int number_of_steps=64;	//1:64
*/


/******************/
#include<my_stepper.h>
My_stepper stepper(8,9,10,11);
void setup()
{
  	Serial.begin(9600);
	stepper.my_setSpeed(500);
}

void loop()
{
  stepper.my_step(64*64/2,-1);//64*64 step == 2*PI
  delay(2000);
}
```

下面是些的库



---------------------------------------------------------------------------------------------------

//my_stepper.cpp

```
/*
 *       Filename:  my_stepper.h
 *
 *    Description:     八拍方式步进电机
 *
 *        Version:  1.0
 *        Created:  2014-1-13 11:16:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (V__KING__),
 *        Company:     sogworks
 *
 * =====================================================================================
 */
#ifndef MY_STEPPR_H
#define MY_STEPPR_H



class My_stepper{
    public:
        My_stepper(int p1,int p2,int p3,int p4);
        void my_stepMotor(int thisStep);
        void my_setSpeed(long whatSpeed);
        void my_step(int steps_to_move,int direction);

    private:
        int whatSpeed;            //set speed
        long step_delay;        //it relate to speed
        long last_step_time;
        int step_number;
        int number_of_steps;    //1:64
        int Pin0,Pin1,Pin2,Pin3;
};





#endif
```



---------------------------------------------------------------------------------------------------

//my_stepper.cpp



```
/*
 *       Filename:  my_stepper.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014-1-13 11:40:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (V__KING__), 
 *        Company: 	sogworks 
 *
 * =====================================================================================
 */

#include"my_stepper.h"
#include"Arduino.h"
My_stepper::My_stepper(int p1,int p2,int p3,int p4)
{
	this->Pin0=p1;	
	this->Pin1=p2;
	this->Pin2=p3;
	this->Pin3=p4;
	pinMode(this->Pin0,OUTPUT);
	pinMode(this->Pin1,OUTPUT);
	pinMode(this->Pin2,OUTPUT);
	pinMode(this->Pin3,OUTPUT);
	
}
void My_stepper::my_stepMotor(int thisStep)
{
	switch (thisStep) {
		case 0:
			digitalWrite(this->Pin0, LOW);
			digitalWrite(this->Pin1, LOW);
			digitalWrite(this->Pin2, LOW);
			digitalWrite(this->Pin3, HIGH);
			break;
		case 1:
			digitalWrite(this->Pin0, LOW);
			digitalWrite(this->Pin1, LOW);
			digitalWrite(this->Pin2, HIGH);
			digitalWrite(this->Pin3, HIGH);
			break;
		case 2:
			digitalWrite(this->Pin0, LOW);
			digitalWrite(this->Pin1, LOW);
			digitalWrite(this->Pin2, HIGH);
			digitalWrite(this->Pin3, LOW);
			break;
		case 3:
			digitalWrite(this->Pin0, LOW);
			digitalWrite(this->Pin1, HIGH);
			digitalWrite(this->Pin2, HIGH);
			digitalWrite(this->Pin3, LOW);
			break;
		case 4:
			digitalWrite(this->Pin0, LOW);
			digitalWrite(this->Pin1, HIGH);
			digitalWrite(this->Pin2, LOW);
			digitalWrite(this->Pin3, LOW);
			break;
		case 5:
			digitalWrite(this->Pin0, HIGH);
			digitalWrite(this->Pin1, HIGH);
			digitalWrite(this->Pin2, LOW);
			digitalWrite(this->Pin3, LOW);
			break;
		case 6:
			digitalWrite(this->Pin0, HIGH);
			digitalWrite(this->Pin1, LOW);
			digitalWrite(this->Pin2, LOW);
			digitalWrite(this->Pin3, LOW);
			break;
		case 7:
			digitalWrite(this->Pin0, HIGH);
			digitalWrite(this->Pin1, LOW);
			digitalWrite(this->Pin2, LOW);
			digitalWrite(this->Pin3, HIGH);
			break;
		default:
			digitalWrite(this->Pin0, LOW);
			digitalWrite(this->Pin1, LOW);
			digitalWrite(this->Pin2, LOW);
			digitalWrite(this->Pin3, LOW);
			break;
	} 
}

void My_stepper::my_setSpeed(long whatSpeed)
{
 	this->number_of_steps=64;
  this->step_delay = 60L * 1000L / this->number_of_steps / whatSpeed;
}


void My_stepper::my_step(int steps_to_move,int direction)
{  
	int steps_left = abs(steps_to_move);  // how many steps to take

	// decrement the number of steps, moving one step each time:
	while(steps_left > 0) {
		// move only if the appropriate delay has passed:examples
		if (millis() - this->last_step_time >= this->step_delay) {
			// get the timeStamp of when you stepped:
			this->last_step_time = millis();
			// increment or decrement the step number,
			// depending on direction:
			if (direction == 1) {
				this->step_number++;
				if (this->step_number == this->number_of_steps) {
					this->step_number = 0;
				}
			} 
			else if(direction==-1){
				if (this->step_number == 0) {
					this->step_number = this->number_of_steps;
				}
				this->step_number--;
				Serial.println(this->step_number);
			}
			// decrement the steps left:
			steps_left--;
			// step the motor to step number 0, 1, 2, or 3:
			my_stepMotor(this->step_number % 8);
		}
	}
}
```








