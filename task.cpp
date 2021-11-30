#include "task.h"
//#include "threadmanager.h"

#include <iostream>
#include <QtConcurrent/QtConcurrent>
#include <windows.h>

Task::Task(QThreadPool &threadPool, pHashFunction hashFunction, QList<QString> fileNames, QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(completed(QSharedPointer<QList<quint64>>)),
            parent, SLOT(taskCompleted(QSharedPointer<QList<quint64>>)));
    connect(&m_watcher, SIGNAL(finished()), this, SLOT(futureFinished()));

    m_future = QtConcurrent::run(&threadPool, this, &Task::hashCalculation, hashFunction, fileNames);
    m_watcher.setFuture(m_future);
}

QList<quint64> Task::hashCalculation(pHashFunction hashFunction, QList<QString> fileNames)
{
    QList<quint64> results;
    for(auto fileName : fileNames)
    {
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly))
            continue;

        QByteArray data = file.readAll();
        results.push_back(hashFunction(data));
    }

    return results;
}


void Task::futureFinished()
{
    //QList<quint64> *result = new QList<quint64>(m_future.result());

    emit completed(QSharedPointer<QList<quint64>>(new QList<quint64>(m_future.result())));
}
