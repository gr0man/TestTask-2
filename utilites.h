#ifndef UTILITES_H
#define UTILITES_H

#include <QString>

// файл с результатами
extern const char *c_logFileName;

// количество итераций для каждого каждого значений количества потоков
// (для набора статистики, значение времени усредняется)
const quint32 c_maxIterations = 100;

// минимальный размер сгенерированноых файлов
const quint32 c_minFileSize = 10000;
// максимальный размер сгенерированных файлов
const quint32 c_maxFileSize = 1000000;

// минимальное количество потоков
const quint32 c_minThreadsNumber = 1;
// максимальное количество потоков
const quint32 c_maxThreadsNumber = 10;

enum ProgParam
{
    PARAM_DIRECTORY,
    PARAM_GENERATE,
    PARAM_BEGIN_TH_NUMBER,
    PARAM_END_TH_NUMBER,
    PARAM_NONE
};

void GenerateFiles(const QString &directoryPath, quint32 filesNumber);

typedef quint64 (*pHashFunction)(const QByteArray&);

// функции для вычисления хэша массива данных
// указатель на хэш-функцию передается объекту Task при его создании в функции TaskManager::startTasks
quint64 LrcHashFunction(const QByteArray &data);
quint64 SimpleHashFunction(const QByteArray &data);

#endif // UTILITES_H
