#include "ColorFeaturer.h"

/*
 **
 * This method is called to get gray levels in histogram
 * @param	sourceImage	input image
 * @param	histogram	array stores image's histogram
 * @param	bin	number of gray levels of histogram
 * @return 
 *		0: if success
 *		1: if fail
 **
 */
int ColorFeature::CalcGrayscaleHistogram(const Mat& sourceImage, Mat& histogram, int bins)
{
	
	if (sourceImage.data == NULL) {
		cout << "Cannot read the image!" << endl;
		return 1;
	}
	
	Mat sourceImg = sourceImage.clone();
	//if sourceImage is not gray scale image
	//convert sourceImage to grayscale image
	if (sourceImage.channels() != 1)
		cvtColor(sourceImage, sourceImg, CV_BGR2GRAY);

	//initialize histogram vector
	if (histogram.data != NULL)
		histogram.release();
	histogram = Mat::zeros(1, bins, CV_32F);

	int widthStep = sourceImg.step;
	int nChannels = sourceImg.channels(); 

	uchar* pSourceData = sourceImg.data;

	//distribute the histogram
	for (int y = 0; y < sourceImg.rows; ++y, pSourceData += widthStep) {
		uchar* pSourceRow = pSourceData;
		for (int x = 0; x < sourceImage.cols; ++x, pSourceRow += nChannels) {
			int idx =*(pSourceRow) * bins / 256;
			histogram.at<float>(idx) += 1;
		}
	}
	
	return 0;
}

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
int ColorFeature::CalcColorHistogram(const Mat& sourceImage, Mat& histogram, int bins)
{
	if (sourceImage.data == NULL || sourceImage.channels() == 1) {
		cout << "Cannot read the image!" << endl;
		return 1;
	}
		
	if (histogram.data != NULL)
		histogram.release();

	histogram = Mat::zeros(3, bins, CV_32F);

	int widthStep = sourceImage.step;
	int nChannels = sourceImage.channels();

	uchar* pSourceData = sourceImage.data;

	//distribute the histogram
	for (int y = 0; y < sourceImage.rows; ++y, pSourceData += widthStep) {
		uchar* pSourceRow = pSourceData;
		for (int x = 0; x < sourceImage.cols; ++x, pSourceRow += nChannels) {
			int idx1 = *(pSourceRow) * bins / 256;
			int idx2 = *(pSourceRow + 1) * bins / 256;
			int idx3 = *(pSourceRow + 2) * bins / 256;
			histogram.at<float>(Point(idx1, 0)) += 1;
			histogram.at<float>(Point(idx2, 1)) += 1;
			histogram.at<float>(Point(idx3, 2)) += 1;
		}
	}

	return 0;
}

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
int ColorFeature::CalcJoinGrayscaleHistogram(const vector<Mat>& sourceImages, Mat& histogram, int bins)
{
	if (histogram.data != NULL)
		histogram.release();

	histogram = Mat::zeros(1, bins, CV_32F);

	for (int i = 0; i < sourceImages.size(); ++i) {
		Mat hist;
		int success = this->CalcGrayscaleHistogram(sourceImages[i], hist, bins);
		if (success == 1)
			return 1;
		for (int j = 0; j < hist.cols; ++j)
			histogram.at<float>(j) += hist.at<float>(j);
	}

	for (int j = 0; j < histogram.cols; ++j)
		histogram.at<float>(j) /= sourceImages.size();

	return 0;
}

/*
**
* This method is called to get color histogram from array of images
* @param	sourceImage	array of input image
* @param	histogram	the array that stores image's histogram
* @param	bins	number of color value of histogram
* @return
*		0: if success
*		1: if fail
**
*/
int ColorFeature::CalcJoinColorHistogram(const vector<Mat>& sourceImages, Mat& histogram, int bins)
{
	if (histogram.data != NULL)
		histogram.release();

	histogram = Mat::zeros(3, bins, CV_32F);

	for (int i = 0; i < sourceImages.size(); ++i) {
		Mat hist;
		int success = this->CalcColorHistogram(sourceImages[i], hist, bins);
		if (success == 1)
			return 1;
		for (int i = 0; i < hist.rows; ++i)
		{
			for (int j = 0; j < hist.cols; ++j)
				histogram.at<float>(i, j) += hist.at<float>(i, j);
		}
	}

	for (int i = 0; i < histogram.rows; ++i)
	{
		for (int j = 0; j < histogram.cols; ++j)
			histogram.at<float>(i, j) /= sourceImages.size();
	}

	return 0;
}

