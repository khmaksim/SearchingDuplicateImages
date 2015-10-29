#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
    class MainWidget;
}

class QStandardItemModel;
class MainWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit MainWidget(QWidget *parent = 0);
        ~MainWidget();

    private:
        Ui::MainWidget *ui;
        QStandardItemModel *imagesModel;
        QString selectedDirectory;

    private slots:
        void selectDirectory();
        void enableStartButton();
        void startSearch();
        void setView(int);
        void changeProgressSearch();
        void clearImagesModel();
};

#endif // MAINWIDGET_H
