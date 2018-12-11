#include <iostream>
#include <time.h>
#include "slic.h"

using namespace cv;
using namespace std;

int main(int argc, char **args)
{
	// 定义算法输入参数
	Mat img, dstimg;
	int numSuperPixel, nc, iter;
	double threshold;
	string fileName = "../data/person.png";
	
	if (argc == 1)
	{
		// 手动设置参数
		img = imread(fileName);
		numSuperPixel = 1024;
		nc = 30;
		iter = 10;
		threshold = 1e-2;
	}
	else
	{
		// 命令行设置参数
		img = imread(args[1]);
		numSuperPixel = atoi(args[2]);
		nc = atoi(args[3]);
		iter = atoi(args[4]);
		threshold = atof(args[5]);
	}
	
	// 展示原始图片
	imshow("raw", img);

	clock_t start = clock();
	// 开始进行超像素分割

	SLIC slic(img, numSuperPixel, nc, iter, threshold);
	slic.generateSuperPixel();
	slic.colour_with_cluster_means(img, dstimg, superPixelWithCenters);	
	clock_t end = clock();
	cout << "run: " << static_cast<double>(end - start) << " ms" << endl;

	// 创建一个可以自由缩放的窗口，默认参数是自适应，无法对图片进行缩放
	namedWindow("slic", WINDOW_NORMAL);
	imshow("slic", dstimg);
	string predName = fileName.insert(fileName.find_last_of('.'), "-slic");
	imwrite(predName, dstimg);

	waitKey();
	return 0;
}