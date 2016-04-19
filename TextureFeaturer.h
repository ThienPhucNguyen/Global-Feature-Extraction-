#pragma once
#ifndef TEXTUREFEATURER_H
#define TEXTUREFEATURER_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

#define PI 3.14
#define LIMIT 10

class TamuraFeature
{
public:
	/*Constructor*/
	TamuraFeature();

	/*Destructor*/
	~TamuraFeature();

	/*
	**
	* This method is called to get derectionality of the image
	* @param sourceImage	input image
	* @param r	ratio
	* @param bins	the number of bins of the edge oriented histogram
	* @return directionality value of the image
	**
	*/
	double Directionality(const Mat& sourceImage, int r, int bins = 180);

	/*
	**
	* This method is called to get linelikeness of the image
	* @param sourceImage	input image
	* @return linelikeness value of the image
	**
	*/
	double Linelikeness(const Mat& sourceImage);
protected:
	/***PREWITT OPERATOR***/
	/*Horizontal matrix*/
	//-1 0 1
	//-1 0 1
	//-1 0 1
	/*Vertical matrix*/
	// 1 1 1
	// 0 0 0
	// -1 -1 -1

	/*
	**
	* This method is called to get Y gradient using Prewitt operator
	* @param sourceImage input grayscale image
	* @param x x-coordinate of the pixel
	* @param y y-coordinate of the pixel
	* @return X gradient value of the pixel(x,y)
	**
	*/
	double PrewittX(const Mat& sourceImage, int x, int y);

	/*
	**
	* This method is called to get Y gradient using Prewitt operator
	* @param sourceImage input grayscale image
	* @param x x-coordinate of the pixel
	* @param y y-coordinate of the pixel
	* @return Y gradient value of the pixel(x,y)
	**
	*/
	double PrewittY(const Mat& sourceImage, int x, int y);
private:

};
#endif