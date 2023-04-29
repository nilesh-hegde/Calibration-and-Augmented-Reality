# Calibration-and-Augmented-Reality

In this project, a system is built that first detects and extarcts target corners. Then if more than 5 calibration images are saved, the camera matrix and distortion matrix are then calculated and stored in a JSON file. These values are then used to display the world frame axes and also project a 3-D object whose vertices and faces were extracted from Wavefront 3D object file (.obj extension).

OS - Linux (Debian - Ubuntu 22.04)
IDE - None (CLE using terminal)


Build instructions: 
1. Use CMakeLists.txt to generate make file then build target and then execute target (Command line execution using terminal in Linux (debian)). Command is ./project <pattern> <obj file> for tasks 1-6 and extension and ./task for Harris Corner.
2. If any of the tasks do not mirror the tasks, please build and execute once again. 
3. NOTE - Please keep target (checkboard or circle grid) in focus for camera to detect intially. Application will close otherwise.

Applicable Keypresses - 
a) ./project 
	1.  'd' - detect corner
	2.  's' - save image for calibration
	3.  'p' - project 3-D axes
	4.  'o' - display 3-D virtual object
	5.  'r' - reset
	6.  'i' - save image
b) ./task
	1.  'h' - detect Harris corner
	2.  'r' - reset
	3.  'i' - save image
  
  ![calib4](https://user-images.githubusercontent.com/55364143/235316861-e4769cf7-f836-415c-9553-f50cce94f17a.jpg)
  ![complex2](https://user-images.githubusercontent.com/55364143/235316869-1afcb55a-0e64-4cbd-be29-bb7ecbffe5b2.jpg)
  ![extention](https://user-images.githubusercontent.com/55364143/235316877-a029cfd5-ab95-4139-9881-3d5d4c5b6263.jpg)

