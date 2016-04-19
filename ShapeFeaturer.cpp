#include "ShapeFeaturer.h" 

//Mat ShapeFeature::get_hogdescriptor_visual_image(Mat& origImg,
//	vector<float>& descriptorValues,
//	Size winSize,
//	Size cellSize,
//	int scaleFactor,
//	double viz_factor)
//{
//	Mat visual_image;
//	resize(origImg, visual_image, Size(512, 512));
//
//	int gradientBinSize = 9;
//	// dividing 180° into 9 bins, how large (in rad) is one bin?
//	float radRangeForOneBin = 3.14 / (float)gradientBinSize;
//
//	// prepare data structure: 9 orientation / gradient strenghts for each cell
//	int cells_in_x_dir = winSize.width / cellSize.width;
//	int cells_in_y_dir = winSize.height / cellSize.height;
//	int totalnrofcells = cells_in_x_dir * cells_in_y_dir;
//	float*** gradientStrengths = new float**[cells_in_y_dir];
//	int** cellUpdateCounter = new int*[cells_in_y_dir];
//	for (int y = 0; y<cells_in_y_dir; y++)
//	{
//		gradientStrengths[y] = new float*[cells_in_x_dir];
//		cellUpdateCounter[y] = new int[cells_in_x_dir];
//		for (int x = 0; x<cells_in_x_dir; x++)
//		{
//			gradientStrengths[y][x] = new float[gradientBinSize];
//			cellUpdateCounter[y][x] = 0;
//
//			for (int bin = 0; bin<gradientBinSize; bin++)
//				gradientStrengths[y][x][bin] = 0.0;
//		}
//	}
//
//	// nr of blocks = nr of cells - 1
//	// since there is a new block on each cell (overlapping blocks!) but the last one
//	int blocks_in_x_dir = cells_in_x_dir - 1;
//	int blocks_in_y_dir = cells_in_y_dir - 1;
//
//	// compute gradient strengths per cell
//	int descriptorDataIdx = 0;
//	int cellx = 0;
//	int celly = 0;
//
//	for (int blockx = 0; blockx<blocks_in_x_dir; blockx++)
//	{
//		for (int blocky = 0; blocky<blocks_in_y_dir; blocky++)
//		{
//			// 4 cells per block ...
//			for (int cellNr = 0; cellNr<4; cellNr++)
//			{
//				// compute corresponding cell nr
//				int cellx = blockx;
//				int celly = blocky;
//				if (cellNr == 1) celly++;
//				if (cellNr == 2) cellx++;
//				if (cellNr == 3)
//				{
//					cellx++;
//					celly++;
//				}
//
//				for (int bin = 0; bin<gradientBinSize; bin++)
//				{
//					float gradientStrength = descriptorValues[descriptorDataIdx];
//					descriptorDataIdx++;
//
//					gradientStrengths[celly][cellx][bin] += gradientStrength;
//
//				} // for (all bins)
//
//
//				// note: overlapping blocks lead to multiple updates of this sum!
//				// we therefore keep track how often a cell was updated,
//				// to compute average gradient strengths
//				cellUpdateCounter[celly][cellx]++;
//
//			} // for (all cells)
//
//
//		} // for (all block x pos)
//	} // for (all block y pos)
//
//
//	// compute average gradient strengths
//	for (int celly = 0; celly<cells_in_y_dir; celly++)
//	{
//		for (int cellx = 0; cellx<cells_in_x_dir; cellx++)
//		{
//
//			float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];
//
//			// compute average gradient strenghts for each gradient bin direction
//			for (int bin = 0; bin<gradientBinSize; bin++)
//			{
//				gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
//			}
//		}
//	}
//
//
//	cout << "descriptorDataIdx = " << descriptorDataIdx << endl;
//
//	// draw cells
//	for (int celly = 0; celly<cells_in_y_dir; celly++)
//	{
//		for (int cellx = 0; cellx<cells_in_x_dir; cellx++)
//		{
//			int drawX = cellx * cellSize.width;
//			int drawY = celly * cellSize.height;
//
//			int mx = drawX + cellSize.width / 2;
//			int my = drawY + cellSize.height / 2;
//
//			rectangle(visual_image,
//				Point(drawX*scaleFactor, drawY*scaleFactor),
//				Point((drawX + cellSize.width)*scaleFactor,
//				(drawY + cellSize.height)*scaleFactor),
//				CV_RGB(100, 100, 100),
//				1);
//
//			// draw in each cell all 9 gradient strengths
//			for (int bin = 0; bin<gradientBinSize; bin++)
//			{
//				float currentGradStrength = gradientStrengths[celly][cellx][bin];
//
//				// no line to draw?
//				if (currentGradStrength == 0)
//					continue;
//
//				float currRad = bin * radRangeForOneBin + radRangeForOneBin / 2;
//
//				float dirVecX = cos(currRad);
//				float dirVecY = sin(currRad);
//				float maxVecLen = cellSize.width / 2;
//				float scale = viz_factor; // just a visual_imagealization scale,
//				// to see the lines better
//
//				// compute line coordinates
//				float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
//				float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
//				float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
//				float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;
//
//				// draw gradient visual_imagealization
//				line(visual_image,
//					Point(x1*scaleFactor, y1*scaleFactor),
//					Point(x2*scaleFactor, y2*scaleFactor),
//					CV_RGB(0, 0, 255),
//					1);
//
//			} // for (all bins)
//
//		} // for (cellx)
//	} // for (celly)
//
//
//	// don't forget to free memory allocated by helper data structures!
//	for (int y = 0; y<cells_in_y_dir; y++)
//	{
//		for (int x = 0; x<cells_in_x_dir; x++)
//		{
//			delete[] gradientStrengths[y][x];
//		}
//		delete[] gradientStrengths[y];
//		delete[] cellUpdateCounter[y];
//	}
//	delete[] gradientStrengths;
//	delete[] cellUpdateCounter;
//
//	return visual_image;
//
//}

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
int ShapeFeature::EDH(const Mat& sourceImage, double*& result)
{
	if (sourceImage.empty()) return 1;

	Mat src_gray, destinationImage;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	int c;
	int nEi = 0;
	result = new double[73];
	for (int i = 0; i < 73; i++)
	{
		result[i] = 0;
	}
	GaussianBlur(sourceImage, sourceImage, Size(3, 3), 0, 0, BORDER_DEFAULT);
	/// Convert it to gray
	cvtColor(sourceImage, src_gray, CV_BGR2GRAY);
	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	grad_x.release();
	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	grad_y.release();
	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, destinationImage);
	for (int i = 0; i < destinationImage.rows; i++)
	for (int j = 0; j < destinationImage.cols; j++)
	{
		if (destinationImage.at<uchar>(i, j) == 0)
			continue;
		nEi++;
		double x, y;
		if (abs_grad_x.at<uchar>(i, j) < 128)
			x = abs_grad_x.at<uchar>(i, j)*-1.0;
		else x = abs_grad_x.at<uchar>(i, j) % 128;

		if (abs_grad_y.at<uchar>(i, j) < 128)
			y = abs_grad_y.at<uchar>(i, j)*-1.0;
		else y = abs_grad_y.at<uchar>(i, j) % 128;
		float Slope = atan2(y, x) * 180 / 3.14;
		if ((x < 0) && (y < 0))
		{
			Slope += 360.0;
		}
		if ((x > 0) && (y < 0))
		{
			Slope = atan2(-y, x) * 180 / 3.14;
			Slope = Slope + 270.0;
		}
		Slope = (int)Slope % 360;

		int numBin = ((int)Slope / 5);
		result[numBin] += 1.0;
	}
	//cout << "AAA   ";
	for (int i = 0; i < 72; i++)
	{
		result[i] *= 1.0 / nEi;
		if (result[i] <= 0.0001) result[i] = 0;
	}

	double Npixel = destinationImage.rows*destinationImage.cols;
	result[72] = (Npixel - nEi) / Npixel;
	abs_grad_x.release();
	abs_grad_y.release();
	destinationImage.release();
	return 0;
}

