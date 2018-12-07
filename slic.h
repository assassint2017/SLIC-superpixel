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
	
	// ����ģʽ
	enum drawMode { 
		superPixelOnly,  // ֻ��ʾ�����طָ���
		superPixelWithCenters,  // ��ʾ�����طָ����Լ��������ĵ�
		superPixelWithEdge,  // ��ʾ�����طָ����Լ�����Ե
		superPixelWithEdgeAndCenters,  // ��ʾ�����طָ���������Ե�Լ��������ĵ�
	};

	/*
	@param img : ͼ�����
	@param numSuperPixel : �����صĸ���
	@param nc : �������������ԵĹ�һ������
	@param iter : �㷨�����������
	@param threshold : �в���ֵ
	*/
	SLIC(const cv::Mat &img, int numSuperPixel, int nc, int iter, int threshold);
	~SLIC();

	// ���ɳ�����
	void generateSuperPixel();
	
	// �þ���õ�����ƽ��ɫ��ֵ������ͼ������ԭ����ɫ��
	void colour_with_cluster_means(const cv::Mat &img, cv::Mat &showimg, int flag = 0);

private:
	cv::Mat labImg;
	double step;
	int nc;
	double ns;
	int iter;
	double threshold;
		
	Vecdd centers;  // ��ž������ά����
	Vecdd distances;  // ���ͼ����ÿһ�����ص������Ծ���
	Vecii labels;  // ���ͼ����ÿһ���������ڵľ�����������
	std::vector<int> numLabels;  // ���ÿһ���������ı��������������

	// ��ʼ������
	void init();

	// ���������ݶ���С�㺯��
	cv::Point findLocalMinimum(cv::Point);

	// �������������Ժ���
	double computeDis(int, const cv::Point&, double, double, double);

	// ��ͼ���ϻ��ƾ������ĵ�
	void showCenters(cv::Mat &img);

	// ��ͼ���ϻ��ƾ�������Ե
	void showEdges(cv::Mat &img);

	// ����ɳ����طָ��ϲ�������
	void mergeOutliers();
};