/*
**
* This method is called to get a mean color image of an array images
* @param	sourceImage	array of input images with same size
* @param	destinationImage	mean image
* @return
*		0: if success
*		1 : if fail
**
*/
//chua test
int ColorFeature::GetMeanColorImage(const vector<Mat>& sourceImages, Mat& destinationImage)
{
	
	for (int i = 0; i < sourceImages.size() - 1; ++i) {
		if (sourceImages[i].cols != sourceImages[i + 1].cols || sourceImages[i].rows != sourceImages[i + 1].rows) {
			cout << "The images should be same size!" << endl;
			return 1;
		}
		if (sourceImages[i].data == NULL || sourceImages[i].channels() == 1) {
			cout << "Cannot read the images!" << endl;
			return 1;
		}
	}

	if (sourceImages[sourceImages.size() - 1].data == NULL|| sourceImages[sourceImages.size() - 1].channels() == 1) {
		cout << "Cannot read the images!" << endl;
		return 1;
	}

	if (destinationImage.data != NULL)
		destinationImage.release();
	destinationImage = Mat::zeros(sourceImages[0].size(), sourceImages[0].type());

	int nImages = sourceImages.size();

	int nChannels = sourceImages[0].channels();
	int widthStep = sourceImages[0].step;

	uchar* pDestinationtData = destinationImage.data;

	int columns = sourceImages[0].cols;
	int rows = sourceImages[0].rows;

	vector<uchar*> pSourceData;
	for (int i = 0; i < nImages; ++i)
		pSourceData.push_back(sourceImages[i].data);

	

	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < columns; ++x) {
			int r = 0;
			int g = 0;
			int b = 0;
			for (int k = 0; k < nImages; ++k) {
				b += pSourceData[k][y*widthStep + x*nChannels];
				g += pSourceData[k][y*widthStep + x*nChannels + 1];
				r += pSourceData[k][y*widthStep + x*nChannels + 2];
			}
			pDestinationtData[y*widthStep + x*nChannels] = saturate_cast<uchar>(static_cast<float>(b) / static_cast<float>(nImages));
			pDestinationtData[y*widthStep + x*nChannels] = saturate_cast<uchar>(static_cast<float>(g) / static_cast<float>(nImages));
			pDestinationtData[y*widthStep + x*nChannels] = saturate_cast<uchar>(static_cast<float>(r) / static_cast<float>(nImages));
		}
	}
	return 0;
}

