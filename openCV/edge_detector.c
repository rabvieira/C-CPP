//*****************************************************************************
// Name      : edgeDetector.c
// Date	    : April 27, 2015
// Class     : SE300, Software Engineering Practices
// Author    : Team 2
// Descript. : This program was implemented as a prototype for the edge detector
//			  system. It outputs a window displaying a video.
//			  There are 3 parameters to set through track bar:
//			  - Threshold: refines the edge detector algorithm;
//			  - Filter:    selects which filter would be used
//			  - Algorithm: selects which edge detector algorithm would be used
//			  The video shown is the frame after a edge detection transformation
//			  Note: this code does require openCV 3.0
//*****************************************************************************

/* libraries */
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui.hpp"
#include<time.h>

/* namespace */
using namespace cv;
using namespace std;

/* Global variables */
Mat edges;
int lowThreshold = 30;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
int alg = 0;
int filter = 0;
char* window_name = "Edge Detector";
//char* FPS = "60";

//*****************************************************************************
// Shows the numeric value of the current threshold
//*****************************************************************************
void CannyThreshold(int, void*)
{
	//cout << "threshold: " << lowThreshold << endl;
}

//*****************************************************************************
// Shows the current filter selected: either GaussianBlur or Sobel
//*****************************************************************************
void filter_selection(int, void *)
{
	if (!filter)
	{
		cout << "filter:    GaussianBlur" << endl;
	}
	else
		cout << "filter:    Sobel" << endl;
}

//*****************************************************************************
// Shows the current algorithm selected: either Canny or Hough Lines
//*****************************************************************************
void algorithm_selection(int, void *)
{
	if (!alg)
	{
		cout << "algorithm: Canny" << endl;
	}
	else
		cout << "algorithm: Hough Lines" << endl;
}

void printData()
{
	algorithm_selection(0, 0);
	filter_selection(0, 0);
}

//*****************************************************************************
// Do the transformation given a frame from the camera a displays that
//*****************************************************************************
int main(int, char**)
{
	time_t start, end;
	int countMod = 0;
	double fps;
	VideoCapture cap(0);  // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	namedWindow(window_name, WINDOW_NORMAL);
	time(&start);
	int counter = 0;
	for (;;)
	{
		Mat frame;
		if (countMod % 25 == 0)
		{
			system("CLS");
			printData();
		}
		cap >> frame; // get a new frame from camera
		cvtColor(frame, edges, COLOR_BGR2GRAY);
		// applies filter/algorithm based upon the track bar input
		if (!filter)
		{
			GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		}
		else
		{
			Sobel(edges, edges, -1, 1, 0, 3, 1, 0, BORDER_REFLECT_101);
		}

		if (!alg)
		{
			Canny(edges, edges, lowThreshold, lowThreshold*ratio, kernel_size);
		}
		else
		{
			Mat dst;
			Canny(frame, dst, lowThreshold + 60, lowThreshold + 160, kernel_size);
			cvtColor(dst, edges, COLOR_GRAY2BGR);
			vector<Vec4i> lines;
			HoughLinesP(dst, lines, 1, CV_PI / 180, 80, 30, 10);
			for (size_t i = 0; i < lines.size(); i++)
			{
				line(edges, Point(lines[i][0], lines[i][1]),
					Point(lines[i][2], lines[i][3]), Scalar(0, 0, 255), 3, 8);
			}
		}
		
		imshow(window_name, edges);
		/// Create a Trackbar for user to enter threshold
		createTrackbar("Threshold", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
		createTrackbar("Filter", window_name, &filter, 1, filter_selection);
		createTrackbar("Algorithm", window_name, &alg, 1, algorithm_selection);
		//double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
		//double fps = cap.get(2);
		//cout << "Frame per seconds : " << fps << endl;

		//sprintf(FPS, "FPS: %d", 60);
		//displayStatusBar(window_name, FPS, 0);
		//displayOverlay(window_name, FPS, 0);
		time(&end);
		++counter;
		double sec = difftime(end, start);
		if (countMod % 25 == 0)
		{
			double fps = counter / sec;
			printf("\nfps:       %lf\n\n", fps);
		}
		countMod++;
		if (waitKey(30) >= 0) break;
	}
	printf("\n");
	//cerr << cv::getBuildInformation();
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}