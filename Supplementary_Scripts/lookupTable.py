#!/usr/bin/env python3

import rospy                                                                            #this program is made to generate a lookup table in a header file for a C++ program
import array
import math

angle_bottom_limitPitch = -30                                                           # These values are the limits for the gimbal, they can be found on the gimbal configurator program when connected to the STorM32 with a mini USB cable
angle_top_limitPitch = 30                                                               # they should be changed so that there is no harm done to the motors

angle_bottom_limitRoll = -90                                                            #it is also important to remember to change the values on the C++ program "floatROS_gimbal_control" for the Arduino if they are changed in this program
angle_top_limitRoll = 90

angle_bottom_limitYaw = -90
angle_top_limitYaw = 90

res = 2                                                                                 #resolution. 1 = every one degree, 2 = every half degree, 10 = every 0.1 degrees... etc

######################################################################################  #in this part of the code it makes the actual array for the pitch, roll and the yaw. the code is essentially the same for the three different sections with only the names changing
#Pitch                                                                                  #Pitch
DegreeRangePitch = angle_top_limitPitch - angle_bottom_limitPitch
lookupArrayPitch = array.array('f', [])

mPitch = 1000 / DegreeRangePitch                                                        #m and b have been derived from PWM = m(degrees) + b, with the initial conditions of PWM when degrees = bottom_limit ---> 1000, and PWM when degrees = top_limit ---> 2000
bPitch = 1000 - (1000 * angle_bottom_limitPitch / DegreeRangePitch)

j = 0
i = angle_bottom_limitPitch
while i <= angle_top_limitPitch:                                                        #make lookup table
    PWM_equivalentPitch = (mPitch * i) + bPitch                                         #get the PWM equivalent to each degree that could be possibly inputted, taking into consideration the resolution
    lookupArrayPitch.append(PWM_equivalentPitch)                                        #putting into the array the PWM values, in order from bottom limit to top limit
    i += (1.0 / res)
    j += 1

######################################################################################
#Roll                                                                                   #Roll
DegreeRangeRoll = angle_top_limitRoll - angle_bottom_limitRoll
lookupArrayRoll = array.array('f', [])

mRoll = 1000 / DegreeRangeRoll
bRoll = 1000 - (1000 * angle_bottom_limitRoll / DegreeRangeRoll)

j = 0
i = angle_bottom_limitRoll
while i <= angle_top_limitRoll:
    PWM_equivalentRoll = (mRoll * i) + bRoll
    lookupArrayRoll.append(PWM_equivalentRoll)
    i += (1.0 / res)
    j += 1

######################################################################################
#Yaw                                                                                    #Yaw
DegreeRangeYaw = angle_top_limitYaw - angle_bottom_limitYaw
lookupArrayYaw = array.array('f', [])

mYaw = 1000 / DegreeRangeYaw
bYaw = 1000 - (1000 * angle_bottom_limitYaw / DegreeRangeYaw)

j = 0
i = angle_bottom_limitYaw
while i <= angle_top_limitYaw:
    PWM_equivalentYaw = (mYaw * i) + bYaw
    lookupArrayYaw.append(PWM_equivalentYaw)
    i += (1.0 / res)
    j += 1

######################################################################################
                                                                                        #in this last section of the code, an output is started, and all the arrays are put into it, to then be written into a header file for the C++ program that is uploaded into the arduino
output = ''                                                                             #it will all be in a string that we can then easily write into the header file. It is also important to have it as the correct format so that the program can be read properly

output += '''
#ifndef LOOKUP_HEADER
#define LOOKUP_HEADER 

PROGMEM const int lookUpTable_pitch[] = {
'''

for x in range(len(lookupArrayPitch)):                                                  #for all the elements on the arrays previously created, this for loop writes down the values with comas so that it is in the correct format for the C++ header file
    if not x == 0:
        output += ', '
    #print(lookupArrayPitch[x])
    output += str(int(round(lookupArrayPitch[x])))

output += '''
};

PROGMEM const int lookUpTable_roll[] = {
'''

for x in range(len(lookupArrayRoll)):
    if not x == 0:
        output += ', '
    output += str(int(round(lookupArrayRoll[x])))

output += '''
};

PROGMEM const int lookUpTable_yaw[] = {
'''

for x in range(len(lookupArrayYaw)):
    if not x == 0:
        output += ', '
    output += str(int(round(lookupArrayYaw[x])))

output += '''
};

#endif // LOOKUP_HEADERS
'''

# Save the output as lookuptable.h

headerFileName = "/home/rwl/Arduino/libraries/lookuptable/lookuptable.h"

file = open(headerFileName,'w')

file.write(output)
file.close()

#used this: https://github.com/StephenCarlson/BYU-Mars-Rover/blob/master/RoverLinkGenerator.py