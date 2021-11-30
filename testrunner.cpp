#include "testrunner.h"

#include <iostream>

TestRunner::TestRunner(const QString &directoryPath, QObject *parent)
    : m_directoryPath(directoryPath)
    , QObject(parent)
{
    m_hashFunctions.push_back(QPair<pHashFunction, QString>(SimpleHashFunction, QString("Simple hash function")));
    m_hashFunctions.push_back(QPair<pHashFunction, QString>(LrcHashFunction, QString("LRC hash function")));

    connect(this, SIGNAL(startTasks(QString,quint32)),
            &m_taskManager, SLOT(startTasks(QString,quint32)),
            Qt::QueuedConnection);
    connect(&m_taskManager, SIGNAL(finished(QList<quint64>*)),
            this, SLOT(tasksFinished(QList<quint64>*)), Qt::QueuedConnection);
}

void TestRunner::start(quint32 minThreadsNumber, quint32 maxThreadsNumber)
{
    Q_ASSERT(minThreadsNumber <= maxThreadsNumber);

    m_iteration = c_maxIterations;

    m_minThreadsNumber = minThreadsNumber;
    m_maxThreadsNumber = maxThreadsNumber;
    m_crThreadsNumber = minThreadsNumber;

    m_hashIndex = 0;

    m_taskManager.setMaxThreads(maxThreadsNumber);
    m_taskManager.setHashFunction(m_hashFunctions.at(m_hashIndex).first);

    m_startPoint = std::chrono::high_resolution_clock::now();
    emit startTasks(m_directoryPath, m_crThreadsNumber);
}

void TestRunner::tasksFinished(QList<quint64> *results)
{
    auto endPoint = std::chrono::high_resolution_clock::now();

    std::chrono::nanoseconds diff = endPoint - m_startPoint;
    double res = diff.count();

    auto iter = m_results.find(m_crThreadsNumber);
    if(iter != m_results.end())
        res += iter.value();
    m_results[m_crThreadsNumber] = res;

    if(m_crThreadsNumber == m_maxThreadsNumber)
    {
        std::cout << "iteration: " << (c_maxIterations - m_iteration + 1) << std::endl;

        m_iteration--;
        if(m_iteration)
        {
            m_crThreadsNumber = m_minThreadsNumber;
            m_startPoint = std::chrono::high_resolution_clock::now();
            emit startTasks(m_directoryPath, m_crThreadsNumber);
        }
        else
        {
            calculateStatistic();

            m_hashIndex++;
            if(m_hashIndex >= m_hashFunctions.size())
            {
                emit completed();
            }
            else
            {
                m_results.clear();
                m_iteration = c_maxIterations;
                m_crThreadsNumber = m_minThreadsNumber;
                m_taskManager.setHashFunction(m_hashFunctions.at(m_hashIndex).first);
                m_startPoint = std::chrono::high_resolution_clock::now();
                emit startTasks(m_directoryPath, m_crThreadsNumber);
            }
        }
    }
    else
    {
        m_crThreadsNumber++;
        m_startPoint = std::chrono::high_resolution_clock::now();
        emit startTasks(m_directoryPath, m_crThreadsNumber);
    }
}

void TestRunner::calculateStatistic()
{
    char info[300];

    std::cout << "saving log for "
              << m_hashFunctions.at(m_hashIndex).second.toStdString().c_str()
              << "..."
              << std::endl;

    emit log(m_hashFunctions.at(m_hashIndex).second.toStdString().c_str());

    quint64 minTime(-1);
    quint32 threads(0);
    QMapIterator<quint32, double> iter(m_results);
    while(iter.hasNext())
    {
        iter.next();
        quint64 averTime = iter.value()/(1000*c_maxIterations);
        sprintf_s(info, "threads: %u, time: %u mcs", iter.key(), averTime);
        emit log(info);

        if(minTime > averTime)
        {
            minTime = averTime;
            threads = iter.key();
        }
    }

    sprintf_s(info, "minimal time %u mcs for %u threads\n", minTime, threads);
    emit log(info);
}
