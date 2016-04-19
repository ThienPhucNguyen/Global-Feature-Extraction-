#pragma once
#ifndef SHAPEFEATURER_H
#define SHAPEFEATURER_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

#define PI 3.14
#define MAXWIDTH 512
#define MAXHEIGHT 512
#define CELLSIZE 8

class ShapeFeature {
public:
	/*
	**
	* This method is called to get edge direction histogram
	* @param sourceImage	input color image
	* @param result	output histogram
	* return
	*		1: if fail
	*		0: if success
	**
	*/
	int EDH(const Mat& sourceImage, double*& result);

	/*
	**
	* This method is called to get HOG descriptors
	* @param sourceImage	inputImage
	* @param hogDescriptors	extracted HOG vector
	**
	*/
	void getHOGFeatures(const Mat& sourceImage, vector<float>& hogDescriptors);
	//Mat get_hogdescriptor_visual_image(Mat& origImg, vector<float>& descriptorValues, Size winSize, Size cellSize, int scaleFactor, double viz_factor);
protected:
private:
};
#endif