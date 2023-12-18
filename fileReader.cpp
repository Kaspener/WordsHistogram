#include "fileReader.h"

#include <QFile>
#include <QTextStream>

bool ReadWord(QTextStream& stream, QString& result)
{
    do
    {
        stream >> result;
    }
    while(result.isEmpty() && !stream.atEnd());

    result = result.toLower();

    return !result.isEmpty();
}


FileReader::FileReader(QObject* parent)
    : QObject(parent)
{
}

FileReader::~FileReader()
{
    stop();
}

void FileReader::notifyDataReceived()
{
    semaphore.release();
}

void FileReader::start(const QString& filePath)
{
    if (isActive)
    {
        return;
    }

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit finished();
        return;
    }

    mutex.lock();
    isActive = true;

    QElapsedTimer timer;
    timer.start();

    QTextStream stream(&file);

    quint64 totalWords = 0;
    quint64 totalBytes = file.size();
    DataOfRead data;
    while(ReadWord(stream, data.word) && isActive)
    {
        data.wordCount = ++words[data.word];
        data.totalProgress = float(totalBytes - file.bytesAvailable()) / totalBytes;

        semaphore.acquire();
        emit dataChanged(data);

        if (mutex.tryLock())
        {
            condition.wait(&mutex);

            timer.restart();
            totalWords = 0;
        }
    }

    isActive = false;
    mutex.unlock();

    emit finished();
}

void FileReader::pause()
{
    if (!mutex.tryLock())
    {
        mutex.unlock();
    }
}

void FileReader::resume()
{
    condition.notify_one();
}

void FileReader::stop()
{
    isActive = false;
    semaphore.release();

    resume();
}
