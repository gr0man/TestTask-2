#include "utilites.h"

#include <QDir>
#include <QFile>

#include <ctime>
#include <functional>

const char *c_logFileName = "results.txt";

// генерирует заданное количество файлов в заданной директории (случайный размер и содержимое)
void GenerateFiles(const QString &directoryPath, quint32 filesNumber)
{
    std::srand(std::time(nullptr));
    for(quint32 i = 0; i<filesNumber; ++i)
    {
        QString fileName = directoryPath + "\\" + QString("%1.bin").arg(i+1);
        double randValue = std::rand();
        randValue = (c_maxFileSize-c_minFileSize)*(randValue/(RAND_MAX+1u));
        quint32 len = c_minFileSize + randValue;

        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        for(quint32 j=0; j<len; ++j)
        {
            quint8 data = std::rand()/((RAND_MAX+1u)/255);
            file.write(reinterpret_cast<const char*>(&data), 1);
        }
        file.close();
    }
}

quint64 LrcHashFunction(const QByteArray &data)
{
    quint8 sum = 0;
    for(auto value : data)
        sum = (sum + value) & 0xFF;

    sum = (((sum ^ 0xFF) + 1) & 0xFF);

    return sum;
}

quint64 SimpleHashFunction(const QByteArray &data)
{
    quint64 a = 54059;
    quint64 b = 76963;
    quint64 initial = 37;

    quint64 hashValue = initial;

    for(auto value : data)
    {
        hashValue = (hashValue * a) ^ (value * b);
    }

    return hashValue;
}
