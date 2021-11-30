#include <QCoreApplication>
#include <QString>

#include "testrunner.h"
#include "logger.h"
#include "utilites.h"

#include <iostream>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc < 2)
    {
        std::cout << "too few parameters" << std::endl;
        return -1;
    }

    Logger logger(c_logFileName);

    QString directoryPath;
    ProgParam progParam(PARAM_NONE);
    quint32 filesNumber(0);
    quint32 beginThNumber(c_minThreadsNumber);
    quint32 endThNumber(c_maxThreadsNumber);

    for(int i=1; i<argc; ++i)
    {
        QString paramString = a.arguments().at(i);
        switch(progParam)
        {
        case PARAM_NONE:
            if(paramString == "-d")
                progParam = PARAM_DIRECTORY;
            else if(paramString == "-g")
                progParam = PARAM_GENERATE;
            else if(paramString == "-b")
                progParam = PARAM_BEGIN_TH_NUMBER;
            else if(paramString == "-e")
                progParam = PARAM_END_TH_NUMBER;
            else
            {
                std::cout << "uncknown parameter: " << paramString.toStdString().c_str() << std::endl;
                return -1;
            }
            break;
        case PARAM_DIRECTORY:
            directoryPath = paramString;
            progParam = PARAM_NONE;
            break;
        case PARAM_GENERATE:
            filesNumber = paramString.toUInt();
            progParam = PARAM_NONE;
            break;
        case PARAM_BEGIN_TH_NUMBER:
            beginThNumber = paramString.toUInt();
            progParam = PARAM_NONE;
            break;
        case PARAM_END_TH_NUMBER:
            endThNumber = paramString.toUInt();
            progParam = PARAM_NONE;
            break;
        }
    }

    if(directoryPath.isEmpty())
    {
        std::cout << "directory path is not defined" << std::endl;
        return -1;
    }

    if(filesNumber)
    {
        GenerateFiles(directoryPath, filesNumber);
    }

    TestRunner runner(directoryPath);

    QObject::connect(&runner, SIGNAL(completed()), &a, SLOT(quit()));
    QObject::connect(&runner, SIGNAL(log(const char*)), &logger, SLOT(add(const char*)));

    runner.start(beginThNumber, endThNumber);

    int ret = a.exec();


    logger.close();

    return ret;
}
