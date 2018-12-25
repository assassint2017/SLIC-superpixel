#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	ui->iterSlider->setMaximum(30);
	ui->iterSlider->setMinimum(1);

	ui->iterSpinBox->setMaximum(30);
	ui->iterSpinBox->setMinimum(1);

	ui->numSuperPixelSlider->setMaximum(5e3);
	ui->numSuperPixelSlider->setMinimum(1);

	ui->numSuperPixelSpinBox->setMaximum(5e3);
	ui->numSuperPixelSpinBox->setMinimum(1);

	ui->ncSlider->setMaximum(50);
	ui->ncSlider->setMinimum(10);

	ui->ncSpinBox->setMaximum(50);
	ui->ncSpinBox->setMinimum(10);

	ui->thresholdSlider->setMaximum(100);
	ui->thresholdSlider->setMinimum(1);

	ui->thersholdSpinBox->setMaximum(100);
	ui->thersholdSpinBox->setMinimum(1);

	// 连接spinbox和滑动条的信号和槽函数
	QObject::connect(ui->numSuperPixelSlider, &QSlider::valueChanged, ui->numSuperPixelSpinBox, &QSpinBox::setValue);
	QObject::connect(ui->iterSlider, &QSlider::valueChanged, ui->iterSpinBox, &QSpinBox::setValue);
	QObject::connect(ui->thresholdSlider, &QSlider::valueChanged, ui->thersholdSpinBox, &QSpinBox::setValue);
	QObject::connect(ui->ncSlider, &QSlider::valueChanged, ui->ncSpinBox, &QSpinBox::setValue);

	QObject::connect(ui->numSuperPixelSpinBox, SIGNAL(valueChanged(int)), ui->numSuperPixelSlider, SLOT(setValue(int)));
	QObject::connect(ui->iterSpinBox, SIGNAL(valueChanged(int)), ui->iterSlider, SLOT(setValue(int)));
	QObject::connect(ui->thersholdSpinBox, SIGNAL(valueChanged(int)), ui->thresholdSlider, SLOT(setValue(int)));
	QObject::connect(ui->ncSpinBox, SIGNAL(valueChanged(int)), ui->ncSlider, SLOT(setValue(int)));

	// 连接三个按钮的信号和槽函数
	QObject::connect(ui->startpushButton, &QPushButton::clicked, this, &MainWindow::onStartPushButtonClicked);
	QObject::connect(ui->imgOpenpushButton, &QPushButton::clicked, this, &MainWindow::onImgOpenPushButtonClicked);
	QObject::connect(ui->imgSavepushButton, &QPushButton::clicked, this, &MainWindow::onImgSavePushButtonClicked);

	// 设置显示方式
	showMode = new QButtonGroup;

	showMode->addButton(ui->radioButton, superPixelOnly);
	showMode->addButton(ui->radioButton_2, superPixelWithCenters);
	showMode->addButton(ui->radioButton_3, superPixelWithEdge);
	showMode->addButton(ui->radioButton_4, superPixelWithEdgeAndCenters);

	QObject::connect(ui->radioButton, &QRadioButton::toggled, this, &MainWindow::onRadionButtonClicked);
	QObject::connect(ui->radioButton_2, &QRadioButton::toggled, this, &MainWindow::onRadionButtonClicked);
	QObject::connect(ui->radioButton_3, &QRadioButton::toggled, this, &MainWindow::onRadionButtonClicked);
	QObject::connect(ui->radioButton_4, &QRadioButton::toggled, this, &MainWindow::onRadionButtonClicked);

	ui->radioButton->setEnabled(false);
	ui->radioButton_2->setEnabled(false);
	ui->radioButton_3->setEnabled(false);
	ui->radioButton_4->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onImgOpenPushButtonClicked()
{
	imgDir = QFileDialog::getOpenFileName(this, "open img", ".");

	ui->imgDir->setText(imgDir);

	img = cv::imread(imgDir.toStdString());

	if (img.rows > 400 || img.cols > 400)
	{
		cv::Mat tempMat;
		cv::resize(img, tempMat, cv::Size(0, 0), 0.5, 0.5);
		QImage tempImg(tempMat.data, tempMat.cols, tempMat.rows, tempMat.step, QImage::Format_RGB888);
		ui->rawImg->setPixmap(QPixmap::fromImage(tempImg.rgbSwapped()));
	}
	else
	{
		QImage tempImg(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
		ui->rawImg->setPixmap(QPixmap::fromImage(tempImg.rgbSwapped()));
	}
	
	// 当打开下一幅图像的时候
	// 应该禁用展示模式选择按钮
	ui->radioButton->setEnabled(false);
	ui->radioButton_2->setEnabled(false);
	ui->radioButton_3->setEnabled(false);
	ui->radioButton_4->setEnabled(false);

	// 并清除展示图片
	ui->resImg->clear();
}

void MainWindow::onImgSavePushButtonClicked()
{
	QFileInfo info(imgDir);

	QString fileName = info.fileName().replace(info.suffix(), "");
	switch (showMode->checkedId())
	{
	case superPixelOnly:
		fileName += "_superPixelOnly.";
		break;
	case superPixelWithCenters:
		fileName += "_superPixelWithCenters.";
		break;
	case superPixelWithEdge:
		fileName += "_superPixelWithEdge.";
		break;
	case superPixelWithEdgeAndCenters:
		fileName += "_superPixelWithEdgeAndCenters.";
		break;
	default:
		break;
	}
	
	fileName += info.suffix();

	QString fullFileName = QFileDialog::getSaveFileName(this, "save img", fileName);

	// 如果选择了取消保存
	if (fullFileName.isEmpty())
		return;
	else
		cv::imwrite(fullFileName.toStdString(), showImg);
}

void MainWindow::onStartPushButtonClicked()
{
	slic = new SLIC(img, 
		ui->numSuperPixelSlider->value(), 
		ui->ncSlider->value(), 
		ui->iterSlider->value(), 
		ui->thresholdSlider->value() * 1e-3);

	SlicThread *thread = new SlicThread(slic, this);

	QObject::connect(thread, &QThread::finished, this, &MainWindow::onSlicFinished);

	thread->start();
}

void MainWindow::onRadionButtonClicked()
{
	switch (showMode->checkedId())
	{
	case superPixelOnly:
		slic->colour_with_cluster_means(img, showImg, superPixelOnly);
		break;
	case superPixelWithCenters:
		slic->colour_with_cluster_means(img, showImg, superPixelWithCenters);
		break;
	case superPixelWithEdge:
		slic->colour_with_cluster_means(img, showImg, superPixelWithEdge);
		break;
	case superPixelWithEdgeAndCenters:
		slic->colour_with_cluster_means(img, showImg, superPixelWithEdgeAndCenters);
		break;
	default:
		break;
	}

	if (img.rows > 400 || img.cols > 400)
	{
		cv::Mat tempMat;
		cv::resize(showImg, tempMat, cv::Size(0, 0), 0.5, 0.5);
		QImage tempImg(tempMat.data, tempMat.cols, tempMat.rows, tempMat.step, QImage::Format_RGB888);
		ui->resImg->setPixmap(QPixmap::fromImage(tempImg.rgbSwapped()));
	}
	else
	{
		QImage tempImg(showImg.data, showImg.cols, showImg.rows, showImg.step, QImage::Format_RGB888);
		ui->resImg->setPixmap(QPixmap::fromImage(tempImg.rgbSwapped()));
	}
}

void MainWindow::onSlicFinished()
{
	// 算法运行完毕，使能展示模型选择按钮
	ui->radioButton->setEnabled(true);
	ui->radioButton_2->setEnabled(true);
	ui->radioButton_3->setEnabled(true);
	ui->radioButton_4->setEnabled(true);

	QMessageBox::information(this, "info", "finished!");

	// 默认为第一种展示模式
	ui->radioButton->setChecked(true);
	onRadionButtonClicked();
}
