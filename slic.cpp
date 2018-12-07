#include "slic.h"

using namespace cv;
using namespace std;

SLIC::SLIC(const Mat &img, int numSuperPixel, int nc, int iter, int threshold)
{
	cout << "ͼ���С: " << img.rows << " * " << img.cols << endl;

	// ��ͼ���RGB�ռ�ת����Lab�ռ�
	cvtColor(img, SLIC::labImg, CV_BGR2Lab);

	// ���㲽��
	step = sqrt(static_cast<double>(img.rows * img.cols) / numSuperPixel);
	this->nc = nc;
	ns = step;
	this->iter = iter;
	this->threshold = threshold;
	
	// ��ʼ��
	init();
}

SLIC::~SLIC()
{

}

void SLIC::init()
{
	// ��ʼ��label��distances����
	for (int i = 0; i < labImg.rows; i++)
	{
		vector<int> label;
		vector<double> distance;
		for (int j = 0; j < labImg.cols; j++)
		{
			label.push_back(-1);
			distance.push_back(FLT_MAX);
		}
		labels.push_back(label);
		distances.push_back(distance);
	}

	// ��ʼ����������
	for (int i = step; i < labImg.rows - step / 2; i += step)
	{
		for (int j = step; j < labImg.cols - step / 2; j += step)
		{
			// ����ÿ���������ı�����ĵ�Ⱥ������
			numLabels.push_back(0);

			vector<double> center;
			Point centerPoint = findLocalMinimum(Point(j, i));

			// ����һ����ά���� (L, a, b, x, y)
			center.push_back(static_cast<double>(labImg.at<Vec3b>(centerPoint.y, centerPoint.x)[0]));
			center.push_back(static_cast<double>(labImg.at<Vec3b>(centerPoint.y, centerPoint.x)[1]));
			center.push_back(static_cast<double>(labImg.at<Vec3b>(centerPoint.y, centerPoint.x)[2]));
			center.push_back(centerPoint.x);
			center.push_back(centerPoint.y);

			centers.push_back(center);
		}
	}
	cout << "�������ĵĸ���: " << centers.size() << endl;
}

Point SLIC::findLocalMinimum(Point center)
{
	Point localMinimum(center.x, center.y);
	double minG = 1e5;

	// ��3*3��������Ѱ���ݶ���Сֵ
	for (int i = center.x - 1; i < center.x + 2; i++)
		for (int j = center.y - 1; j < center.y + 2; j++)
		{
			// Lab�ռ䣬L�ķ�Χ��0-100���������ȣ���ab�ķ�Χ����-127��128��������ɫ��
			// �����һ��BGRģ�͵�ͼƬת������LAB�ռ䣬��LAB�ռ��ȡֵ��Χ��Ȼ��0-255����Ϊ�����������������µ�ת��
			// L:L * 255 / 100 
			// a:a + 128 
			// b:b + 128

			double l1 = static_cast<double>(labImg.at<Vec3b>(j + 1, i)[0]);
			double l2 = static_cast<double>(labImg.at<Vec3b>(j - 1, i)[0]);
			double l3 = static_cast<double>(labImg.at<Vec3b>(j, i + 1)[0]);
			double l4 = static_cast<double>(labImg.at<Vec3b>(j, i - 1)[0]);

			// �����ݶ� 
			double g = sqrt(pow(l1 - l2, 2)) + sqrt(pow(l3 - l4, 2));

			if (g < minG)
			{
				minG = g;
				localMinimum.x = i, localMinimum.y = j;
			}
		}	
	return localMinimum;
}

