#include "duplicate.h"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtGui/QImage>
#include <QtCore/QBitArray>
#include <QDebug>

Duplicate::Duplicate()
{

}

QList<QFileInfo> Duplicate::search(const QString &dir)
{
    QMap<QByteArray, QFileInfo> allFiles;
    QMultiMap<QByteArray, QFileInfo> duplicateFiles;
    QByteArray hashFile;

    QList<QFileInfo> filesList = getFilesList(dir);

    QList<QFileInfo>::const_iterator constIt;
    for (constIt = filesList.constBegin(); constIt != filesList.constEnd(); ++constIt) {
        hashFile = getHashImage((*constIt).absoluteFilePath());
        if (duplicateFiles.contains(hashFile))
            duplicateFiles.insertMulti(hashFile, (*constIt));
        else if (allFiles.contains(hashFile)) {
            duplicateFiles.insertMulti(hashFile, (*constIt));
            duplicateFiles.insertMulti(hashFile, allFiles.value(hashFile));
            allFiles.remove(hashFile);
        }
        else
            allFiles.insert(hashFile, (*constIt));
    }
    return duplicateFiles.values();
}

QList<QFileInfo> Duplicate::getFilesList(const QString &dir)
{
    QList<QFileInfo> files;
    QFileInfoList filesAndDirs = QDir(dir).entryInfoList(QStringList() << "*.jpg" << "*.jpeg",
                                                   QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst);
    QList<QFileInfo>::const_iterator constIt;
    for (constIt = filesAndDirs.constBegin(); constIt != filesAndDirs.constEnd(); ++constIt) {
        if ((*constIt).isDir()) {
            files.append(getFilesList((*constIt).absoluteFilePath()));
            continue;
        }
        files.append((*constIt));
    }
    return files;
}

QByteArray Duplicate::getHashImage(const QString &nameFile) const
{
    QImage image(nameFile);

    image = image.scaled(QSize(8, 8));          // small image
    image = image.convertToFormat(QImage::Format_Mono);         // convert to monochrome image

    QBitArray bits(64);
    QByteArray hash;
    QVector<int> valuesColorPixel;
    int valueColorPixel = 0;
    int averageValueColor = 0;

    for (int y = 0; y < image.height(); y++)
        for (int x = 0; x < image.width(); x++) {
            valueColorPixel = qGray(image.pixel(x, y));
            averageValueColor += valueColorPixel;
            valuesColorPixel << valueColorPixel;
        }

    averageValueColor = averageValueColor / (image.width() * image.height());

    QVector<int>::const_iterator constIt = valuesColorPixel.constBegin();
    int index = 0;
    while (constIt != valuesColorPixel.constEnd()) {
        if ((*constIt) > averageValueColor)
            bits[index] = 1;
        else
            bits[index] = 0;
        ++constIt;
        index++;
    }
    QDataStream out(&hash, QIODevice::WriteOnly);
    out << bits;

    return hash.toHex();
}
