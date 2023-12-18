#pragma once

#include <QHash>
#include <QObject>
#include <QWaitCondition>
#include <QMutex>
#include <QSemaphore>

class DataOfRead
{
public:
    QString word;
    int wordCount = 0;
    float totalProgress = 0;
};


class FileReader : public QObject
{
    Q_OBJECT

    QMutex mutex;
    QWaitCondition condition;
    QSemaphore semaphore{1};
    QHash<QString, int> words;
    bool isActive{false};

public:
    explicit FileReader(QObject* parent = nullptr);
    ~FileReader() override;

    void notifyDataReceived();

    void start(const QString& filePath);
    void pause();
    void resume();
    void stop();

signals:
    void dataChanged(const DataOfRead& data);
    void finished(const QString& error = {});

};
