#include "ColorFeaturer.h"
#include "ShapeFeaturer.h"
#include <math.h>
#include <vector>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;

//void show_result(const Mat& labels, const Mat& centers, int height, int width)
//{
//	cout << "===\n";
//	cout << "labels: " << labels.rows << " " << labels.cols << std::endl;
//	cout << "centers: " << centers.rows << " " << centers.cols << std::endl;
//	assert(labels.type() == CV_32SC1);
//	assert(centers.type() == CV_32FC1);
//
//	Mat rgb_image(height, width, CV_8UC3);
//	MatIterator_<Vec3b> rgb_first = rgb_image.begin<cv::Vec3b>();
//	MatIterator_<Vec3b> rgb_last = rgb_image.end<cv::Vec3b>();
//	MatConstIterator_<int> label_first = labels.begin<int>();
//
//	Mat centers_u8;
//	centers.convertTo(centers_u8, CV_8UC1, 255.0);
//	Mat centers_u8c3 = centers_u8.reshape(3);
//
//	while (rgb_first != rgb_last) {
//		const cv::Vec3b& rgb = centers_u8c3.ptr<cv::Vec3b>(*label_first)[0];
//		*rgb_first = rgb;
//		++rgb_first;
//		++label_first;
//	}
//	imshow("tmp", rgb_image);
//	waitKey(0);
//}

int main()
{
	Mat sourceImage = imread("images/Lena.jpg", IMREAD_UNCHANGED);
	Mat referenceImage = imread("images/c.png", IMREAD_UNCHANGED);
	Mat destinationImage;
	destinationImage.create(referenceImage.size(), sourceImage.type());
	
	vector<Mat> images;
	images.push_back(sourceImage);
	images.push_back(referenceImage);
	
	
	//int arr[9] = { 160, 65, 75, 180, 68, 25, 200, 90, 97 };
	//int bins = 9;
	


	///*Color Feature*/
	//ColorFeature colorFeature;
	//
	//Mat histogram;
	//int bins = 9;

	///*Grayscale histogram*/
	//colorFeature.CalcGrayscaleHistogram(sourceImage, histogram, bins);
	//colorFeature.ShowHistogram(histogram, bins);

	//Mat histogram1;
	///*Color histogram*/
	//colorFeature.CalcColorHistogram(sourceImage, histogram1, bins);
	//colorFeature.ShowHistogram(histogram1, bins);

	//Mat histogram2;
	///*gray histogram of 2 images*/

	//colorFeature.CalcJoinGrayscaleHistogram(images, histogram2, bins);
	//colorFeature.ShowHistogram(histogram2, bins);

	///*color histogram of 2 images*/
	//Mat histogram3;
	//colorFeature.CalcJoinColorHistogram(images, histogram3, bins);
	//colorFeature.ShowHistogram(histogram3, bins);

	///*color moment vector*/
	//vector<float> momentVector;
	//colorFeature.ColorMomentVector(sourceImage, momentVector);

	//cout << "\tMoment#1        Moment#2        Moment#3" << endl;
	//for (int i = 0; i < momentVector.size(); i++)
	//{
	//	if (i == 0)
	//		cout << "R\t";
	//	if (i == 3)
	//		cout << "G\t";
	//	if (i == 6)
	//		cout << "B\t";

	//	cout << momentVector[i] << "\t";
	//	if (i % 3 == 2)
	//		cout << endl;
	//}

	//cout << endl;

	//ShapeFeature shapeFeature;
	///*edge direction histogram*/
	//double* edh;
	//shapeFeature.EDH(sourceImage, edh);
	//cout << "Done EDH " << endl;
	//for (int i = 0; i < 73; i++)
	//{

	//	cout << edh[i] << endl;
	//}
	//cout << endl;
	return 0;
}