#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <QObject>
#include <QMap>

#include <chrono>

#include "utilites.h"
#include "taskmanager.h"

// управляет запуском пакетов задач и замеряет время
class TestRunner : public QObject
{
    Q_OBJECT
public:
    explicit TestRunner(const QString &directoryPath, QObject *parent = nullptr);

    void start(quint32 minThreadsNumber, quint32 maxThreadsNumber);

signals:
    void startTasks(QString directoryPath, quint32 threadsNumber);
    void completed();
    void log(const char *info);

public slots:
    void tasksFinished(QList<quint64> *results);

private:
    void calculateStatistic();

    TaskManager m_taskManager;
    quint32 m_maxThreadsNumber;
    quint32 m_minThreadsNumber;
    quint32 m_crThreadsNumber;
    QString m_directoryPath;

    quint32 m_iteration;
    QMap<quint32, double> m_results;

    QList<QPair<pHashFunction, QString>> m_hashFunctions;
    quint32 m_hashIndex;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint;
};

#endif // TESTRUNNER_H
