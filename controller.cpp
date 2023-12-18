#include <QThread>
#include "controller.h"

Controller::Controller(QObject* parent)
    : QObject(parent)
{
}

Controller::~Controller()
{
    stop();

    if (m_thread)
    {
        m_thread->quit();
        m_thread->wait();
    }
}

QUrl Controller::filePath() const
{
    return m_filePath.fileName();
}

void Controller::setFilePath(const QUrl& filePath)
{
    if (m_filePath != filePath)
    {
        m_filePath = filePath;
        emit filePathChanged();

        stop();
    }
}

void Controller::startPause()
{
    switch (m_state)
    {
    case State::Running:
        m_reader->pause();
        setState(State::Paused);
        return;

    case State::Paused:
        m_reader->resume();
        setState(State::Running);
        return;

    default:
        break;
    }

    stop();

    FileReader* reader = new FileReader;
    QThread* thread = new QThread(this);
    reader->moveToThread(thread);

    connect(thread, &QThread::started, reader, [this, reader]()
            {
                reader->start(m_filePath.toLocalFile());
            });

    connect(thread, &QThread::finished, this, [thread, reader]()
            {
                thread->deleteLater();
                reader->deleteLater();
            });

    connect(reader, &FileReader::dataChanged, this, [this, reader](const DataOfRead& data)
            {
                reader->notifyDataReceived();

                if (m_state == State::Running)
                {
                    setProgress(data.totalProgress);
                }

                if (m_state != State::Stopped)
                {
                    m_model.handle(data.word, data.wordCount);
                }
            });

    connect(reader, &FileReader::finished, this, [this, thread](const QString& error)
            {
                setState(State::Stopped);
                thread->quit();
            });

    setState(State::Running);

    m_reader = reader;
    m_thread = thread;

    m_thread->start();
}

void Controller::stop()
{
    setProgress(0.);
    setState(State::Stopped);

    if (m_reader)
    {
        m_reader->stop();
    }

    m_model.reset();
}

void Controller::setState(State value)
{
    if (m_state != value)
    {
        m_state = value;
        emit stateChanged();
    }
}

void Controller::setProgress(float value)
{
    if (!qFuzzyCompare(m_progress, value))
    {
        m_progress = value;
        emit progressChanged();
    }
}
