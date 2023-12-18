#pragma once

#include <QHash>
#include <QAbstractListModel>

class HistogramModel : public QAbstractListModel
{
    Q_OBJECT

    enum Roles
    {
        Word = Qt::UserRole+1,
        HtmlWord,
        Count,
        Percent
    };

    class TextWord
    {
    public:
        QString word;
        int count = 0;
    };

public:
    explicit HistogramModel(QObject* parent = nullptr);

    QHash<int,QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void reset();
    void handle(const QString& word, int count);

private:
    int tryUpdate(const QString& word, int count);

private:
    float len{1};
    QList<TextWord> words;
    QHash<QString, int> rows;

};
