#ifndef TASK_H
#define TASK_H
#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QSharedPointer>

#include "utilites.h"

// инкапсулирует функционал QFuture и QFutureWatcher для вычислений.
class Task : public QObject
{
    Q_OBJECT
public:
    Task(QThreadPool &threadPool, pHashFunction hashFunction, QList<QString> fileNames, QObject *parent = nullptr);

signals:
    void completed(QSharedPointer<QList<quint64>> result);

private slots:
    void futureFinished();

private:
    QList<quint64> hashCalculation(pHashFunction hashFunction, QList<QString> fileNames);

    QFuture<QList<quint64>> m_future;
    QFutureWatcher<QList<quint64>> m_watcher;
};

#endif // TASK_H
