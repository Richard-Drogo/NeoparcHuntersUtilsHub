#include <QCoreApplication>
#include <QTimer>
#include <QThreadPool>
#include <QThread>
#include <QtConcurrent>

#include "maintask.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qRegisterMetaType<qintptr>("qintptr");

    QThreadPool::globalInstance()->setMaxThreadCount(1);

    MainTask * mainTask = new MainTask(argc, argv, &a);
    QObject::connect(mainTask, &MainTask::quit, &a, QCoreApplication::quit);
    QTimer::singleShot(0, mainTask, SLOT(startingPoint()));

    return a.exec();
}
