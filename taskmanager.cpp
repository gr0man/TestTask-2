#include "taskmanager.h"
#include "task.h"
#include "utilites.h"

#include <QDir>
#include <QMutexLocker>
#include <QByteArray>

TaskManager::TaskManager(QObject *parent)
    : QObject(parent)
    , m_hashFunction(nullptr)
{

}

void TaskManager::taskCompleted(QSharedPointer<QList<quint64>> results)
{
    QMutexLocker locker(&m_mutex);
    m_results.append(*results);
    m_tasksNumber--;
    if(m_tasksNumber == 0)
    {
        m_tasks.clear();
        emit finished(&m_results);
    }
}

void TaskManager::startTasks(QString directory, quint32 thNumber)
{
    Q_ASSERT(m_hashFunction);

    m_results.clear();
    m_tasks.clear();
    m_tasksNumber = 0;

    QDir dir(directory);
    auto fileInfos = dir.entryInfoList(QDir::NoFilter, QDir::Size);
    QList<QString> filePaths;
    for(auto fileInfo : fileInfos)
    {
        if(fileInfo.isFile())
            filePaths.push_back(fileInfo.absoluteFilePath());
    }

    quint32 threadsNumber = std::min(static_cast<int>(thNumber), filePaths.size());

    for(quint32 i=0; i<threadsNumber; ++i)
    {
        QList<QString> files;
        // файлы отсортированы по размеру для равномерного распределения объемов данных для задач
        for(quint32 j=i; j<filePaths.size(); j+=threadsNumber)
        {
            files.push_back(filePaths.at(j));
        }

        QMutexLocker locker(&m_mutex);
        m_tasksNumber++;
        m_tasks.push_back(QSharedPointer<Task>(new Task(m_threadPool, m_hashFunction, files, this)));
    }
}
