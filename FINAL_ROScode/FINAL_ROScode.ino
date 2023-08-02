//This program is written for Arduino Nano
#define USE_USBCON
#include <ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Point32.h>
#include <Servo.h>
#include <avr/pgmspace.h>
#include <lookuptable.h>
//#include <std_msgs/Float64MultiArray.h>
//#include <std_msgs/Int16MultiArray.h>

Servo motorPitch;
Servo motorRoll;
Servo motorYaw;
Servo lock;

ros::NodeHandle nh;
                                                                                                    //Hardcoded angle limits for the gimbal, and resolution for inputs. These are not truly the limits
                                                                                                    //these should be "symmetric", if not, you may change the values for the constrain function in line 41. Those are the actual limits for the pitch
                                                                                                    //if changed here, they must also be changed in lookupTable.py
                                                                                                    //should also change them in the callback function in the contrain function
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int angle_bottom_limitPitch = -90;
int angle_top_limitPitch = 90;

int angle_bottom_limitRoll = -33;
int angle_top_limitRoll = 33;

int angle_bottom_limitYaw = -90;
int angle_top_limitYaw = 90;

int res = 2;                                                                                        //resolution. 1 = every one degree, 2 = every half degree, 10 = every 0.1 degrees... etc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//void Callback(const std_msgs::Float64MultiArray& InputNumber)                                     //this function will be called when there is anything published to the topic: "input_topic"
//void Callback(const std_msgs::Int16MultiArray& InputNumber)                                       //it is expected that the user publishes a message of the style: rostopic pub input_topic geometry_msgs/Point32 {"x: 0.0, y: 0.0, z: 0.0"}
void Callback(const geometry_msgs::Point32& InputNumber)                                            //the user can input decimals, but they should take into consideration the resolution, i.e. not input 50.3 if the resolution is 2.
                                                                                                    //the inputted degrees must also be within the range set on the top of this program and in lookupTable.py for the program to work
{
  //following if input is in degrees
  float inputtedPitch = InputNumber.x;                                                              //the constrain function takes a number that is too high for the physical limits of the gimbal and brings it down to the actual limit. Those must be set manually
  int degreePitch = round((constrain(inputtedPitch, -40, 90) - angle_bottom_limitPitch) * res);     //by subtracting the desired angle by the bottom limit and multiplying that by the resolution, you'll get the number where you can the PWM in the array for the desired degrees (inputted)
  int value_pitch = pgm_read_word(&lookUpTable_pitch[degreePitch]);
  motorPitch.writeMicroseconds(value_pitch);

  float inputtedRoll = InputNumber.y;
  int degreeRoll = round((constrain(inputtedRoll, -33, 33) - angle_bottom_limitRoll) * res);
  int value_roll = pgm_read_word(&lookUpTable_roll[degreeRoll]);
  motorRoll.writeMicroseconds(value_roll);


  float inputtedYaw = InputNumber.z;
  int degreeYaw = round((constrain(inputtedYaw, -30, 30) - angle_bottom_limitYaw) * res);
  int value_yaw = pgm_read_word(&lookUpTable_yaw[degreeYaw]);
  motorYaw.writeMicroseconds(value_yaw);
}

//ros::Subscriber<std_msgs::Float64MultiArray> sub("input_topic", Callback);                        //subscribe to topic called "input_topic" and call Callback function
//ros::Subscriber<std_msgs::Int16MultiArray> sub("input_topic", Callback);
ros::Subscriber<geometry_msgs::Point32> sub("input_topic", Callback);

void setup()
{
  nh.initNode();                                                                                    //initializing necessary for rosserial
  nh.subscribe(sub);
//  pinMode(13, OUTPUT);
//
//  pinMode(11, OUTPUT);
//  pinMode(10, OUTPUT);
//  pinMode(9, OUTPUT);
//  pinMode(6, OUTPUT);

  motorPitch.attach(11);                                                                            //these lines assign a pin to each dimension/axis. The PWM signal will come out of the respective pins, (11, 10, 9 or 6) and will go to the motors in the gimbal
  motorRoll.attach(10);                                                                             //they use the servo library for arduino
  motorYaw.attach(9);
  lock.attach(6);
}

void loop()
{
  nh.spinOnce();
  delay(10);
}
                                                                                                    //to operate this through terminal, start "roscore" in a terminal
                                                                                                    //open this program with "rosrun rosserial_arduino serial_node.py /dev/ttyACM1", ttyACM1 is in my case the port to whitch the arduino is connected
                                                                                                    //one can then publish in the input topic with: "rostopic pub input_topic geometry_msgs/Point32 {"x: 0.0, y: 0.0, z: 0.0"}"
