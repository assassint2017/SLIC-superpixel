#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QPixMap>
#include <QImage>
#include <QbuttonGroup>
#include <QRadioButton>
#include <QMessageBox>
#include <QFileInfo>

#include <opencv2/opencv.hpp>

#include "slic.h"
#include "SlicThread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

	// ͼ���ַ
	QString imgDir;

	// ����ͼ��
	cv::Mat img;

	// �㷨
	SLIC *slic;

	// չʾͼ��
	cv::Mat showImg;

	// չʾ��ʽ 
	QButtonGroup *showMode;

	// ������ť�Ĳۺ���
	void onImgOpenPushButtonClicked();
	void onStartPushButtonClicked();
	void onImgSavePushButtonClicked();

	// ��ʾģʽ�Ĳۺ���
	void onRadionButtonClicked();

	// �㷨������ϵĲۺ���
	void onSlicFinished();
};

#endif // MAINWINDOW_H