/*
**
* Show the histogram
* @param	histogram
* @return	visual presentation of the histogram
**
*/
void ColorFeature::ShowHistogram(const Mat& histogram, int bins)
{
	//draw histogram image
	int histogramWidth = 521;
	int histogramHeight = 400;
	int binWidth = cvRound(static_cast<double>(histogramWidth) / static_cast<double>(bins));
	Mat histogramImage(histogramHeight, histogramWidth, CV_8UC3, Scalar(0, 0, 0));

	if (histogram.rows == 1)
	{
		//Normalize the result to [0, histImage.rows]
		normalize(histogram, histogram, 0, histogramImage.rows, NORM_MINMAX, -1, Mat());
		//draw
		for (int i = 1; i < bins; ++i)
			line(histogramImage, Point(binWidth * (i - 1), histogramHeight - cvRound(histogram.at<float>(i - 1))), Point(binWidth * (i), histogramHeight - cvRound(histogram.at<float>(i))), Scalar(255, 255, 255), 2, 8, 0);
	}
	else
	{
		Mat bHistogram, gHistogram, rHistogram;
		//Normalize the result to [0, histImage.rows]
		normalize(histogram.row(0), bHistogram, 0, histogramImage.rows, NORM_MINMAX, -1, Mat());
		normalize(histogram.row(1), gHistogram, 0, histogramImage.rows, NORM_MINMAX, -1, Mat());
		normalize(histogram.row(2), rHistogram, 0, histogramImage.rows, NORM_MINMAX, -1, Mat());

		// Draw for each channel
		for (int i = 1; i < bins; i++)
		{
			line(histogramImage, Point(binWidth * (i - 1), histogramHeight - cvRound(bHistogram.at<float>(i - 1))), Point(binWidth * (i), histogramHeight - cvRound(bHistogram.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
			line(histogramImage, Point(binWidth * (i - 1), histogramHeight - cvRound(gHistogram.at<float>(i - 1))), Point(binWidth * (i), histogramHeight - cvRound(gHistogram.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
			line(histogramImage, Point(binWidth * (i - 1), histogramHeight - cvRound(rHistogram.at<float>(i - 1))), Point(binWidth * (i), histogramHeight - cvRound(rHistogram.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);
		}
	}
		

	//display
	imshow("Histogram", histogramImage);
	waitKey(0);
}

/*
**
* Thid method is called to get the color coherence vector of the image
* @param sourceImage	input image
* @param colorCoherenceVector The map holds the alpha and beta values for each color and can be referred to as the color coherence vector
* @return
*		0: if success
*		1: if fail
**
*/
int ColorFeature::GetColorCoherenceVector(const Mat& sourceImage, map< uchar, std::pair<long, long>>& colorCoherenceVector, int nColors)
{
	if (!sourceImage.data) {
		cout << "Cannot read image!" << endl;
		return 1;
	}

	if (sourceImage.channels() != 3) {
		cout << "Input image isn't a color image!" << endl;
		return 1;
	}

	//scale the source image
	Mat resizedImage;
	resize(sourceImage, resizedImage, cvSize(ccvWidth, ccvHeight));
	//smooth image
	medianBlur(resizedImage, resizedImage, 3);

	//Discretize the color-space (images’ colors) into n distinct color.
	Mat reduceImage;
	this->ReduceColors(resizedImage, reduceImage, 64);

	//connected components
	Mat labeledComponents;

	//connected components labeling to detemine the coherence of each pixel.
	//The coherence is the size of connected component of the current pixel
	this->ConnectedComponentLabeling(reduceImage, labeledComponents);

	//label is color size
	map<ushort, pair<uchar, long>> coherenceMap = this->CalcCoherence(reduceImage, labeledComponents);

	//Calculate color coherence vector

	//Iterator over the coherenceMap
	std::map<ushort, std::pair<uchar, long> >::iterator it;

	//Walk through the coherence map and sum up the incoherent and coherent pixels for every color
	for (it = coherenceMap.begin(); it != coherenceMap.end(); ++it) {
		if (colorCoherenceVector.find(it->second.first) != colorCoherenceVector.end()) {
			//already have this color
			if (it->second.second >= this->ccvRegion) {
				//pixels in current blob are coherent -> increase alpha
				colorCoherenceVector[it->second.first].first += it->second.second;
			}
			else {
				//pixels in current blob are incoherent -> increase beta
				colorCoherenceVector[it->second.first].second += it->second.second;
			}
		}
		else {
			//if don't have this color in our ccv yet and need to add it
			if (it->second.second >= this->ccvRegion)
			{
				//pixels in current blob are coherent -> set alpha
				colorCoherenceVector[it->second.first].first = it->second.second;
				colorCoherenceVector[it->second.first].second = 0;
			}
			else
			{
				//pixels in current blob are incoherent -> set beta
				colorCoherenceVector[it->second.first].first = 0;
				colorCoherenceVector[it->second.first].second = it->second.second;
			}
		}
	}

	//set unused colors to 0	
	for (unsigned char c = 0; c < nColors; ++c)
	{
		if (colorCoherenceVector.find(c) == colorCoherenceVector.end())
		{
			colorCoherenceVector[c].first = 0;
			colorCoherenceVector[c].second = 0;
		}
	}
	
	return 0;
}

/*
**
* This function is called to reduce numbers of color in the image
* @param sourceImage	input image
* @param destinationImage	output image after reducing colors
* @param nColors	numbers of color
**
*/
void ColorFeature::ReduceColors(const Mat& sourceImage, Mat& destinationImage, int nColors)
{
	int widthStep = sourceImage.step;
	int nChannels = sourceImage.channels();
	
	destinationImage = sourceImage.clone();
	uchar* pSoureData = sourceImage.data;
	uchar* pDestinationData = destinationImage.data;
	for (int y = 0; y < sourceImage.rows; ++y) {
		for (int x = 0; x < sourceImage.cols; ++x) {
			pDestinationData[y*widthStep + x*nChannels] = pSoureData[y*widthStep + x*nChannels] / nColors + nColors / 2;
			pDestinationData[y*widthStep + x*nChannels + 1] = pSoureData[y*widthStep + x*nChannels + 1] / nColors + nColors / 2;
			pDestinationData[y*widthStep + x*nChannels + 2] = pSoureData[y*widthStep + x*nChannels + 2] / nColors + nColors / 2;
		}
	}
}

/*
**
* This function is called to calculate connected components
* @param sourceImage	input image
* @param labeledMat		output labeled components
**
*/
void ColorFeature::ConnectedComponentLabeling(const Mat& sourceImage, Mat& labeledMat)
{
	//allocate labeledMat and set it to zero
	labeledMat = Mat(sourceImage.size(), CV_16U, Scalar(0));

	//disjoint algorithm
	//set data structure to manage the labels
	unsigned long* linked = new unsigned long[labeledMat.rows * labeledMat.cols];
	for (unsigned long i = 0; i < labeledMat.cols * labeledMat.rows; ++i)
		linked[i] = i;

	//1 channel pointer to input image
	cv::Mat_<uchar>& ptrSource = (cv::Mat_<uchar>&)sourceImage;
	//1 channel pointer to output image
	cv::Mat_<ushort>& ptrLabeled = (cv::Mat_<ushort>&)labeledMat;

	//first pass: Initial labeling
	unsigned int currentLabel = 0;
	for (int y = 0; y < labeledMat.rows; ++y) {
		for (int x = 0; x < labeledMat.cols; ++x) {
			if (y == 0) {
				if (x == 0) {
					//first pixel
					//create first label
					ptrLabeled(y, x) = ++currentLabel;
				}
				else {
					//first row
					//only check left pixel
					if (ptrSource(y, x) == ptrSource(y, x - 1)) {
						//same region
						ptrLabeled(y, x) = ptrLabeled(y, x - 1);
					}
					else {
						//difference region
						ptrLabeled(y, x) = ++currentLabel;
					}
				}
			}
			else {
				if (x == 0) {
					//first column
					//only check top pixel
					if (ptrSource(y, x) == ptrSource(y - 1, x)) {
						//same region
						ptrLabeled(y, x) = ptrLabeled(y - 1, x);
					}
					else {
						//difference region
						ptrLabeled(y, x) = ++currentLabel;
					}
				}
				else {
					//regular column
					if (ptrSource(y, x) == ptrSource(y, x - 1) && ptrSource(y, x) == ptrSource(y - 1, x)) {
						//same region
						//assign minimum label of both
						ptrLabeled(y, x) = min(ptrLabeled(y, x - 1), ptrLabeled(y - 1, x));
						if (ptrLabeled(y, x - 1) != ptrLabeled(y - 1, x)) {
							//mark labels
							//use union/find algorithm for disjoint sets
							this->Unite(this->Find(ptrLabeled(y, x - 1), linked), this->Find(ptrLabeled(y - 1, x), linked), linked);
						}
					}
					else if (ptrSource(y, x) == ptrSource(y, x - 1)) {
						//same region as left pixel
						ptrLabeled(y, x) = ptrLabeled(y, x - 1);
					}
					else if (ptrLabeled(y, x) == ptrLabeled(y - 1, x)) {
						//same region as top pixel
						ptrLabeled(y, x) = ptrLabeled(y - 1, x);
					}
					else {
						//difference region
						ptrLabeled(y, x) = ++currentLabel;
					}
				}
			}
		}
	}

	//second pass: merge equivalent labels
	for (int y = 0; y < labeledMat.rows; ++y) {
		for (int x = 0; x < labeledMat.cols; ++x) {
			//use union/find algorithm for disjont sets
			ptrLabeled(y, x) = (unsigned long) this->Find(ptrLabeled(y, x), linked);
		}
	}

	delete[] linked;
}

/*
**
* This method is called to find labeled components for disjoint sets
* @param val	value
* @param linked	access array
* @return value
**
*/
unsigned long ColorFeature::Find(unsigned long val, unsigned long linked[])
{
	while (linked[val] != val) {
		linked[val] = linked[linked[val]];
		val = linked[val];
	}
	return val;
}

/*
* unite method
* @param val1, val2	values
* @param linked access array
*/
void ColorFeature::Unite(unsigned long val1, unsigned long val2, unsigned long linked[])
{
	linked[this->Find(val1, linked)] = this->Find(val2, linked);
}

/*
**
* This method to called to calculate the coherence each pixels
* @param sourceImage	the image belong to the labeled connected components
* @param inputLabels	the labeled connected components
* @return the map that contains the size and color value for eachconnected component
**
*/
map<ushort, std::pair<uchar, long>> ColorFeature::CalcCoherence(Mat sourceImage, Mat inputLabels)
{
	//1 channel pointer to input image
	cv::Mat_<ushort>& ptrLabels = (cv::Mat_<ushort>&)inputLabels;
	//1 channel pointer to input colors image
	cv::Mat_<uchar>& ptrSource = (cv::Mat_<uchar>&)sourceImage;

	//Map to hold the number of pixels and the color per label
	map<ushort, std::pair<uchar, long>> coherences;

	//calculate coherence values per label
	for (int y = 0; y < sourceImage.rows; ++y) {
		for (int x = 0; x < sourceImage.cols; ++x) {
			if (coherences.find(ptrLabels(y, x)) != coherences.end()) {
				++coherences[ptrLabels(y, x)].second;
			}
			else {
				coherences[ptrLabels(y, x)].second = 1;
				coherences[ptrLabels(y, x)].first = ptrSource(y, x);
			}
		}
	}
	return coherences;
}

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
int ColorFeature::ColorMomentVector(const Mat& sourceImg, vector<float>& momentVector)
{
	if (sourceImg.empty())
		return 1;
	double E[3] = { 0, 0, 0 }, delta[3] = { 0, 0, 0 }, s[3] = { 0, 0, 0 };
	for (int i = 0; i < sourceImg.rows; i++)
	for (int j = 0; j < sourceImg.cols; j++)
	for (int k = 0; k < 3; k++)
	{
		E[k] += sourceImg.at<Vec3b>(i, j)[k];

	}
	for (int k = 0; k < 3; k++)
	{
		E[k] /= sourceImg.rows*sourceImg.cols;
	}
	for (int i = 0; i < sourceImg.rows; i++)
	for (int j = 0; j < sourceImg.cols; j++)
	for (int k = 0; k < 3; k++)
	{
		delta[k] += pow(sourceImg.at<Vec3b>(i, j)[k] - E[k], 2);
		s[k] += pow(sourceImg.at<Vec3b>(i, j)[k] - E[k], 3);
	}

	for (int k = 0; k < 3; k++)
	{
		delta[k] = sqrt(delta[k] / (sourceImg.rows*sourceImg.cols - 1));
		s[k] = pow(abs(delta[k] / (sourceImg.rows*sourceImg.cols - 1)), (double)1 / 3);

	}
	//cout << delta[2] << endl;
	momentVector.resize(9);

	for (int i = 0; i < 3; i++)
	{
		momentVector[3 * i] = E[2 - i] / 255.f;
		momentVector[3 * i + 1] = delta[2 - i] / 255.f;
		momentVector[3 * i + 2] = s[2 - i] / 255.f;
	}
	//cout << "\tMoment#1        Moment#2        Moment#3" << endl;
	//for (int i = 0; i < 9; i++)
	//{
	//	if (i == 0)
	//		cout << "R\t";
	//	if (i == 3)
	//		cout << "G\t";
	//	if (i == 6)
	//		cout << "B\t";

	//	cout << result[i] << "\t";
	//	if (i % 3 == 2)
	//		cout << endl;
	//}
	return 0;
}
