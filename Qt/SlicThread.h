#ifndef SLICTHREAD_H
#define SLICTHREAD_H

#include <QThread>

#include "slic.h"

class SlicThread : public QThread
{
	Q_OBJECT
public:
	explicit SlicThread(SLIC *slic, QObject *parent = nullptr);
	~SlicThread();
private:
	SLIC *slic;
protected:
	virtual void run();
};

#endif // !SLICTHREAD_H
