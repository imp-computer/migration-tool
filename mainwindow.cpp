#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    //ui(new Ui::MainWindow)

{

    setupUi(this);

    connect(pushButton, SIGNAL(clicked()), this, SLOT(on_go_clicked()));


}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::on_go_clicked()
{
    QElapsedTimer timer;
    timer.start();

    fileTree->clear();
    MigrationAssistant m;
    QList<QString> res = m.loadDirectory(textEdit->toPlainText()).toList();

    for(int i=0; i < res.size(); i++)
    {
        fileTree->append(res.value(i));
    }
    fileTree->append("total files found: " + QString::number(res.size()));
    //fileTree->setText(m.loadDirectory(textEdit->toPlainText()));
    label_2->setText("Total Time: " + QString::number(timer.elapsed() / 1000) + " seconds");




}
