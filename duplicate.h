#ifndef DUPLICATE_H
#define DUPLICATE_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QTime>

class QString;
class QByteArray;
class QFileInfo;
class QStandardItemModel;
class Duplicate : public QObject
{
        Q_OBJECT

    public:
        explicit Duplicate(QStandardItemModel *model, QObject *parent = 0);
        ~Duplicate();

    public:
        void search(const QString &dir);

    private:
        QList<QFileInfo> getFilesList(const QString &dir);
        QByteArray hashImage(const QString &nameFile) const;
        void addItemToModel(QFileInfo fileInfo);

        QTime time;
        QStandardItemModel *model;

    signals:
        void fileFound();
};

#endif // DUPLICATE_H
