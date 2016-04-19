#include "TextureFeaturer.h"

/*Constructor*/
TamuraFeature::TamuraFeature() {}

/*Destructor*/
TamuraFeature::~TamuraFeature() {}

/*
**
* This method is called to get derectionality of the image
* @param sourceImage input image
* @return directionality value of the image
*		if cannot calculate the directionality then return -1
**
*/
double TamuraFeature::Directionality(const Mat& sourceImage, int r, int bins)
{
	if (!sourceImage.data) {
		cout << "Cannot read the image!" << endl;
		return -1;
	}

	Mat grayImg;
	if (sourceImage.channels() == 3) {
		cvtColor(sourceImage, grayImg, CV_BGR2GRAY);
	}
	else {
		grayImg = sourceImage.clone();
	}

	Mat prewittX = Mat::zeros(grayImg.size(), CV_16S);
	Mat prewittY = Mat::zeros(grayImg.size(), CV_16S);
	Mat absPrewittX = Mat::zeros(grayImg.size(), CV_16S);
	Mat absPrewittY = Mat::zeros(grayImg.size(), CV_16S);

	for (int y = 1; y < grayImg.rows - 1; ++y) {
		for (int x = 1; x < grayImg.cols - 1; ++x) {
			prewittX.at<uchar>(y, x) = this->PrewittX(grayImg, x, y);
			prewittY.at<uchar>(y, x) = this->PrewittY(grayImg, x, y);
		}
	}

	vector<pair<Mat, Mat>> gradientImages;
	gradientImages.push_back(pair<Mat, Mat>(prewittX, prewittY));

	//create edge oriented histogram
	vector<float> edgeHist(bins, 0);
	int intervalDegree = 180.f / bins;
	short* pixelX = gradientImages[0].second.ptr<short>(0);
	short* pixelY = gradientImages[0].first.ptr<short>(0);
	for (int i = 0; i < grayImg.rows * grayImg.cols; ++i) {
		double directionRAD = atan2(pixelY[i], pixelX[i]);
		int directionDEG = static_cast<int>(directionRAD / PI * 90.f);
		double gradient = (abs(pixelX[i]) + abs(pixelY[i])) / 2.0;
		if (gradient > LIMIT) 
			++edgeHist[directionDEG / intervalDegree];
	}

	//finding local max
	vector<vector<int>> maxPoints;
	for (int w = 1; w < bins; ++w) {
		for (int i = 0; i < bins; ++i) {
			for (int j = i; j < i + w; ++j) {
				if (j >= bins - 1) break;
				vector<int> t(4);
				if (j == 0) {
					if ((float)edgeHist[j] / (float)edgeHist[i + w] > 2) {
						t[0] = j;
						t[1] = w;
						t[2] = j;
						t[3] = i + w;
						maxPoints.push_back(t);
					}
				}
				else if (j < bins - 1) {
					int z = i + w;
					if (z > bins - 1) {
						z = bins - 1;
					}
					if (((float)edgeHist[j] / (float)edgeHist[i] > 2) && ((float)edgeHist[j] / (float)edgeHist[z] > 2)) {
						t[0] = j;
						t[1] = w;
						t[2] = i;
						t[3] = z;
						maxPoints.push_back(t);
					}
				}

			}
		}
	}

	//sort 
	for (int i = 0; i < maxPoints.size() - 1; ++i) {
		for (int j = i + 1; j < maxPoints.size(); ++j) {
			if (maxPoints[i][0] > maxPoints[j][0]) {
				vector<int> t;
				t = maxPoints[i];
				maxPoints[i] = maxPoints[j];
				maxPoints[j] = t;
			}
		}
	}
	//delete not localPoints
	for (int i = 0; i < maxPoints.size(); ++i) {
		for (int j = 0; j < maxPoints.size(); ++j) {
			if (i != 0 && j == 0) continue;
			if (maxPoints[i][0] == 0) {
				if (maxPoints[i][0] == maxPoints[j][0]) {
					if (maxPoints[i][1] < maxPoints[j][1]) {
						maxPoints[j][1] = 0;
					}
				}
			}
			else {
				if (maxPoints[i][0] == maxPoints[j][0]) {
					if (maxPoints[i][1] > maxPoints[j][1]) {
						maxPoints[j][1] = 0;
					}
				}
			}
		}
	}

	for (int i = 0; i < maxPoints.size(); ++i) {
		if (maxPoints[i][1] == 0) {
			maxPoints.erase(maxPoints.begin() + i);
			--i;
		}
	}

	for (int i = 0; i < maxPoints.size() - 1; ++i) {
		if (maxPoints[i][0] != maxPoints[i + 1][0] && maxPoints[i][3] != maxPoints[i + 1][2]) {
			maxPoints.erase(maxPoints.begin() + i + 1);
			--i;
		}
	}

	/*for (int i = 0; i < maxPoints.size(); ++i) {
		for (int j = 0; j < maxPoints[i].size(); ++j) {
			cout << maxPoints[i][j] << " ";
		}
		cout << endl;
	}*/

	//calculate Fdir
	float Fdir = 1.0;

	int sum = 0;
	for (int i = 0; i < maxPoints.size(); ++i) {
		for (int j = 0; j < edgeHist.size(); j += maxPoints[i][1]) {
			for (int k = 0; k < maxPoints[i][1]; ++k) {
				sum += (j + k - maxPoints[i][0]) * (j + k - maxPoints[i][0]) * edgeHist[j + k];
			}
		}
	}
	
	Fdir -= r * maxPoints.size() * sum;
	return Fdir;
}

