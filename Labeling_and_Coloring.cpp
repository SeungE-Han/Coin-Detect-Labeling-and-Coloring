#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;



int main() {
	Mat src = imread("coins.jpg", IMREAD_GRAYSCALE); //영상 읽어옴
	CV_Assert(src.data);
	Mat gau_img, bin1, morpimg;

	Size size(3, 3);
	double sigmaX = 0.3*((size.width - 1)*0.5 - 1) + 0.8;
	double sigmaY = 0.3*((size.height - 1)*0.5 - 1) + 0.8;
	GaussianBlur(src, gau_img, size, sigmaX, sigmaY); //가우시안 블러링

	int th1 = (int)threshold(gau_img, bin1, 0, 255, THRESH_BINARY | THRESH_OTSU); //Otsu알고리즘에 의해 자동 이진화
	cout << th1 << endl;
	
	Matx <uchar, 3, 3> mask; //Matx객체를 이용한
	mask << 0, 1, 0, 1, 1, 1, 0, 1, 0; //초기화

	morphologyEx(bin1, morpimg, MORPH_CLOSE, mask); //닫힘 함수(팽창 -> 침식)

	Mat labels, stats, centroids;
	int cnt = connectedComponentsWithStats(morpimg, labels, stats, centroids); //레이블의 수

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR); //칼라로 전환
	
	char a[100] = { "A" };
	String b[100];
	
	srand(unsigned(time(NULL)));

	for (int i = 1; i < cnt; i++) {
		int* p = stats.ptr<int>(i); //통계정보
		double* q = centroids.ptr<double>(i); //무게중심

		if (p[4] > 10) {
			rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(0, 255, 255), 2); //사각형 그리기
			b[i - 1] = char(a[0] + i - 1); //알파벳 증가
			putText(dst, b[i - 1], Point(q[0], q[1]), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255)); //글자
		}
	}

	Size sz(labels.cols, labels.rows);
	Mat dst2(sz, CV_8UC3, Scalar(0,0,0)), tmp;

	sortIdx(stats, tmp, SORT_EVERY_COLUMN | SORT_DESCENDING); //내림차순으로 정렬
	
	for (int k = 1; k < cnt; k++) {
		int* p = stats.ptr<int>(k);  //통계정보
		double* q = centroids.ptr<double>(k); //무게중심
		int n[] = { rand() % 256 ,rand() % 256 ,rand() % 256 }; //색칠->랜드함수를 이용하여 임의로 색 배정
		for (int i = 0; i < labels.rows; i++) {
			for (int j = 0; j < labels.cols; j++) {
				if (labels.at<int>(i, j) == k) 
					dst2.at<Vec3b>(i, j) = Vec3b(n[0], n[1], n[2]);		
			}
		}

		for (int i = 1; i < cnt; i++) {
			if (tmp.at<int>(i, 4) == k)
				putText(dst2, b[i - 1], Point(q[0], q[1]), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255)); //크기순 알파벳
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