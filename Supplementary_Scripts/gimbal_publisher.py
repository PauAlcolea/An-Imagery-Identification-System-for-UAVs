#!/usr/bin/env python3

import rospy
import time
#from std_msgs.msg import UInt64MultiArray
from geometry_msgs.msg import Point32



def gimbal_publisher(a, b, c):                                                      #this function will take no argumentgs, it will publish an UInt64MultiArray
    rospy.init_node('gimbal_publisher')                                             #sets up a node called gimbal_publisher

    pub = rospy.Publisher('input_topic', Point32, queue_size=10)           #sets up a topic called "input_topic" to whitch it will publish messages of type UInt64MultiArray
                                                                                        #the program ROS_gimbal_control on the Teensy 4.0 is subscribed to the topic declared above
                                                                                        #that program takes an array with thee values (pitch, roll, yaw) and moves the motors to those positions 
    rate = rospy.Rate(10)                                                           # 10hz

    #while not rospy.is_shutdown():                                                 #for as long as it is not ^C
    array_output = Point32()
    array_output.x = a
    array_output.y = b
    array_output.z = c                                                               #publish these hardcoded "position" for the motors
    pub.publish(array_output)
       


if __name__== '__main__':
    try:
        gimbal_publisher(0, 0, 0)
        gimbal_publisher(0,0,-90)
        time.sleep(2)
        j = -90
        for i in range(88):
            gimbal_publisher(0,0,j)
            time.sleep(0.5)
            j += 2
       
        gimbal_publisher(0,0,0)
        time.sleep(1)
        gimbal_publisher(90,0,0)

        k = 90
        for x in range(65):
            gimbal_publisher(k,0,0)
            time.sleep(0.5)
            k -= 2

        gimbal_publisher(0,0,0)
        time.sleep(1)
        gimbal_publisher(0,-25,0)

        w = -25
        for y in range(25):
            gimbal_publisher(0,w,0)
            time.sleep(0.5)
            w += 2

        gimbal_publisher(0,0,0)
        # time.sleep(2)
        # gimbal_publisher(-40, 10, -70)
        time.sleep(2)
        gimbal_publisher(20, -25, 45)
        time.sleep(2)
        gimbal_publisher(90, -10, 40)
        time.sleep(2)
        gimbal_publisher(0, 25, -40)
        time.sleep(2)
        gimbal_publisher(0,0,0)

         

    except rospy.ROSInterruptException:
        pass