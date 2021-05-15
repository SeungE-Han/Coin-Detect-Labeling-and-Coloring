#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat img = imread("lena.jpg", IMREAD_GRAYSCALE);
	CV_Assert(img.data);

	Size size(5, 5); //����ũ ũ��
	double sigmaX = 0.3*((size.width - 1)*0.5 - 1) + 0.8; //���ι��� ǥ������
	double sigmaY = 0.3*((size.height - 1)*0.5 - 1) + 0.8; //���ι��� ǥ������
	Mat gau_img; //����þ� ���� �� ����
	GaussianBlur(img, gau_img, size, sigmaX, sigmaY);

	Mat dx, dy, sobel;
	Sobel(gau_img, dx, CV_32F, 1, 0, 3); //x���� �̺� -> ���� ����ũ
	Sobel(gau_img, dy, CV_32F, 0, 1, 3); //y���� �̺� -> ���� ����ũ
	magnitude(dx, dy, sobel); //ũ�� ����

	convertScaleAbs(dx, dx); //���� �� uchar ����ȯ
	convertScaleAbs(dy, dy);
	convertScaleAbs(sobel, sobel);
	
	imshow("dx", dx);
	imshow("dy", dy);
	imshow("Sobel", sobel);
	waitKey(0);
}