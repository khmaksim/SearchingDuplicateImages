#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QtWidgets/QFileDialog>
#include <QtGui/QStandardItemModel>
#include <QtCore/QDateTime>
#include <QDebug>

#include "duplicate.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    imagesModel = new QStandardItemModel(this);
    imagesModel->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Date") << tr("Size"));

    ui->imageListView->setModel(imagesModel);
    ui->imagesTableView->setModel(imagesModel);
    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->selectDirButton, SIGNAL(clicked(bool)), SLOT(selectDirectory()));
    connect(ui->selectedDirectorylineEdit, SIGNAL(textChanged(QString)), SLOT(enableStartButton()));
    connect(ui->startButton, SIGNAL(clicked(bool)), SLOT(startSearch()));
    connect(ui->viewComboBox, SIGNAL(activated(int)), SLOT(setView(int)));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::selectDirectory()
{
    selectedDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                    QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!selectedDirectory.isEmpty()) {
        ui->selectedDirectorylineEdit->setText(QDir::toNativeSeparators(selectedDirectory));
    }
}

void MainWidget::enableStartButton()
{
    ui->startButton->setEnabled(!selectedDirectory.isEmpty());
}

void MainWidget::startSearch()
{
    Duplicate *duplicate = new Duplicate();
    QList<QFileInfo> list = duplicate->search(selectedDirectory);
    QStandardItem *item;
    int row = 0;

    QList<QFileInfo>::const_iterator constIt = list.constBegin();
    while (constIt != list.constEnd()) {
        item = new QStandardItem(QIcon((*constIt).absoluteFilePath()), QDir().toNativeSeparators((*constIt).absoluteFilePath()));
        item->setCheckable(true);
        imagesModel->setItem(row, 0, item);
        item = new QStandardItem((*constIt).created().toString("dd.MM.yyyy hh:mm"));
        item->setTextAlignment(Qt::AlignCenter);
        imagesModel->setItem(row, 1, item);
        item = new QStandardItem(QString::number((*constIt).size()));
        item->setTextAlignment(Qt::AlignCenter);
        imagesModel->setItem(row, 2, item);
        ++constIt;
        row++;
    }
}

void MainWidget::setView(int index)
{
    switch (index) {
        case 0:
            ui->stackedWidget->setCurrentIndex(index);
            break;
        case 1:
            ui->stackedWidget->setCurrentIndex(index);
    }
}
