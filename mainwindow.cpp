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

    MigrationAssistant m;
    fileTree->setText(m.loadDirectory(textEdit->toPlainText()));
    label_2->setText("Total Time: " + QString::number(timer.elapsed() / 60000) + " minutes");




}