/*
**
* This method is called to get linelikeness of the image
* @param sourceImage	input image
* @return linelikeness value of the image
**
*/
double TamuraFeature::Linelikeness(const Mat& sourceImage)
{
	if (!sourceImage.data) {
		cout << "Cannot read the image" << endl;
		return -1;
	}

	Mat grayImg;
	if (sourceImage.channels() == 3) {
		cvtColor(sourceImage, grayImg, CV_BGR2GRAY);
	}
	else {
		grayImg = sourceImage.clone();
	}

	Mat sobelX = Mat::zeros(grayImg.size(), CV_16S);
	Mat sobelY = Mat::zeros(grayImg.size(), CV_16S);
	Mat absSobelX = Mat::zeros(grayImg.size(), CV_16S);
	Mat absSobelY = Mat::zeros(grayImg.size(), CV_16S);

	Sobel(grayImg, sobelX, CV_16S, 1, 0);
	Sobel(grayImg, sobelY, CV_16S, 0, 1);

	convertScaleAbs(sobelX, absSobelX);
	convertScaleAbs(sobelY, absSobelY);

	Mat gradientImage = Mat::zeros(grayImg.size(), CV_32F);
	Mat directionImage = Mat::zeros(grayImg.size(), CV_16S);
	for (int i = 0; i < grayImg.rows * grayImg.cols; ++i) {
		gradientImage.at<float>(i) = sqrtf(absSobelX.at<float>(i) * absSobelX.at<float>(i) +absSobelY.at<float>(i) * absSobelY.at<float>(i));
		double directionRad = atan2(absSobelY.at<short>(i), absSobelX.at<short>(i));
		directionImage.at<int>(i) = static_cast<int>(directionRad / PI * 180.f);
	}

	Mat co_occurenceMat = Mat::zeros(cvSize(grayImg.cols * grayImg.rows, grayImg.cols * grayImg.rows), )
	return 0.0;
}


/*Horizontal matrix*/ 
//-1 0 1
//-1 0 1
//-1 0 1

/*
**
* This method is called to get x gradient using Prewitt operator
* @param sourceImage input grayscale image
* @param x x-coordinate of the pixel
* @param y y-coordinate of the pixel
* @return X gradient value of the pixel(x,y)
**
*/
double TamuraFeature::PrewittX(const Mat& sourceImage, int x, int y)
{
	return - sourceImage.at<uchar>(y + 1, x + 1) + sourceImage.at<uchar>(y + 1, x - 1)
		- sourceImage.at<uchar>(y, x + 1) + sourceImage.at<uchar>(y, x - 1)
		- sourceImage.at<uchar>(y - 1, x + 1) + sourceImage.at<uchar>(y - 1, x - 1);
}

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
* @return Y gradient value of the pixel(x,y)
**
*/
double TamuraFeature::PrewittY(const Mat& sourceImage, int x, int y)
{
	return - sourceImage.at<uchar>(y + 1, x + 1) - sourceImage.at<uchar>(y + 1, x) - sourceImage.at<uchar>(y + 1, x - 1)
		+ sourceImage.at<uchar>(y - 1, x + 1) + sourceImage.at<uchar>(y - 1, x) + sourceImage.at<uchar>(y - 1, x - 1);
}