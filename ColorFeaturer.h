#pragma once
#ifndef COLOR_FEATURER
#define COLOR_FEATURER

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>
#include <cmath>
#include <map>

using namespace std;
using namespace cv;

class ColorFeature
{
public:
	/** The constructor**/
	ColorFeature() {}

	/** The destructor**/
	~ColorFeature() {} 

	/*
	 **
	 * This method is called to get gray levels histogram
	 * @param	sourceImage	input image
	 * @param	histogram	the array that stores image's histogram 
	 * @param	bins	number of gray levels of histogram
	 * @return	
	 *		0: if success
	 *		1: if fail
	 **
	 */
	int CalcGrayscaleHistogram(const Mat& sourceImage, Mat& histogram, int bins = 2);

	/*
	**
	* This method is called to get histogram of color image
	* @param	sourceImage	input image
	* @param	histogram	the array stores that store image's histogram
	* @param	bins	number of color value of histogram
	* @return
	*		0: if success
	*		1: if fail
	**
	*/
	int CalcColorHistogram(const Mat& sourceImage, Mat& histogram, int bins = 2);

	/*
	**
	* This method is called to get gray levels histogram from array of images
	* @param	sourceImage	array of input image
	* @param	histogram	the array that stores image's histogram
	* @param	bins	number of gray levels of histogram
	* @return
	*		0: if success
	*		1: if fail
	**
	*/
	int CalcJoinGrayscaleHistogram(const vector<Mat>& sourceImages, Mat& histogram, int bins = 2);

	/*
	**
	* This method is called to get color histogram from array of images
	* @param	sourceImage	array of input images
	* @param	histogram	the array that stores image's histogram
	* @param	bins	number of color value of histogram
	* @return
	*		0: if success
	*		1: if fail
	**
	*/
	int CalcJoinColorHistogram(const vector<Mat>& sourceImages, Mat& histogram, int bins = 2);

	/*
	**
	* This method is called to get a mean color image of an array images
	* @param	sourceImage	array of input images
	* @param	destinationImage	mean image
	* @return
	*		0: if success
	*		1 : if fail
	**
	*/
	int GetMeanColorImage(const vector<Mat>& sourceImages, Mat& destinationImage);

	/*
	**
	* this method Show the histogram 
	* @param	histogram 
	* @return	visual presentation of the histogram
	**
	*/
	void ShowHistogram(const Mat& histogram, int bins = 2);

	/*
	**
	* Thid method is called to get the color coherence vector of the image
	* @param sourceImage	input color image
	* @param colorCoherenceVector The map holds the alpha and beta values for each color and can be referred to as the color coherence vector	
	* @return
	*		0: if success
	*		1: if fail
	**
	*/
	int GetColorCoherenceVector(const Mat& sourceImage, map< uchar, std::pair<long, long>>& colorCoherenceVector, int nColors = 64);


	/*
	**
	* This method is called to get Color mement Vector
	* @param sourceImg input color image
	* @param momentVector	output moment output vector
	* @return
	*		1: if fail
	*		0: if success
	**
	*/
	int ColorMomentVector(const Mat& sourceImg, vector<float>& momentVector);
protected:
	/*
	**
	* This function is called to reduce numbers of color in the image
	* @param sourceImage	input image
	* @param destinationImage	output image after reducing colors
	* @param nColors	numbers of color
	**
	*/
	void ReduceColors(const Mat& sourceImage, Mat& destinationImage, int nColors = 64);

	/*
	**
	* This function is called to calculate connected components
	* @paramsourceImage	input image
	* @param labeledMat		output labeled components
	**
	*/
	void ConnectedComponentLabeling(const Mat& sourceImage, Mat& labeledMat);

	/*
	**
	* This method is called to find labeled components for disjoint sets
	* @param val	value
	* @param linked	access array
	*
	**
	*/
	unsigned long Find(unsigned long val, unsigned long linked[]);

	/*
	* unite method
	* @param val1, val2	values
	* @param linked access array
	*/
	void Unite(unsigned long val1, unsigned long val2, unsigned long linked[]);

	/*
	**
	* This method to called to calculate the coherence each pixels
	* @param sourceImage	the image belong to the labeled connected components
	* @param inputLabels	the labeled connected components 
	* @return the map that contains the size and color value for eachconnected component
	**
	*/
	map<ushort, std::pair<uchar, long>> CalcCoherence(Mat sourceImage, Mat inputLabels);

private:
	static const int ccvWidth = 240;
	static const int ccvHeight = 160;
	static const int ccvRegion = 25;
};


#endif