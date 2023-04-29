/*
Nilesh Hegde
Project 4 - Camera calibration and AR
Description - This script along with helper.hpp is used to complete Tasks 1-6 of Project 4
*/

#include <opencv2/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include "helper.hpp"

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    VideoCapture *capdev;
    
    //Open Video device
    capdev = new VideoCapture(0);
    if( !capdev->isOpened() ) {
            cout << "Unable to open Video \n";
            return(-1);
    }

    //Get properties of frame
    Size refS( (int) capdev->get(CAP_PROP_FRAME_WIDTH ),
                   (int) capdev->get(CAP_PROP_FRAME_HEIGHT));
    
    //Create window to display
    namedWindow("Video", 1);
    Mat frame;
    Mat gray;
    
    //Declare 2-D and 3-D corner sets and lists variables
    vector< Point2f > corner_set;
    vector< vector< Point2f > > corner_list;
    vector<Point3f> point_set;
    vector< vector< Point3f > > point_list;
    
    //Decalare variables for camera matrix, distortion coefficient, translation vectors and rotation vectors
    Mat cammat = Mat::zeros(3, 3, CV_64F);
    Mat distcoef = Mat::zeros(8, 1, CV_64F);
    Mat rvecs;
    Mat tvecs;
    
    //Counter for image names when stored for calibration
    int n = 1;
    
    //Boolean values used to continuously display a task
    bool displayxyz = false;
    bool displayobj = false;
    bool displaydetect = false;
    bool displayextension = false;
    
    //Store image used for extension
    Mat extensionimage;
    extensionimage = imread( "calvinandhobbes.jpg", 1 );
    
    while(true)
    {
        //Wait for keypress
        char key = waitKey(30);
        
        //Retrieve frame from camera stream and check if it is empty
        *capdev >> frame;
        if( frame.empty() )
        {
          cout << "Empty frame \n";
          break;
        }
        
        //Find grayscale of frame
        cvtColor(frame, gray, COLOR_RGBA2GRAY, 0);
        
        //Clear corner and point set for each frame (NOT list)
        corner_set.clear();
        point_set.clear();
        
 		//x and y limits for 2-D points
        int start1,end1,start2,end2;
        
        //Size of target
        Size pattern;
        
        //Boolean to check if pattern is found
        bool found = false;
        
        //Variables used to store 4 corners of target in different sequence as different functions use differnt sequences
        int topleft, topright, bottomleft, bottomright;
        Point2f inputQuad[4]; 
		Point2f outputQuad[4];
		vector< Point > inputquad;
		vector< Point > outputquad;
		vector< Point > outputquadfill;
		inputquad.push_back(Point(0,0));
		inputquad.push_back(Point(extensionimage.cols-1,0));
		inputquad.push_back(Point(0,extensionimage.rows-1));
		inputquad.push_back(Point(extensionimage.cols-1,extensionimage.rows-1));
		inputQuad[0] = Point2f(0,0);
		inputQuad[1] = Point2f(extensionimage.cols-1,0);
		inputQuad[2] = Point2f(0,extensionimage.rows-1); 
		inputQuad[3] = Point2f(extensionimage.cols-1,extensionimage.rows-1);
		
		//For checkerboard pattern
        if( string(argv[1]).compare("checkerboard")==0 )
        {
        	
        	topleft = 0;
        	topright = 8;
        	bottomleft = 45;
        	bottomright = 53;
        	
        	start1 = 0;
		    end1 = 5;
		    start2 = 0;
		    end2 = 8;
		    pattern = Size(9,6);
		    
        	found = findChessboardCorners(frame, pattern, corner_set, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS);
        	
        	outputquad.push_back(Point(corner_set[topleft]));
        	outputquad.push_back(Point(corner_set[topright]));
        	outputquad.push_back(Point(corner_set[bottomleft]));
        	outputquad.push_back(Point(corner_set[bottomright]));
        	outputquadfill.push_back(Point(corner_set[topleft]));
        	outputquadfill.push_back(Point(corner_set[topright]));
        	outputquadfill.push_back(Point(corner_set[bottomright]));
        	outputquadfill.push_back(Point(corner_set[bottomleft]));
        	outputQuad[0] = corner_set[topleft];
			outputQuad[1] = corner_set[topright];
			outputQuad[2] = corner_set[bottomleft];
			outputQuad[3] = corner_set[bottomright];
			
        }
        
        //For symmetrical circle grid pattern
        else if( string(argv[1]).compare("symcircle")==0 )
        {
        	
        	topleft = 0;
        	topright = 5;
        	bottomleft = 30;
        	bottomright = 35;
        	
        	start1 = 0;
		    end1 = 5;
		    start2 = 0;
		    end2 = 5;
		    
		    pattern = Size(6,6);
        	found = findCirclesGrid(frame, pattern, corner_set);
        	
        	outputquad.push_back(Point(corner_set[topleft]));
        	outputquad.push_back(Point(corner_set[topright]));
        	outputquad.push_back(Point(corner_set[bottomleft]));
        	outputquad.push_back(Point(corner_set[bottomright]));
        	outputquadfill.push_back(Point(corner_set[topleft]));
        	outputquadfill.push_back(Point(corner_set[topright]));
        	outputquadfill.push_back(Point(corner_set[bottomright]));
        	outputquadfill.push_back(Point(corner_set[bottomleft]));
        	outputQuad[0] = corner_set[topleft];
			outputQuad[1] = corner_set[topright];
			outputQuad[2] = corner_set[bottomleft];
			outputQuad[3] = corner_set[bottomright];
			
        }
        
        //Store the points/corners found
        if (found)
        {
        	//If pattern is checkerboard, use cornerSubPix function
        	if( string(argv[1]).compare("checkerboard")==0 )
		    {
		        cornerSubPix(gray, corner_set, Size(5, 5), Size(-1, -1), TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 30, 0.1));
		        
		    }
		    
		    
		    for (int i = start1; i <= end1; i++)
		    {
		    	for (int j = start2; j <=end2; j++)
		    	{
		    		point_set.push_back(Point3f((float)i, (float)-j, 0));
		    	}
		    }

        }
    	
    	
    	//Quit application
        if( key == 'q')
        {
            break;
        }
        
        //Display the detected corners
        if (key == 'd' || displaydetect)
        {
        	displayxyz = false;
        	displayobj=false;
        	displaydetect = true;
        	displayextension = false;
        	if (!found)
            {
                cout << "No corners found \n";
            }
            else
            {
            	
            	drawChessboardCorners(frame, pattern, corner_set, found);
            }
        }
        
        //Save images for calibration
        if( key == 's')
        {
            if (!found)
            {
                cout << "No corners found \n";
            }
            else
            {
            	//save image
                cout << "Found corners in frame. Storing Image" <<n<<".jpg\n";
                imwrite("Image" + to_string(n) + ".jpg", frame);
                n=n+1;
                corner_list.push_back(corner_set);
                point_list.push_back(point_set);
                
                //Calibrate if 5 or more images are saved
                if(corner_list.size() >= 5)
                {
                	//Initialize camera matrix
                	cout<<"Calibrating...\n";
                	if( string(argv[1]).compare("checkerboard")==0 )
		    		{
		            	cammat.at<double>(0,0) = 1;
		            	cammat.at<double>(0,2) = 4.5;
		                cammat.at<double>(1,1) = 1;
		                cammat.at<double>(1,2) = 3;
		                cammat.at<double>(2,2) = 1;
		            }
		            if( string(argv[1]).compare("symcircle")==0 )
		    		{
		            	cammat.at<double>(0,0) = 1;
		            	cammat.at<double>(0,2) = 3;
		                cammat.at<double>(1,1) = 1;
		                cammat.at<double>(1,2) = 3;
		                cammat.at<double>(2,2) = 1;
		            }
		            
		            //Calibrate the camera
                    vector<Mat> rvecs, tvecs;
                    double error = calibrateCamera(point_list, corner_list, frame.size(), cammat, distcoef, rvecs, tvecs, CALIB_FIX_ASPECT_RATIO,TermCriteria(TermCriteria::EPS |TermCriteria::COUNT, 30, 0.1));
                    
                    //Store camera matrix and distortion coefficient and display them along with reprojection error
                    cout << "Camera Matrix and Distortion Coefficient saved to JSON file \n" ;
                    cout<<"Camera Matrix : "<<cammat<<endl<<"Distortion coefficients : "<<distcoef<<endl;
                    cout << "Reprojection Error : " << error << endl;
                    storeJSON(cammat,distcoef);
                }
            }
            
        }
        
        
        //Display world 3-D axes
        if( key == 'p' || displayxyz)
        {
        	displayxyz = true;
        	displayobj=false;
        	displaydetect = false;
        	displayextension = false;
        	if(!found)
        	{
        		cout<<"Pattern not found\n";
        	}
        	else if(corner_list.size() < 5)
        	{
        		cout<<"Calibration not done\n";
        	}
        	//Draw the axes using solvePnP() and projectedPoints() functions
        	else
        	{
        		readJSON(cammat,distcoef);
        		cout<<"Camera Matrix : "<<cammat<<endl<<"Distortion coefficients : "<<distcoef<<endl;
        		solvePnP(point_set, corner_set, cammat, distcoef, rvecs, tvecs);
                cout << "Rotation Matrix:" << endl << rvecs << endl << "Translation Matrix:" << endl << tvecs << endl;
                
                //Translate points from 2-D to 3-D
                vector<Point2f> imageAxisEndPoint;
                vector<Point3f> worldAxisEndPoint;
                worldAxisEndPoint.push_back(Point3f(1,0,0));
                worldAxisEndPoint.push_back(Point3f(0,-1,0));
                worldAxisEndPoint.push_back(Point3f(0,0,-1));
                
                projectPoints(worldAxisEndPoint, rvecs, tvecs, cammat, distcoef, imageAxisEndPoint);
                arrowedLine(frame,corner_set[0],imageAxisEndPoint[0],Scalar(255,0,0), 3);
                arrowedLine(frame,corner_set[0],imageAxisEndPoint[1],Scalar(0,255,0), 3);
                arrowedLine(frame,corner_set[0],imageAxisEndPoint[2],Scalar(0,0,255), 3);
               
               
        	}
        }
        
        //Display Virtual 3-D object
        if( key == 'o' || displayobj )
        {
        	displayobj = true;
        	displayxyz = false;
        	displaydetect = false;
        	displayextension = false;
        	if(!found)
        	{
        		cout<<"Pattern not found\n";
        	}
        	else if(corner_list.size() < 5)
        	{
        		cout<<"Calibration not done\n";
        	}
        	
        	//Read vertices and face vertices from .obj file which is passed as command line argument and reproject object by drawing lines between vertices
        	else
        	{
        		readJSON(cammat,distcoef);
        		solvePnP(point_set, corner_set, cammat, distcoef, rvecs, tvecs);
        		vector<Point2f> objPoints;
        		vector<Point3f> vertices;
        		vector<vector<int>> faceVertices;
        		OBJextractor(argv[2],vertices, faceVertices);
                projectPoints(vertices, rvecs, tvecs, cammat, distcoef, objPoints);
                int i,j;
                for(i = 0; i<faceVertices.size(); i++)
                {
                    for(j = 0; j<faceVertices[i].size() - 1; j++)
                    {
                        line(frame,objPoints[faceVertices[i][j]-1],objPoints[faceVertices[i][j+1]-1],Scalar(128,255,0), 1);
                    }
                }
        	}
        }
        
        //Display an image over target pattern
        if( key == 'e' || displayextension)
        {
        	displayobj = false;
        	displayxyz = false;
        	displaydetect = false;
        	displayextension = true;
        	
        	if(!found)
        	{
        		cout<<"Pattern not found\n";
        	}
        	else
        	{
        		//Create mask
        		Mat mask1;
        		Mat temp(frame.size(), CV_8UC3);
        		//Find Homography
        		Mat H = findHomography(inputquad,outputquad,RANSAC,5,mask1);
        		//Warp the image
        		warpPerspective(extensionimage,temp,H,frame.size());
        		Mat mask2(frame.size(), CV_8UC3, Scalar(0,0,0));
        		Mat r1(frame.size(), CV_8UC3);
        		//Fill the empty target pattern with white
        		fillConvexPoly(mask2,outputquadfill,Scalar(255,255,255));
        		//Invert Mask
        		bitwise_not(mask2,mask2);
        		//Retain the background
        		bitwise_and(mask2,frame,r1);
        		bitwise_or(r1,temp,frame);
        	}
        }
        
        //Save image for report purpose
        if (key == 'i')
        {
        	imwrite("report.jpg",frame);
        }
        
        //Reset system
        if ( key == 'r')
        {
        	displayobj = false;
        	displayxyz = false;
        	displaydetect = false;
        	displayextension = false;
        }
        
        //Display frame
        imshow("Video", frame);
    }

    delete capdev;
    return(0);
}
