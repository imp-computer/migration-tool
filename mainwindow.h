#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMainWindow>

#include "migrationassistant.h"
#include "ui_mainwindow.h"
#include "account.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow, private Ui_MainWindow
{
   Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    void determinAuthType(impPcClient::Account *account, const QString &urlString);
    //Ui::MainWindow *ui;
    MigrationAssistant m_migrationAssistant;

private slots:
    void on_go_clicked();
    void on_connect_clicked();
    void slotUrlChanged(const QString& url);
    void slotUrlEditFinished();

};

#endif // MAINWINDOW_H
