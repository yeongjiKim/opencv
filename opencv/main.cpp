#include <opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "OpenFile.h"
#include "Teeth.h"

using namespace std;
using namespace cv;

void CallBackFunc(int event, int x, int y, int flags, void* userdata) {
	FILE *fp = fopen("result.txt", "a+");
	if (event == EVENT_LBUTTONDOWN) {
		cout << "왼쪽 마우스 버튼 클릭.. 좌표 = (" << x << ", " << y << ")" << endl;
		fprintf(fp, " 사람인식 : mouse on down = (%3d,%3d)", x, y);
	}
	else if (event == CV_EVENT_LBUTTONUP) {
		cout << "오른쪽 마우스 버튼 클릭..좌표 = (" << x << ", " << y << ")" << endl;
		fprintf(fp, "mouse on up = (%3d,%3d)\r\n", x, y);
	}
	fclose(fp);
}
int main(int argc, char** argv){
	cout << "TeethAR [filename] [K;value of color] [D;reconginition dist] [DebugMode;1or0]" << endl;
	cout << CV_VERSION << endl;
	Mat img;
	Mat img1;
	int cnt = 0;
	int K = 200;
	int dist = 100;
	bool debug = false;
	string sFilename;
	char* filepath;
	if (argc > 1)
	{
		string s = argv[0];
		int x = s.find_last_of('\\');
		string filename = s.substr(0, x + 1) + argv[1];
		cout << filename << endl;
		img = imread(filename);
		if (!img.size().width) { cout << "This file is not exist." << endl; exit(1); }
	}
	if (argc > 2)
	{
		K = atoi(argv[2]);
		cout << "K=" << K << endl;
	}
	if (argc > 3)
	{
		dist = atoi(argv[3]);
		cout << "Dist=" << dist << endl;
	}
	if (argc > 4) {
		if (atoi(argv[4]) > 0)
			debug = true;
		else
			debug = false;
		cout << "DebugMOde=" << debug << endl;
	}
	if (argc<2) {
		do {
			string filename = OpenFileDialogA();
			cout << filename << endl;
			sFilename = filename;
			if (filename != "") {img = imread(filename);break;}
			else { cout << "arUkiddingMe?" << endl; if (++cnt > 2)exit(1); }
		} while (1);
	}
	filepath = (char*)sFilename.c_str();
	Canny(img, img1, 50, 120, 3);

	imshow("original", img);
	imshow("canny", img1);
	Teeth t(img, K, dist);
	t.DrawRect(debug);
	imshow("teeth", img);
	//윈도우에 콜백함수를 등록
	setMouseCallback("teeth", CallBackFunc, NULL);

	FILE *fp = fopen("result.txt", "a+");
	fprintf(fp, "%s K=%d Dist=%d 기계인식 : Min(%3d,%3d) Max(%3d,%3d)",filepath,K,dist,t.minX,t.minY,t.maxX,t.maxY);
	fclose(fp);

	waitKey(0);
	return 0;
}



