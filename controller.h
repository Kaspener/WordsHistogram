#pragma once

#include <qqml.h>
#include <QObject>
#include <QPointer>

#include "histogramModel.h"
#include "fileReader.h"

class Reader;
class QThread;

class Controller : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QAbstractListModel* model READ model CONSTANT)
    Q_PROPERTY(QUrl filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)

    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(float progress READ progress NOTIFY progressChanged)

public:
    enum State
    {
        Running,
        Paused,
        Stopped
    };
    Q_ENUM(State)

    explicit Controller(QObject* parent = nullptr);
    ~Controller() override;

    QUrl filePath() const;
    void setFilePath(const QUrl& filePath);

    QAbstractListModel* model() { return &m_model; }

    State state() const { return m_state; }
    float progress() const { return m_progress; }

    Q_INVOKABLE void startPause();
    Q_INVOKABLE void stop();

private:
    void setState(State value);
    void setProgress(float value);

private:
    QUrl m_filePath;
    HistogramModel m_model;
    QPointer<FileReader> m_reader;
    QPointer<QThread> m_thread;

    QString m_error;
    float m_progress{0};
    State m_state {State::Stopped};

signals:
    void filePathChanged();
    void stateChanged();
    void progressChanged();

};
