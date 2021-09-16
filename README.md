# Object detection

This project allows to track green objects in live (like tennis ball) on the image of a camera.
To do so, the frames of the camera are converted to HSV to be thresholded with min-max values. Then erode and dilate operations are 
used to remove noise and improve the object detection. Finally, the processed frames are displayed in RGB or black and white if the calibration mode is turned on.

# Technologies
- Qt 5.9.0
- OpenCV 4.1.2

# Lauch
- Download and extract the project
- Open the project with Qt
- Change the OpenCV directory in the CMakeFile
- Change the loading of the packages in the CMakeFile according to your Qt version
- Run the project and clic play button to start displaying the image of your camera 
- Tune the HSV thresholds parameters to optimise the detection of the object. 
- The calibration mode allows to display the thresholded images with the HSV parameters.
