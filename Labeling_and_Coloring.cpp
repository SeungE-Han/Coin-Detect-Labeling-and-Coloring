#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat img = imread("lena.jpg", IMREAD_GRAYSCALE);
	CV_Assert(img.data);

	Size size(5, 5); //마스크 크기
	double sigmaX = 0.3*((size.width - 1)*0.5 - 1) + 0.8; //가로방향 표준편차
	double sigmaY = 0.3*((size.height - 1)*0.5 - 1) + 0.8; //세로방향 표준편차
	Mat gau_img; //가우시안 블러링 후 영상
	GaussianBlur(img, gau_img, size, sigmaX, sigmaY);

	Mat dx, dy, sobel;
	Sobel(gau_img, dx, CV_32F, 1, 0, 3); //x방향 미분 -> 수직 마스크
	Sobel(gau_img, dy, CV_32F, 0, 1, 3); //y방향 미분 -> 수평 마스크
	magnitude(dx, dy, sobel); //크기 구함

	convertScaleAbs(dx, dx); //절댓값 및 uchar 형변환
	convertScaleAbs(dy, dy);
	convertScaleAbs(sobel, sobel);
	
	imshow("dx", dx);
	imshow("dy", dy);
	imshow("Sobel", sobel);
	waitKey(0);
}