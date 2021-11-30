#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QMutex>

// сохраняет текстовые сообщения в файл
class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(const char *filePath, QObject *parent = nullptr);
    ~Logger();

    void open(const char *filePath);
    void close();

signals:

public slots:
    void add(const char *msg);

private:
    QFile m_logFile;
    QMutex m_mutex;
};

#endif // LOGGER_H
