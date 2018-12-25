#include "SlicThread.h"

SlicThread::SlicThread(SLIC *slic, QObject *parent) : QThread(parent)
{
	this->slic = slic;
}

SlicThread::~SlicThread()
{

}

void SlicThread::run()
{
	slic->generateSuperPixel();
}
