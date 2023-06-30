#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64MultiArray.h>
//#include <string>
//#include <vector>
//#include <iostream>
#include <Servo.h>
#include <stdlib.h>

Servo motorPitch;
Servo motorRoll;
Servo motorYaw;
Servo lock;

ros::NodeHandle nh;
//                                                                                                     Hardcoded angle limits for the gimbal, and resolution for inputs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int angle_bottom_limitPitch = -90;
int angle_top_limitPitch = 90;

int angle_bottom_limitRoll = -25;
int angle_top_limitRoll = 25;

int angle_bottom_limitYaw = -90;
int angle_top_limitYaw = 90;

int res = 1;                                                                                      //resolution. 1=every one degree, 2=every half degree, 10=every 0.1 degrees... etc

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float DegreeRangePitch = angle_top_limitPitch - angle_bottom_limitPitch;                            // sets the range (top - bottom), as this will be used in a couple ocassions
double* lookupArrayPitch = (double*)malloc((res * DegreeRangePitch + 1) * sizeof(double));                 //problem. by having this memory alocation from the beginning, when the range is too great, too much memory is allocated and the arduino cannot handle the math to calculate the PWM equivalent for the degree values. the math simply gives out wrong answers...

int *test_array=(int*)malloc((300)*sizeof(int));

//int DegreeRangeRoll = angle_top_limitRoll - angle_bottom_limitRoll;
//int lookupArrayRoll[res * DegreeRangeRoll + 1];
//float mRoll = (1000) / (DegreeRangeRoll);
//float bRoll = 1000 - ((1000 * angle_bottom_limitRoll) / DegreeRangeRoll);
//int x = 0;
//for (float i = angle_bottom_limitRoll; i <= angle_top_limitRoll; i += (1.0 / res), x++)   
//{
//  float PWM_equivalentRoll = (mRoll * i) + bRoll;
//  lookupArrayRoll[x] = PWM_equivalentRoll;
//}
//
//int DegreeRangeYaw = angle_top_limitYaw - angle_bottom_limitYaw;
//int lookupArrayYaw[res * DegreeRangeYaw + 1];
//float mYaw = (1000) / (DegreeRangeYaw);
//float bYaw = 1000 - ((1000 * angle_bottom_limitYaw) / DegreeRangeYaw);
//int y = 0;
//for (float i = angle_bottom_limitYaw; i <= angle_top_limitYaw; i += (1.0 / res), y++)  
//{
//  float PWM_equivalentYaw = (mYaw * i) + bYaw;
//  lookupArrayYaw[y] = PWM_equivalentYaw;
//}

void Callback(const std_msgs::Float64MultiArray& InputNumber)                                     //this function will be called when there is anything published to the topic: "input_topic"                         
{                                 
  //following if input is in pwm (microseconds)
  //motorPitch.writeMicroseconds(InputNumber.data[0]);                                            //move the motor to the position inputted in microseconds (PWM signal)
  //motorRoll.writeMicroseconds(InputNumber.data[1]);
  //motorYaw.writeMicroseconds(InputNumber.data[2]);


  
                                                                                                  //following if input is in degrees
  int degreePitch = round(InputNumber.data[0] - angle_bottom_limitPitch) * res;                   //by subtracting the desired angle by the bottom limit and multiplying that by the resolution, you'll get the number where you can the PWM in the array for the desired degrees (inputted)

  //Serial.println(lookupArrayPitch[0]);

  //  if (lookupArrayPitch[0] == 1000)
  //  {
  //    digitalWrite(LED_BUILTIN, 1);
  //  }
  //motorPitch.writeMicroseconds(lookupArrayPitch[degreePitch]);
}

ros::Subscriber<std_msgs::Float64MultiArray> sub("input_topic", Callback);                        //subscribe to topic called "input_topic" and call Callback function

void setup()
{
  nh.initNode();
  nh.subscribe(sub);
  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);

  motorPitch.attach(11);                                                                          //these lines assign a pin to each dimension. The PWM signal will come out of the respective pins, (11, 10, 9 or 8) and will go to the motors in the gimbal                  
  motorRoll.attach(10);
  motorYaw.attach(9);
  lock.attach(6);

  Serial.begin(115200);//(9600);
 

  double mPitch = 1000L/DegreeRangePitch;                                                     // m and b have been derived from PWM = m(degrees) + b, with the initial conditions of PWM when degrees = bottom_limit ---> 1000, and PWM when degrees = top_limit ---> 2000  
  //Serial.println(mPitch);
  float bPitch = 1000L - (1000L * angle_bottom_limitPitch/DegreeRangePitch);
  //Serial.println(bPitch);
  int j = 0;
//  lookupArrayPitch = new double[180];
  for (double i = angle_bottom_limitPitch; i <= angle_top_limitPitch; i += (1.0 / res))       //make lookup table
  {
    double PWM_equivalentPitch = (mPitch * i) + bPitch;                                            //get the PWM equivalent to each degree that could be possibly inputted, taking into consideration the resolution

//    lookupArrayPitch[j] = PWM_equivalentPitch;                                                    //putting into the array the PWM values, in order from bottom limit to top limit

    j++;
    Serial.println(PWM_equivalentPitch);
    
    delay(500);
  }
//  free(lookupArrayPitch);
  //Serial.println(" ");

//  for (int k=0; k<500; k++)
//  {
//    Serial.println(k);
//    delay(100); 
//  }



  //  int DegreeRangePitch = angle_top_limitPitch - angle_bottom_limitPitch;
  //  int lookupArrayPitch[res * DegreeRangePitch + 1];
  //  float mPitch = (1000) / (DegreeRangePitch);
  //  float bPitch = 1000 - ((1000 * angle_bottom_limitPitch) / DegreeRangePitch);
  //  int j = 0;
  //  for (float i = angle_bottom_limitPitch; i <= angle_top_limitPitch; i += (1.0 / res), j++)   //make lookup table
  //  {
  //    float PWM_equivalentPitch = (mPitch * i) + bPitch;
  //    lookupArrayPitch[j] = PWM_equivalentPitch;
  //  }
  //
  //  int DegreeRangeRoll = angle_top_limitRoll - angle_bottom_limitRoll;
  //  int lookupArrayRoll[res * DegreeRangeRoll + 1];
  //  float mRoll = (1000) / (DegreeRangeRoll);
  //  float bRoll = 1000 - ((1000 * angle_bottom_limitRoll) / DegreeRangeRoll);
  //  int x = 0;
  //  for (float i = angle_bottom_limitRoll; i <= angle_top_limitRoll; i += (1.0 / res), x++)     //make lookup table
  //  {
  //    float PWM_equivalentRoll = (mRoll * i) + bRoll;
  //    lookupArrayRoll[x] = PWM_equivalentRoll;
  //  }
  //
  //  int DegreeRangeYaw = angle_top_limitYaw - angle_bottom_limitYaw;
  //  int lookupArrayYaw[res * DegreeRangeYaw + 1];
  //  float mYaw = (1000) / (DegreeRangeYaw);
  //  float bYaw = 1000 - ((1000 * angle_bottom_limitYaw) / DegreeRangeYaw);
  //  int y = 0;
  //  for (float i = angle_bottom_limitYaw; i <= angle_top_limitYaw; i += (1.0 / res), y++)       //make lookup table
  //  {
  //    float PWM_equivalentYaw = (mYaw * i) + bYaw;
  //    lookupArrayYaw[y] = PWM_equivalentYaw;
  //  }
}

void loop()
{
  nh.spinOnce();
  //delay(10);
}
