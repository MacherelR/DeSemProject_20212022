#include <QRecursiveMutex>
#include "critical.h"


volatile unsigned char bInISR = 0;

static QRecursiveMutex mutex;

void enterCritical()
{
    mutex.lock();
}

void exitCritical()
{
    mutex.unlock();
}
