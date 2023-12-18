#include "histogramModel.h"

const int rowsCount = 15;

HistogramModel::HistogramModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

QHash<int,QByteArray> HistogramModel::roleNames() const
{
    QHash<int,QByteArray> temp;
    temp.insert(Roles::Word, "word");
    temp.insert(Roles::Count, "count");
    temp.insert(Roles::Percent, "percent");
    return temp;
}

int HistogramModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return words.size();
}

QVariant HistogramModel::data(const QModelIndex& index, int role) const
{
    const TextWord& word = words.at(index.row());

    switch (role)
    {
    case Roles::Word:
        return word.word;

    case Roles::Count:
        return word.count;
    case Roles::Percent:
        return 0.1 + len * word.count;

    default:
        return QVariant();
    }
}

void HistogramModel::reset()
{
    beginResetModel();
    rows.clear();
    words.clear();
    endResetModel();
}

void HistogramModel::handle(const QString& word, int count)
{
    int row = tryUpdate(word, count);

    if (row < 0)
    {
        return;
    }

    for (const int prevRow = row; row > 0; --row)
    {
        TextWord& cur = words[row];
        TextWord& prev = words[row-1];

        if (cur.count > prev.count)
        {
            words.swapItemsAt(row, row-1);
            std::swap(rows[cur.word], rows[prev.word]);
        }
        else
        {
            if (row != prevRow)
            {
                beginMoveRows({}, prevRow, prevRow, {}, row);
                endMoveRows();
            }
            break;
        }
    }
    len = 0.9 / words.first().count;
    QModelIndex first = QAbstractListModel::index(0);
    QModelIndex last = QAbstractListModel::index(words.size()-1);
    emit dataChanged(first, last, { Roles::Percent });
}

int HistogramModel::tryUpdate(const QString& word, int count)
{
    if (count < 1)
    {
        return -1;
    }

    int row = rows.value(word, -1);

    if (row < 0)
    {
        if (words.isEmpty() ||
            words.size() < rowsCount ||
            count > words.last().count)
        {
            if (words.size() == rowsCount)
            {
                beginRemoveRows({}, words.size() - 1, words.size() - 1);
                rows.remove(words.last().word);
                words.removeLast();
                endRemoveRows();
            }

            row = words.size();

            beginInsertRows({}, row, row);
            words.append({ word, count });
            rows[word] = row;
            endInsertRows();

            return row;
        }
    }
    else
    {
        TextWord& wordItem = words[row];

        if (wordItem.count < count)
        {
            wordItem.count = count;

            QModelIndex index = QAbstractListModel::index(row);
            emit dataChanged(index, index, { Roles::Count });

            return row;
        }
    }

    return -1;
}
