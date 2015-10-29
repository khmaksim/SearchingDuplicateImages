#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QtWidgets/QFileDialog>
#include <QtGui/QStandardItemModel>
#include <QDebug>

#include "duplicate.h"
#include "tableitemdelegate.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    imagesModel = new QStandardItemModel(this);
    imagesModel->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Date") << tr("Size"));

    ui->imageListView->setModel(imagesModel);
    ui->imagesTableView->setModel(imagesModel);
    ui->imagesTableView->setItemDelegate(new TableItemDelegate(this));

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
    Duplicate *duplicate = new Duplicate(imagesModel);
    connect(duplicate, SIGNAL(fileFound()), this, SLOT(changeProgressSearch()));
    duplicate->search(selectedDirectory);
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

void MainWidget::changeProgressSearch()
{
    ui->lcdNumber->display(ui->lcdNumber->value() + 1);
}

void MainWidget::clearImagesModel()
{
    while (imagesModel->rowCount() > 0) {
        imagesModel->removeRow(0);
    }
}
