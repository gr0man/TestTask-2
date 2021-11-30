#include "logger.h"

Logger::Logger(const char *filePath, QObject *parent)
    : QObject(parent)
    , m_logFile(filePath)
{
    m_logFile.open(QIODevice::WriteOnly | QIODevice::Text);
}

Logger::~Logger()
{
    close();
}

void Logger::close()
{
    QMutexLocker locker(&m_mutex);
    m_logFile.close();
}

void Logger::add(const char *info)
{
    QMutexLocker locker(&m_mutex);

    if (m_logFile.isOpen())
    {
        m_logFile.write(info);
        m_logFile.write("\n");
    }
}
