#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThreadPool>
#include <QMutex>
#include <QList>
#include <QSharedPointer>

#include "task.h"
#include "utilites.h"

// запускает задачи в заданном количестве потоков
class TaskManager : public QObject
{
    Q_OBJECT
public:
    explicit TaskManager(QObject *parent = nullptr);

    void setMaxThreads(quint32 maxThreads){ m_threadPool.setMaxThreadCount(maxThreads); }
    void setHashFunction(pHashFunction hashFunction){ m_hashFunction = hashFunction; }

signals:
    void finished(QList<quint64> *results);
    void log(const char *info);

public slots:
    void startTasks(QString directory, quint32 thNumber);
    void taskCompleted(QSharedPointer<QList<quint64>> results);

private:
    QThreadPool m_threadPool;
    QList<quint64> m_results;
    quint32 m_tasksNumber;
    QMutex m_mutex;
    pHashFunction m_hashFunction;
    QList<QSharedPointer<Task>> m_tasks;
};

#endif // THREADMANAGER_H
