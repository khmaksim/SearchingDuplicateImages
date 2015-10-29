#include "duplicate.h"

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtGui/QImage>
#include <QtCore/QBitArray>
#include <QtGui/QStandardItemModel>
#include <QtCore/QDateTime>
#include <QDebug>

Duplicate::Duplicate(QStandardItemModel *model, QObject *parent) : QObject(parent)
{
    this->model = model;
}

Duplicate::~Duplicate()
{

}

void Duplicate::search(const QString &dir)
{
    QMap<QByteArray, QFileInfo> allFiles;
    QMultiMap<QByteArray, QFileInfo> duplicateFiles;
    QByteArray hashFile;

    time.start();
    QList<QFileInfo> filesList = getFilesList(dir);
    qDebug("getFilesList: %d ms", time.elapsed());

    QList<QFileInfo>::const_iterator constIt;
    for (constIt = filesList.constBegin(); constIt != filesList.constEnd(); ++constIt) {
        time.start();
        hashFile = hashImage((*constIt).absoluteFilePath());
        qDebug("hashImage: %d ms", time.elapsed());

        if (duplicateFiles.contains(hashFile)) {
            duplicateFiles.insertMulti(hashFile, (*constIt));
            addItemToModel((*constIt));
        }
        else if (allFiles.contains(hashFile)) {
            duplicateFiles.insertMulti(hashFile, (*constIt));
            duplicateFiles.insertMulti(hashFile, allFiles.value(hashFile));
            addItemToModel((*constIt));
            addItemToModel(allFiles.value(hashFile));
            allFiles.remove(hashFile);
        }
        else
            allFiles.insert(hashFile, (*constIt));
        qApp->processEvents();
    }

    return;
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
        emit fileFound();
        qApp->processEvents();
    }
    return files;
}

QByteArray Duplicate::hashImage(const QString &nameFile) const
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

void Duplicate::addItemToModel(QFileInfo fileInfo)
{
    QStandardItem *item = new QStandardItem(QIcon(fileInfo.absoluteFilePath()), QDir().toNativeSeparators(fileInfo.absoluteFilePath()));
    item->setCheckable(true);
    model->appendRow(QList<QStandardItem*>()
                               << item
                               << new QStandardItem(fileInfo.created().toString("dd.MM.yyyy hh:mm"))
                               << new QStandardItem(QString::number(fileInfo.size())));
}
