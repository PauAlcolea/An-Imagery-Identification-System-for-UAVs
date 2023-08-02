Instructions to use the URDF for the system
Pau Alcolea Vila
Summer 2023
UNR, Robowork

To use the urdf, the package (gimbal_urdf) should be put in the src directory in some workspace
There are two important files:
    - finalRedesigned.urdf.xacro
    - rvizLAUNCH.launch

finalRedesidgned.urdf.xacro contains the actual urdf, which references some STL files on the meshes folder in the package
rvizLAUNCH.launch is the file that opens the rviz simulator and opens the urdf file

on the terminal, source the workspace where the package is located and run the following line, replacing gimbal_urdf with whatever you named the package:
roslaunch gimbal_urdf rvizLAUNCH.launch


Initial_Iteration.urdf.xacro is the initial iteration, back when the camera wasn't mounted shifted 90 degrees. To run that urdf, use the launch file: oldLAUNCH.launch
