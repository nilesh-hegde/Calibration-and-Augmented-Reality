#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	VideoCapture* capdev;

	//Open Video source
	capdev = new VideoCapture(0);
	if (!capdev->isOpened()) {
		printf("Unable to open video device\n");
		return (-1);
	}

	//Retrieve properties
	Size refS((int)capdev->get(CAP_PROP_FRAME_WIDTH),
	              (int)capdev->get(CAP_PROP_FRAME_HEIGHT));
	
	//Create window to display
	namedWindow("Robust Features", 1);
	
	//Initialise threshold
	int thresh = 100;
	
	//Declare frames
	Mat frame;
	Mat gray;
	
	//Boolean to keep displaying Harris corners
	bool displayharris = false;

	while (true) 
	{
		//Get current frame form camera stream
		*capdev >> frame;
		if (frame.empty()) {
			printf("frame is empty\n");
			break;
		}
		
		//Find grayscale of frame
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		
		//Wait for keypress
		char key = waitKey();
		
		//To quit application
		if(key == 'q')
		{
			break;
		}

		//To start detecting Harrsi corners
		if(key == 'h' || displayharris)
		{
			//Change boolean value to keep displaying Harrsi corners
			displayharris = true;
			vector<Point2f> corners;
			//Detect Harris corners with 'true' parameter in the following function
			goodFeaturesToTrack(gray, corners, 100, 0.1, 10, Mat(), 3, 3, true, 0.04);
			//Draw the corners on original frame
			for (int i = 0; i < corners.size(); i++) 
			{
				circle(frame, corners.at(i), 2, Scalar(255, 0, 0), 2);
			}
		}
		
		//Save image for reporting
		if (key == 'i')
        {
        	imwrite("report.jpg",frame);
        }
		
		//Reset stream
		if(key == 'r')
		{
			displayharris = false;
		}
		
		//Display updated frame
		imshow("video", frame);
	}
	return 0;
}
