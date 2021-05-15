#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;



int main() {
	Mat src = imread("coins.jpg", IMREAD_GRAYSCALE); //���� �о��
	CV_Assert(src.data);
	Mat gau_img, bin1, morpimg;

	Size size(3, 3);
	double sigmaX = 0.3*((size.width - 1)*0.5 - 1) + 0.8;
	double sigmaY = 0.3*((size.height - 1)*0.5 - 1) + 0.8;
	GaussianBlur(src, gau_img, size, sigmaX, sigmaY); //����þ� ����

	int th1 = (int)threshold(gau_img, bin1, 0, 255, THRESH_BINARY | THRESH_OTSU); //Otsu�˰��� ���� �ڵ� ����ȭ
	cout << th1 << endl;
	
	Matx <uchar, 3, 3> mask; //Matx��ü�� �̿���
	mask << 0, 1, 0, 1, 1, 1, 0, 1, 0; //�ʱ�ȭ

	morphologyEx(bin1, morpimg, MORPH_CLOSE, mask); //���� �Լ�(��â -> ħ��)

	Mat labels, stats, centroids;
	int cnt = connectedComponentsWithStats(morpimg, labels, stats, centroids); //���̺��� ��

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR); //Į��� ��ȯ
	
	char a[100] = { "A" };
	String b[100];
	
	srand(unsigned(time(NULL)));

	for (int i = 1; i < cnt; i++) {
		int* p = stats.ptr<int>(i); //�������
		double* q = centroids.ptr<double>(i); //�����߽�

		if (p[4] > 10) {
			rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(0, 255, 255), 2); //�簢�� �׸���
			b[i - 1] = char(a[0] + i - 1); //���ĺ� ����
			putText(dst, b[i - 1], Point(q[0], q[1]), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255)); //����
		}
	}

	Size sz(labels.cols, labels.rows);
	Mat dst2(sz, CV_8UC3, Scalar(0,0,0)), tmp;

	sortIdx(stats, tmp, SORT_EVERY_COLUMN | SORT_DESCENDING); //������������ ����
	
	for (int k = 1; k < cnt; k++) {
		int* p = stats.ptr<int>(k);  //�������
		double* q = centroids.ptr<double>(k); //�����߽�
		int n[] = { rand() % 256 ,rand() % 256 ,rand() % 256 }; //��ĥ->�����Լ��� �̿��Ͽ� ���Ƿ� �� ����
		for (int i = 0; i < labels.rows; i++) {
			for (int j = 0; j < labels.cols; j++) {
				if (labels.at<int>(i, j) == k) 
					dst2.at<Vec3b>(i, j) = Vec3b(n[0], n[1], n[2]);		
			}
		}

		for (int i = 1; i < cnt; i++) {
			if (tmp.at<int>(i, 4) == k)
				putText(dst2, b[i - 1], Point(q[0], q[1]), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255)); //ũ��� ���ĺ�
		}
	}
	
	imshow("src", src);
	imshow("gau_img", gau_img);
	imshow("bin1", bin1);
	imshow("morpimg", morpimg);
	imshow("dst", dst);
	imshow("dst2", dst2);
	waitKey(0);
}