void SLIC::generateSuperPixel()
{
	// �㷨����ָ���Ĵ���
	for (int iterIndex = 0; iterIndex < iter; iterIndex++)
	{
		// ����2S*2S��Χ�ڵ����ص���������
		for (int centerIndex = 0; centerIndex < centers.size(); centerIndex++)
		{
			int x = centers[centerIndex][3];
			int y = centers[centerIndex][4];

			for (int i = y - step > 0 ? y - step : 0; (i < y + step) && (i < labImg.rows); i++)
			{
				for (int j = x - step > 0 ? x - step : 0; (j < x + step) && j < (labImg.cols); j++)
				{
					Point point(j, i);

					double l = labImg.at<Vec3b>(i, j)[0];
					double a = labImg.at<Vec3b>(i, j)[1];
					double b = labImg.at<Vec3b>(i, j)[2];

					double distance = computeDis(centerIndex, point, l, a, b);
					if (distance < distances[i][j])
					{
						distances[i][j] = distance;
						labels[i][j] = centerIndex;
					}
				}
			}
		}

		// �����������������浽��ʱ�����У���������
		Vecdd oldcenters = centers;
		for (int i = 0; i < centers.size(); i++)
		{
			centers[i][0] = 0;
			centers[i][1] = 0;
			centers[i][2] = 0;
			centers[i][3] = 0;
			centers[i][4] = 0;
		}

		// ���ݷ����������¼���������ĵ�����
		for (int i = 0; i < labImg.rows; i++)
		{
			for (int j = 0; j < labImg.cols; j++)
			{
				int centerIndex = labels[i][j];
				if (centerIndex != -1)
				{
					numLabels[centerIndex]++;

					centers[centerIndex][0] += static_cast<double>(labImg.at<Vec3b>(i, j)[0]);
					centers[centerIndex][1] += static_cast<double>(labImg.at<Vec3b>(i, j)[1]);
					centers[centerIndex][2] += static_cast<double>(labImg.at<Vec3b>(i, j)[2]);

					centers[centerIndex][3] += j;
					centers[centerIndex][4] += i;
				}
				// ������ֵ������Ϊ�����
				distances[i][j] = FLT_MAX;
			}
		}

		// �Ծ������Ľ��й�һ�����õ����յ��¾�������
		for (int i = 0; i < centers.size(); i++)
		{
			centers[i][0] /= numLabels[i];
			centers[i][1] /= numLabels[i];
			centers[i][2] /= numLabels[i];
			centers[i][3] /= numLabels[i];
			centers[i][4] /= numLabels[i];

			numLabels[i] = 0;
		}

		// �������Ͼ�������֮���ƽ���в�
		double residual = 0.0;
		for (int i = 0; i < centers.size(); i++)
		{
			Point point(oldcenters[i][3], oldcenters[i][4]);
			residual += computeDis(i, point, oldcenters[i][0], oldcenters[i][1], oldcenters[i][2]);
		}
		residual /= centers.size();
		cout << "iter: " << iterIndex <<" residual: " << residual << endl;

		// ���ƽ���в�С��һ������ֵ����ǰ��ֹ�㷨
		if (residual <= threshold)
		{
			cout << "�㷨����" << endl;
			break;
		}
	}

	// ����ɳ����طָ��ϲ�������

	mergeOutliers();
}