/*
**
* This method is called to get HOG descriptors
* @param sourceImage	inputImage
* @param hogDescriptors	extracted HOG vector
**
*/
void ShapeFeature::getHOGFeatures(const Mat& sourceImage, vector<float>& hogDescriptors)
{
	if (!sourceImage.data) {
		cout << "Cannot read the image!" << endl;
		return;
	}

	if (sourceImage.channels() == 3) {
		cout << "Input image is not a grayscale image!" << endl;
		return;
	}

	Mat resizedImage;
	//scale image
	resize(sourceImage, resizedImage, Size(MAXWIDTH, MAXHEIGHT));
	
	int cellsInXDir = sourceImage.cols / CELLSIZE;
	int cellsInYDir = sourceImage.rows / CELLSIZE;

	uchar* pResized = resizedImage.data;
	//get cells
	vector<Mat> cellImages;
	for (int i = 0; i < MAXWIDTH * MAXHEIGHT; i += CELLSIZE * MAXWIDTH) {
		for (int y = i; y < MAXHEIGHT; y += CELLSIZE) {
			for (int x = 0; x < MAXWIDTH; x += CELLSIZE) {
				Mat temp = Mat::zeros(Size(CELLSIZE, CELLSIZE), CV_16S);
				uchar* pTemp = temp.data;
				for (int j = 0; j < CELLSIZE; ++j) {
					for (int k = 0; k < CELLSIZE; ++k) {
						pTemp[j * CELLSIZE + k] = pResized[y + x + MAXWIDTH * j + k];
					}
				}
				cellImages.push_back(temp);
			}
		}
	}

	/*cout << cellImages.size();*/

	//first is gradientX, second is gradientY
	vector<pair<Mat, Mat>> gradientImages;
	for (int i = 0; i < cellImages.size(); ++i) {
		Mat sobelX = Mat::zeros(Size(CELLSIZE, CELLSIZE), CV_16S);
		Mat sobelY = Mat::zeros(Size(CELLSIZE, CELLSIZE), CV_16S);
		Mat absSobelX = Mat::zeros(Size(CELLSIZE, CELLSIZE), CV_16S);
		Mat absSobelY = Mat::zeros(Size(CELLSIZE, CELLSIZE), CV_16S);
		
		//gradientX
		Sobel(cellImages[i], sobelX, CV_16S, 1, 0);

		//gradientY
		Sobel(cellImages[i], sobelY, CV_16S, 0, 1);
		gradientImages.push_back(pair<Mat, Mat>(sobelX, sobelY));
	}

	//set the vector each cell
	vector<vector<float>> gradientVectors;
	for (int i = 0; i < gradientImages.size(); ++i) {
		vector<float> temp(9, 0);
		short* pixelX = gradientImages[i].second.ptr<short>(0);
		short* pixelY = gradientImages[i].first.ptr<short>(0);
		for (int j = 0; j < CELLSIZE * CELLSIZE; ++j) {
			double directionRAD = atan2(pixelY[j], pixelX[j]);
			int directionDEG = static_cast<int>(directionRAD / PI * 180.f);
			if (directionDEG < 20) ++temp[0];
			else if (directionDEG < 40) ++temp[1];
			else if (directionDEG < 60) ++temp[2];
			else if (directionDEG < 80) ++temp[3];
			else if (directionDEG < 100) ++temp[4];
			else if (directionDEG < 120) ++temp[5];
			else if (directionDEG < 140) ++temp[6];
			else if (directionDEG < 160) ++temp[7];
			else ++temp[8];
		}

		//normalize histogram
		for (int j = 0; j < temp.size(); ++j)
			temp[j] /= static_cast<float>(CELLSIZE * CELLSIZE);

		gradientVectors.push_back(temp);
	}

	vector<vector<float>> blockVectors;
	for (int i = 0; i < cellsInXDir * cellsInYDir; i += cellsInXDir) {
		for (int y = i; y < cellsInXDir - 1; ++y) {
			for (int x = 0; x < cellsInYDir - 1; ++x) {
				vector<float> temp;
				for (int j = 0; j < 2; ++j) {
					for (int k = 0; k < 2; ++k) {
						for (int l = 0; l < gradientVectors[y + x + j * cellsInXDir + k].size(); ++l) {
							temp.push_back(gradientVectors[y + x + j * cellsInXDir + k][l]);
						}
					}
				}
				blockVectors.push_back(temp);
			}
		}
	}

	//cout << blockVectors.size();

	//extract HOG descriptor
	for (int i = 0; i < blockVectors.size(); ++i) {
		for (int j = 0; j < blockVectors[i].size(); ++j)
			hogDescriptors.push_back(blockVectors[i][j]);
	}
	

}