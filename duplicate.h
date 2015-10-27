#ifndef DUPLICATE_H
#define DUPLICATE_H

#include <QtCore/QList>

class QString;
class QByteArray;
class QStringList;
class QFileInfo;
class Duplicate
{
    public:
        Duplicate();

    public:
        QList<QFileInfo> search(const QString &dir);

    private:
        QList<QFileInfo> getFilesList(const QString &dir);
        QByteArray getHashImage(const QString &nameFile) const;
};

#endif // DUPLICATE_H
