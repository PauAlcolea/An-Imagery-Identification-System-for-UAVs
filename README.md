![NEWdesign5](https://github.com/PauAlcolea/An-Imagery-Identification-System-for-UAVs/assets/86881699/250217db-9c91-4e06-a44b-a05e0792b2a9)
# Identification System for UAVs <a name="head-brief"></a>
**Pau Alcolea Vila** Worcester Polytechnic Institute, Worcester MA \
RoboWork Lab, University of Nevada Reno, NV  
&nbsp;  
&nbsp;  
The following instructions are meant to describe the process I took to make an identification system that can be integrated into Unmanned Aerial Vehicles. The use of this system will be to incorporate it to a DJI M600 Pro drone, tasked with mapping and surveying a brush landscape. The system will be responsible for the imaging of the sage bushes, providing an external software with clear images of the leaves so that the software might identify the species. This will be used for wildfire prevention through better understanding of the environment in question.

The final product will consist of a 3-axis gimbal with a high-zoom camera. An Arduino Micro (or any other similar microcontroller), must be connected to the gimbal via PWM signal wires.. The goal is for an outside system or software to publish the desired camera direction to a ROS topic, which the arduino node will subscribe to and read, and command the gimbal to move to the position where such direction from the camera is achieved.

This whole project requires a lot of different parts that together come to create the final product, and this repository includes all the different components and scripts necessary.
&nbsp;  
&nbsp;  
#### Table of Contents
0. [Introduction](#head-brief)  
1. [Acquiring materials](#acquiring-materials)
2. [Setting up the Gimbal](#setting-up-the-gimbal)
3. [Setting up the Arduino Board](#set-up-arduino)
4. [Supplementary Scripts](#sup-scripts)
5. [Mechanical](#mech)  
&nbsp;  
&nbsp;  
&nbsp;  
## 1. Acquiring Materials <a name="acquiring-materials"></a>
The first step is to acquire the necessary materials: 

The gimbal should be a 3-axis gimbal, providing freedom of motion to reach all the desired positions. The product that was used for this project can be found by going to the following link: [Gimbal](https://www.amazon.com/Brushless-Storm32-Controlller-Runcam3-Walkera/dp/B0C3X6HKF1/ref=sr_1_3?crid=2KMZRJ4MOXB49&keywords=storm32+gimbal&qid=1686785746&sprefix=storm32+gimbal%2Caps%2C125&sr=8-3&ufe=app_do%3Aamzn1.fos.006c50ae-5d4c-4777-9bc0-4513d670b6bc)

The next important component is the microcontroller. This will control the gimbal and determine the actual PWM signals required for each movement of the motors on the gimbal. In the initial stages of the project, we used a [Teensy 4.0](https://www.pjrc.com/store/teensy40.html). This microcontroller was capable enough for our intended use, but ended up not being a viable solution after back EMF from the motors fried it. With this we learnt that the microcontroller should operate in 5V to prevent an incident like this from occurring again. Some examples of 5V tolerant microcontrollers are the [Arduino Nano](https://docs.arduino.cc/hardware/nano) and [Arduino Micro](https://docs.arduino.cc/hardware/micro). Which are still small in design and capable of performing the desired task. 

On the camera/imaging side of the system, we used a [BlackFly Flir Camera](https://www.edmundoptics.com/p/bfs-u3-16s2c-cs-usb3-blackflyreg-s-color-camera/40164/), which will be connected to the on-board computer on the drone via a USB-3 cable. The lens on the camera of choice must have a very small FoV, preferably under around 7°, so that it has the capability to look at the small details, which I will need according to my application. For other applications, such a small Field of View might not be necessary.
&nbsp;  
&nbsp;  
&nbsp;  
## 2. Setting up the Gimbal <a name="setting-up-the-gimbal"></a>
After all the materials had been gathered/ordered, I started by setting up the gimbal. 

2.1. On a Windows computer install the gimbal GUI. For my specific gimbal, I installed the GUI from the following repository on GitHub: [https://github.com/olliw42/storm32bgc](https://github.com/olliw42/storm32bgc). 
I installed version v090, as that is what was already on the gimbal. If you wish to install a different version, you must flash the firmware on the gimbal. I found that the version already installed on my gimbal was sufficient for the adjustments I had to make. 
After cloning the repository, the .exe file to boot up the GUI can be found in the zip file for v090 found in the path: storm32bc/firmware binaries & gui/

2.2. The next step is to calibrate the gimbal. Attach the camera to the gimbal, and go to the “Gimbal Configuration” tab on the GUI. By clicking on the “Gimbal Configure” button and following the instructions that will appear on the screen, you will have calibrated your gimbal.

2.3. I recommend you keep the default PID values, as it can overly complicate the setup process.
</br>  
![Capture](https://github.com/PauAlcolea/An-Imagery-Identification-System-for-UAVs/assets/86881699/b6600b91-3787-466e-a01d-39b382dc1610)
<div align="center"> Your PID tab should look something like this. </div>
</br>

2.4. To actually increase the speed of the gimbal and some of the other more manageable settings, you can head to the Rc Inputs tab. For Pitch, Roll and Yaw, you may change the “Speed Limit” and the “Accel Limit”. Increasing the speed limit makes the motors faster and decreasing the acceleration limit makes them accelerate faster.
(We tuned our gimbal to max speed limit: 100°/s and minimum accel limit: 0.01, for all three motor orientations.)
This worked properly, but the faster speeds slightly take away from the stability. </br>
</br>
![Capture2](https://github.com/PauAlcolea/An-Imagery-Identification-System-for-UAVs/assets/86881699/97669244-1226-4c17-9d46-eed6391fe2ba)
</br>
2.5. By changing the limits for the Yaw from -90/90 to 0/180, you can also achieve a 90° phase turn, which can be beneficial by creating more space for the camera, and the cable while bringing the center of mass of the camera/lens closer to the axis of rotation. This was necessary for us, as the lens we used was substantially heavy and put the whole gimbal off balance with the original orientation.
&nbsp;  
&nbsp;  
&nbsp;  
## 3. Setting up the Arduino Board <a name="set-up-arduino"></a>
For the Arduino to be able to connect to other parts of the UAV, we must use Rosserial. Rosserial allows you to create a ROS node on the arduino, with which another device can communicate through ROS. To install this tool, follow the detailed instructions provided in ROS.org: [http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup](http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup)

You must also download the arduino IDE on your machine, which will allow you to connect to your board and upload scripts to it. This can be downloaded from the Arduino website: [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software). I downloaded the Legacy IDE (1.8.19) due to compatibility issues with Rosserial at the time of my project.

On this repository, the arduino program is called FINAL_ROScode.ino, (found in the folder with the same name). There are more detailed comments on the code itself, but the main idea is that the code subscribes to the topic “input_topic”, from which it gets a Point32 message, containing three values. With these angle values, it looks at the lookup table and determines the PWM signal value that must be outputted to the gimbal to move the motors to those degrees.
In terms of PWM signals, 1000 is the maximum to the left and 2000 the maximum to the right, this corresponds to the following degree angles (estimation): 
- Pitch:  -90° to 90°
- Roll: -33° to 33°
- Yaw: -90° to 90°

These different angles can be changed on the gimbal GUI and must also be changed on the Arduino program and the auxiliary scripts.
&nbsp;  
&nbsp;  
&nbsp;  
## 4. Supplementary Scripts <a name="sup-scripts"></a>
In addition to the arduino script, there are a couple of scripts that are necessary (or helpful )in this project. 

4.1. The first of these is called lookupTable.py This goal for this program is to create the lookup table that the arduino script references. As with the previous program, there are detailed comments in the code to further understand what the program does, but basically, its task is to generate a header file that contains three arrays. Each array contains the PWM values for all the angles in the range determined by the user for all three dimensions.
This is achieved by using a linear extrapolation and mapping all the angles to the PWM range signal: 1000-2000.
This also takes into consideration the desired resolution, which allows the user to input degrees to decimal places.
This program should be ran before uploading the arduino script. Making sure that it saves the header file it creates to the same location that the arduino program is taking it from. The location of this should under a folder called lookuptable in the libraries folder inside the Arduino directory that is created when you download the Arduino software.

4.2. The second program: gimbal_publisher.py, is used to publish a series of Point32 messages to the topic:  “input_topic”. This is later read by the arduino

Another way to move the gimbal, once the gimbal and Arduino are connected to a power source, is through the terminal. From the computer, run the lookupTable.py program, which will regenerate the lookup table and make it accessible to the arduino program. Then, you may upload the arduino script through the IDE by hitting Upload. </br> 

<img width="484" alt="Screenshot 2023-07-31 at 14 43 37" src="https://github.com/PauAlcolea/An-Imagery-Identification-System-for-UAVs/assets/86881699/0871e00b-8327-41f3-b3c4-71566d614707">

Open roscore on a terminal, as that is necessary for you to open other nodes in ROS. </br>
```
pau@terminal:~$ roscore
```

On another terminal, go to your workspace (in my case /catkin_ws) and source it. </br>
```
pau@terminal:~$ cd catkin_ws
pau@terminal:~/catkin_ws$ source devel/setup.bash
```
Now you should be able to run your arduino script with the following line, replacing /dev/ttyACM1 with the actual port to which your Arduino is connected. This can be checked in the Arduino IDE, in Port under the Tools tab. </br>
```
pau@terminal:~$ rosrun rosserial_python serial_node.py /dev/ttyACM1
```
This cannot run if you haven’t uploaded your code to the board, but you must also remember that you will not be able to upload your code if you have this running in the background. Remember to ^C to stop the program if you want to change anything on your code and upload it again.

To publish a message by hand to the desired topic (in this case /input_topic) you must type the following: </br>
```
pau@terminal:~$ rostopic pub /input_topic/ geometry_msgs/Point32 {"x: 0.0 y: 0.0 z: 0.0"}
```
In this case we are telling the gimbal to go to 0° of Pitch, 0° or Roll and 0° of Yaw (or centered), but we can input any number of degrees here, and the gimbal should move to that position.
&nbsp;  
&nbsp;  
&nbsp;  
## 5. Mechanical <a name="mech"></a>









