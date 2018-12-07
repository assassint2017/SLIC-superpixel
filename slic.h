#pragma once
#include <math.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

typedef std::vector<std::vector<double>>  Vecdd;
typedef std::vector<std::vector<int>> Vecii;

class SLIC
{
public:
	
	// 绘制模式
	enum drawMode { 
		superPixelOnly,  // 只显示超像素分割结果
		superPixelWithCenters,  // 显示超像素分割结果以及聚类中心点
		superPixelWithEdge,  // 显示超像素分割结果以及蔟边缘
		superPixelWithEdgeAndCenters,  // 显示超像素分割结果、蔟边缘以及聚类中心点
	};

	/*
	@param img : 图像矩阵
	@param numSuperPixel : 超像素的个数
	@param nc : 计算像素相似性的归一化参数
	@param iter : 算法整体迭代次数
	@param threshold : 残差阈值
	*/
	SLIC(const cv::Mat &img, int numSuperPixel, int nc, int iter, int threshold);
	~SLIC();

	// 生成超像素
	void generateSuperPixel();
	
	// 用聚类好的蔟的平均色彩值来代替图像像素原本的色彩
	void colour_with_cluster_means(const cv::Mat &img, cv::Mat &showimg, int flag = 0);

private:
	cv::Mat labImg;
	double step;
	int nc;
	double ns;
	int iter;
	double threshold;
		
	Vecdd centers;  // 存放聚类的五维向量
	Vecdd distances;  // 存放图像中每一个像素的相似性距离
	Vecii labels;  // 存放图像中每一个像素属于的聚类中心索引
	std::vector<int> numLabels;  // 存放每一个聚类中心被分配的像素数量

	// 初始化函数
	void init();

	// 计算邻域梯度最小点函数
	cv::Point findLocalMinimum(cv::Point);

	// 计算像素相似性函数
	double computeDis(int, const cv::Point&, double, double, double);

	// 在图像上绘制聚类中心点
	void showCenters(cv::Mat &img);

	// 在图像上绘制聚类蔟边缘
	void showEdges(cv::Mat &img);

	// 在完成超像素分割后合并孤立点
	void mergeOutliers();
};
