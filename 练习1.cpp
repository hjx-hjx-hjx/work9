#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
using namespace std;
using namespace cv;
float norml2(float* a,float* b,int n) {
	float dis = 0;
	for (int i = 0; i < n; i++) {
		dis = (a[i] - b[i]) * (a[i] - b[i]);
	}
	return sqrt(dis);
}
int calcHOG(Mat src,float* hist,int nAngle,int cellSize) {
	Mat gx, gy;
	Mat mag, angle;
	Sobel(src, gx, CV_32F, 1, 0, 1);
	Sobel(src, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, true);
	Rect roi;
	roi.height = cellSize;
	roi.width = cellSize;
	int nX = src.cols / cellSize;
	int nY = src.rows / cellSize;
	for (int i = 0; i < nY; i++) {
		for (int j = 0; j < nX; j++) {
			Mat roiMat,roiMag,roiAgl;
			roi.x = j * cellSize;
			roi.y = i * cellSize;
			roiMat = src(roi);
			roiMag = mag(roi);
			roiAgl = angle(roi);
			int head = (i *nX + j)*nAngle ;
			for (int n = 0; n < roiMat.rows; n++) {
				for (int m = 0; m < roiMat.cols; m++) {
					hist[head+ (int)roiAgl.at<float>(n, m) / (360 / nAngle)] += roiMag.at<float>(n, m);
				}
			}
		}
	}
	return 0;
}
int main() {
	int cellSize = 16,nAngle=8;
	Mat refMat = imread("D:\\pic\\refMat.jpg");
	Mat plMat = imread("D:\\pic\\plMat.jpg");
	Mat bgMat = imread("D:\\pic\\bgMat.jpg");
	int nX = refMat.cols / cellSize;
	int nY = refMat.rows / cellSize;
	int bins = nX * nY * nAngle;
	float* ref_hist = new float[bins];
	memset(ref_hist, 0, sizeof(float) * bins);
	float* pl_hist = new float[bins];
	memset(pl_hist, 0, sizeof(float) * bins);
	float* bg_hist = new float[bins];
	memset(bg_hist, 0, sizeof(float) * bins);
	int reCode = 0;
	reCode = calcHOG(refMat, ref_hist, nAngle, cellSize);
	reCode = calcHOG(plMat, pl_hist, nAngle, cellSize);
	reCode = calcHOG(bgMat, bg_hist, nAngle, cellSize);
	if (reCode != 0) {
		return -1;
	}
	float dis1 = norml2(ref_hist, pl_hist, bins);
	float dis2 = norml2(ref_hist, bg_hist, bins);
	cout << dis1 << endl;
	cout << dis2 << endl;
	delete[] ref_hist;
	delete[] pl_hist;
	delete[] bg_hist;
	return 0;
}