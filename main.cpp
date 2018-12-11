#include <iostream>
#include <time.h>
#include "slic.h"

using namespace cv;
using namespace std;

int main(int argc, char **args)
{
	// �����㷨�������
	Mat img, dstimg;
	int numSuperPixel, nc, iter;
	double threshold;
	string fileName = "../data/person.png";
	
	if (argc == 1)
	{
		// �ֶ����ò���
		img = imread(fileName);
		numSuperPixel = 1024;
		nc = 30;
		iter = 10;
		threshold = 1e-2;
	}
	else
	{
		// ���������ò���
		img = imread(args[1]);
		numSuperPixel = atoi(args[2]);
		nc = atoi(args[3]);
		iter = atoi(args[4]);
		threshold = atof(args[5]);
	}
	
	// չʾԭʼͼƬ
	imshow("raw", img);

	clock_t start = clock();
	// ��ʼ���г����طָ�

	SLIC slic(img, numSuperPixel, nc, iter, threshold);
	slic.generateSuperPixel();
	slic.colour_with_cluster_means(img, dstimg, superPixelWithCenters);	
	clock_t end = clock();
	cout << "run: " << static_cast<double>(end - start) << " ms" << endl;

	// ����һ�������������ŵĴ��ڣ�Ĭ�ϲ���������Ӧ���޷���ͼƬ��������
	namedWindow("slic", WINDOW_NORMAL);
	imshow("slic", dstimg);
	string predName = fileName.insert(fileName.find_last_of('.'), "-slic");
	imwrite(predName, dstimg);

	waitKey();
	return 0;
}