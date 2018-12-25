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

	// 图像地址
	QString imgDir;

	// 输入图像
	cv::Mat img;

	// 算法
	SLIC *slic;

	// 展示图像
	cv::Mat showImg;

	// 展示方式 
	QButtonGroup *showMode;

	// 三个按钮的槽函数
	void onImgOpenPushButtonClicked();
	void onStartPushButtonClicked();
	void onImgSavePushButtonClicked();

	// 显示模式的槽函数
	void onRadionButtonClicked();

	// 算法运行完毕的槽函数
	void onSlicFinished();
};

#endif // MAINWINDOW_H
