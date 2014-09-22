#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    //ui(new Ui::MainWindow)

{

    setupUi(this);

    connect(pushButton, SIGNAL(clicked()), this, SLOT(on_go_clicked()));

    connect(lineEditUrl, SIGNAL(textChanged(QString)),this, SLOT(slotUrlChanged(QString)));
    connect(lineEditUrl, SIGNAL(editingFinished()),this, SLOT(slotUrlEditFinished()));

    connect(pushButtonConnect, SIGNAL(clicked()), this, SLOT(on_connect_clicked()));

    lineEditUrl->setText("http://demo.owncloud.org/remote.php/webdav/");


}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::on_connect_clicked()
{
    m_migrationAssistant.connectToServer(lineEditUrl->text());

}

void MainWindow::on_go_clicked()
{
    QElapsedTimer timer;
    timer.start();

    fileTree->clear();
    MigrationAssistant m;
    QString res = m.loadDirectory(textEdit->toPlainText());

    fileTree->append(res);
    //fileTree->append("total files found: " + QString::number(res.size()));
    //fileTree->setText(m.loadDirectory(textEdit->toPlainText()));
    label_2->setText("Total Time: " + QString::number(timer.elapsed() / 1000) + " seconds");




}

// slot hit from textChanged of the url entry field.
void MainWindow::slotUrlChanged(const QString& url)
{

    QString newUrl = url;
    if (url.endsWith("index.php")) {
        newUrl.chop(9);
    }
    if (url.endsWith("remote.php/webdav")) {
        newUrl.chop(17);
    }
    if (url.endsWith("remote.php/webdav/")) {
        newUrl.chop(18);
    }
    if (newUrl != url) {
        lineEditUrl->setText(newUrl);
    }

    /*
    if (url.startsWith(QLatin1String("http://"))) {
        _ui.urlLabel->setPixmap( QPixmap(":/mirall/resources/lock-http.png"));
        _ui.urlLabel->setToolTip(tr("This url is NOT secure as it is not encrypted.\n"
                                    "It is not advisable to use it."));
    } else {
        _ui.urlLabel->setPixmap( QPixmap(":/mirall/resources/lock-https.png"));
        _ui.urlLabel->setToolTip(tr("This url is secure. You can use it."));
    }*/
}

void MainWindow::slotUrlEditFinished()
{

    QString url = lineEditUrl->text();
    if (QUrl(url).isRelative()) {
        // no scheme defined, set one
        url.prepend("https://");
    }
    lineEditUrl->setText(url);
}