void SLIC::mergeOutliers()
{
	int area = 0;

	// ��С��ͨ������
	int minimumArea = step * step / 16;

	// ��8����������
	int neX[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int neY[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

	// ��¼ͼ���е�ÿһ�����ص��Ƿ��Ѿ����������
	vector<vector<bool>> whetherProcessed;
	for (int i = 0; i < labImg.rows; i++)
	{
		vector<bool> whetherprocessed;
		for (int j = 0; j < labImg.cols; j++)
		{
			whetherprocessed.push_back(false);
		}
		whetherProcessed.push_back(whetherprocessed);
	}

	// ����ͼ���е�ÿһ�����ص㣬����Ⱥ���label���Ϊ-1
	for (int i = 0; i < labImg.rows; i++)
	{
		for (int j = 0; j < labImg.cols; j++)
		{
			if (whetherProcessed[i][j] == false)
			{
				vector<Point> connectedComponent;  // ��¼����ͨ��ĵ�����
				connectedComponent.push_back(Point(j, i));

				for (int k = 0; k < connectedComponent.size(); k++)
				{
					for (int n = 0; n < 8; n++)
					{
						int x = neY[n] + connectedComponent[k].y;
						int y = neX[n] + connectedComponent[k].x;

						if (x >= 0 && x < labImg.rows && y >= 0 && y < labImg.cols && whetherProcessed[x][y] == false)
						{
							if (labels[connectedComponent[k].y][connectedComponent[k].x] == labels[x][y])
							{
								connectedComponent.push_back(Point(y, x));
								whetherProcessed[x][y] = true;
							}
						}
					}
				}
				area += connectedComponent.size();
				// �������ͨ������С��ָ������ֵ���򽫸������еĵ��label����Ϊ�����Ⱥ�������label
				if (connectedComponent.size() < minimumArea)
				{
					cout << "too small: " << connectedComponent.size() << endl;
					bool found = false;
					int adjLabel;
					for (int k = 0; k < connectedComponent.size(); k++)
					{
						for (int n = 0; n < 8; n++)
						{
							int x = neY[n] + connectedComponent[k].y;
							int y = neX[n] + connectedComponent[k].x;

							if (x >= 0 && x < labImg.rows && y >= 0 && y < labImg.cols)
							{
								if (labels[connectedComponent[k].y][connectedComponent[k].x] != labels[x][y])
								{
									adjLabel = labels[x][y];
									found = true;
									break;
								}
							}
						}
						if (found) break;
					}

					for (int k = 0; k < connectedComponent.size(); k++)
					{
						labels[connectedComponent[k].y][connectedComponent[k].x] = adjLabel;
					}
				}
			}
		}
	}
	cout << "area: " << area << endl;
}

double SLIC::computeDis(int centerIndex, const Point &point, double l, double a, double b)
{
	double dc = sqrt(pow(centers[centerIndex][0] - l, 2) + pow(centers[centerIndex][1] - a, 2) + pow(centers[centerIndex][2] - b, 2));
	double ds = sqrt(pow(centers[centerIndex][3] - point.x, 2) + pow(centers[centerIndex][4] - point.y, 2));
	return sqrt(pow(dc / nc, 2) + pow(ds / ns, 2));
}

void SLIC::colour_with_cluster_means(const Mat &img, Mat &showimg, int flag)
{
	Vecdd colors;
	vector<int> numLabel;
	showimg = img.clone();

	for (int i = 0; i < centers.size(); i++)
	{
		vector<double> color;
		color.push_back(0);
		color.push_back(0);
		color.push_back(0);
		colors.push_back(color);
		numLabel.push_back(0);
	}

	// ������ɫ��ֵ
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			int label = labels[i][j];
			if (label != -1)
			{
				colors[label][0] += showimg.at<Vec3b>(i, j)[0];
				colors[label][1] += showimg.at<Vec3b>(i, j)[1];
				colors[label][2] += showimg.at<Vec3b>(i, j)[2];

				numLabel[label]++;
			}
		}
	}

	// ��һ��
	for (int i = 0; i < centers.size(); i++)
	{
		colors[i][0] /= numLabel[i];
		colors[i][1] /= numLabel[i];
		colors[i][2] /= numLabel[i];
	}

	// ��ͼ�����������ɫ
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			showimg.at<Vec3b>(i, j)[0] = colors[labels[i][j]][0];
			showimg.at<Vec3b>(i, j)[1] = colors[labels[i][j]][1];
			showimg.at<Vec3b>(i, j)[2] = colors[labels[i][j]][2];	
		}
	}

	// ���ݻ���ģʽ���в������
	switch (flag)
	{
	case superPixelOnly:
		break;
	case superPixelWithCenters:
		showCenters(showimg);
		break;
	case superPixelWithEdge:
		showEdges(showimg);
		break;
	case superPixelWithEdgeAndCenters:
		showEdges(showimg);
		showCenters(showimg);
		break;
	default:
		break;
	}
}

void SLIC::showCenters(Mat &img)
{
	// ʹ�õ�������������ÿһ����������
	for (Vecdd::iterator iter = centers.begin(); iter != centers.end(); iter++)
	{
		Point point((*iter)[3], (*iter)[4]);
		//cout << point.x << ',' << point.y << endl;
		circle(img, point, 1, Scalar(0, 0, 0), -1, 16);
	}
}

void SLIC::showEdges(Mat &img)
{
	// ���û��Ʊ�Ե����ɫ
	Vec3b edgeColor(128, 128, 128);

	// ��8����������
	int neX[] = { -1, 0, 1, -1, 1, -1, 0, 1};
	int neY[] = {-1, -1, -1, 0, 0, 1, 1, 1};

	/*
	���������������Ҫԭ����
	��ֹlabels�����е�ֵ�任�ù��ھ��ҵ��°Ѻܶ�㶼�жϳ��˱߽��
	��������ǰ�����ú��ƺ���û�й۲쵽̫�������
	*/
	vector<vector<int>> whetherEdge;
	for (int i = 0; i < labImg.rows; i++)
	{
		vector<int> whetheredge;
		for (int j = 0; j < labImg.cols; j++)
		{
			whetheredge.push_back(false);
		}
		whetherEdge.push_back(whetheredge);
	}

	for (int i = 0; i < labImg.rows; i++)
	{
		for (int j = 0; j < labImg.cols; j++)
		{
			int pt = 0;
			for (int k = 0; k < 8; k++)
			{
				if (i + neX[k] >=0 && i + neX[k] < labImg.rows && j + neY[k] >= 0 && j + neY[k] < labImg.cols)
					if (whetherEdge[i][j] == false && labels[i][j] != labels[i + neX[k]][j + neY[k]])
						pt++;
			}
			if (pt >= 2)
			{
				img.at<Vec3b>(i, j) = edgeColor;
				whetherEdge[i][j] = true;
			}
		}
	}
}