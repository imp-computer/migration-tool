#include "migrationassistant.h"
#include "qwebdav/QWebDAV.h"
#include "SyncQtOwnCloud.h"
#include "treebuilder.h"
#include "account.h"
#include "creds/httpcredentials.h"
#include "networkjobs.h"
#include <QJsonDocument>

MigrationAssistant::MigrationAssistant() :m_fileTree(m_namePool)
{

    upload();
}

QString MigrationAssistant::loadDirectory(QString directory)
{
    treeBuilder builder;
    builder.scanDirectory(directory);
    QJsonObject output;

    builder.mRootDir.write(output);
    QJsonDocument doc = QJsonDocument(output);
    return QString::fromLatin1(doc.toJson());
}


void MigrationAssistant::upload()
{





}

void MigrationAssistant::connectToServer(QString urlString)
{
    m_account.setCredentials(new impPcClient::HttpCredentials("test", "test"));

    QUrl url = QUrl::fromUserInput(urlString);
    // fromUserInput defaults to http, not http if no scheme is specified
    if (!urlString.startsWith("http://") && !urlString.startsWith("https://")) {
        url.setScheme("https");
    }
    m_account.setUrl(url);

    impPcClient::CheckServerJob *job = new impPcClient::CheckServerJob(&m_account,false, this);
    job->setIgnoreCredentialFailure(true);
    connect(job, SIGNAL(instanceFound(QUrl,QVariantMap)), SLOT(slotOwnCloudFoundAuth(QUrl,QVariantMap)));
    connect(job, SIGNAL(instanceNotFound(QNetworkReply*)), SLOT(slotNoOwnCloudFoundAuth(QNetworkReply*)));
    connect(job, SIGNAL(timeout(const QUrl&)), SLOT(slotNoOwnCloudFoundAuthTimeout(const QUrl&)));
    job->setTimeout(10*1000);
    job->start();



}

void MigrationAssistant::lsRemoteDir(QString path)
{
    impPcClient::LsColJob *job = new impPcClient::LsColJob(&m_account, path, this);
    connect(job, SIGNAL(directoryListing(QStringList)),
            SLOT(slotUpdateDirectories(QStringList)));
    job->start();
}

void MigrationAssistant::mkRemoteDir(QString path)
{
    impPcClient::MkColJob *job = new impPcClient::MkColJob(&m_account, path, this);
    /* check the owncloud configuration file and query the ownCloud */
    connect(job, SIGNAL(finished(QNetworkReply::NetworkError)),
                 SLOT(slotCreateRemoteFolderFinished(QNetworkReply::NetworkError)));
    connect(job, SIGNAL(networkError(QNetworkReply*)), SLOT(slotHandleNetworkError(QNetworkReply*)));
    job->start();
}

void MigrationAssistant::uploadFile(QString filePath, QString remotePath)
{
    impPcClient::UploadFileJob *job = new impPcClient::UploadFileJob(&m_account, filePath, remotePath, this);
    /* check the owncloud configuration file and query the ownCloud */
    //connect(job, SIGNAL(finished(QNetworkReply::NetworkError)),
    //             SLOT(slotCreateRemoteFolderFinished(QNetworkReply::NetworkError)));
    //connect(job, SIGNAL(networkError(QNetworkReply*)), SLOT(slotHandleNetworkError(QNetworkReply*)));
    job->start();
}

void MigrationAssistant::slotCreateRemoteFolderFinished(QNetworkReply::NetworkError error)
{
    if (error == QNetworkReply::NoError) {
        qDebug() << "** webdav mkdir request finished";

    }

}

void MigrationAssistant::slotHandleNetworkError(QNetworkReply *reply)
{
    qDebug() << "** webdav mkdir request failed:" << reply->error();
}

void MigrationAssistant::slotUpdateDirectories(const QStringList &list)
{
    qDebug() << "Directories in cloud:";
    for (int i =0; i<list.size(); i++)
    {
       qDebug() << list.at(i);
    }

}

void MigrationAssistant::slotOwnCloudFoundAuth(const QUrl& url, const QVariantMap &info)
{
    /*
    _ocWizard->appendToConfigurationLog(tr("<font color=\"green\">Successfully connected to %1: %2 version %3 (%4)</font><br/><br/>")
                                        .arg(url.toString())
                                        .arg(Theme::instance()->appNameGUI())
                                        .arg(CheckServerJob::versionString(info))
                                        .arg(CheckServerJob::version(info)));

                                        */
    QString p = url.path();
    if (p.endsWith("/status.php")) {
        // We might be redirected, update the account
        QUrl redirectedUrl = url;
        redirectedUrl.setPath(url.path().left(url.path().length() - 11));
        m_account.setUrl(redirectedUrl);
        qDebug() << Q_FUNC_INFO << " was redirected to" << redirectedUrl.toString();
    }

    lsRemoteDir("/");
    mkRemoteDir("Bla2");
    mkRemoteDir("Bla2/bla3");
    mkRemoteDir("Bla2/bla3/bla4");
    uploadFile("/Users/dafnathary/Documents/test-img/photo.JPG", "remote.php/webdav/photo.JPG");
    uploadFile("/Users/dafnathary/Documents/test-img/photo.JPG", "remote.php/webdav/gili/test/photo.JPG");


    /*
    impPcClient::DetermineAuthTypeJob *job = new impPcClient::DetermineAuthTypeJob(m_account, this);
    job->setIgnoreCredentialFailure(true);
    connect(job, SIGNAL(authType(WizardCommon::AuthType)),
            _ocWizard, SLOT(setAuthType(WizardCommon::AuthType)));
    job->start();
    */
}

void MigrationAssistant::slotNoOwnCloudFoundAuth(QNetworkReply *reply)
{
   qDebug()  << tr("Failed to connect to %1 at %2:<br/>%3")
                            //.arg(Theme::instance()->appNameGUI())
                            .arg("test")
                            .arg(reply->url().toString())
                            .arg(reply->errorString());

}

void MigrationAssistant::slotNoOwnCloudFoundAuthTimeout(const QUrl&url)
{

    qDebug() << tr("Failed to connect to %1 at %2:<br/>%3")
                            //.arg(Theme::instance()->appNameGUI())
                            .arg("test")
                            .arg(url.toString())
                            .arg("Timeout");
}
