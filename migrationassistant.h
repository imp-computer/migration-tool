#ifndef MIGRATIONASSISTANT_H
#define MIGRATIONASSISTANT_H

#include <QtXmlPatterns>
#include <QXmlNamePool>

#include "qwebdav/QWebDAV.h"
#include "filetree.h"
#include "SyncQtOwnCloud.h"
#include "account.h"






class MigrationAssistant : QObject
{
    Q_OBJECT
public:

    MigrationAssistant();

    QString loadDirectory(QString path);
    void upload();
    void evaluateResult();

    void connectToServer(QString url);
    void lsRemoteDir(QString path);
    void mkRemoteDir(QString path);
    void uploadFile(QString filePath, QString remotePath);

    const QXmlNamePool  m_namePool;
    const FileTree      m_fileTree;
    QXmlNodeModelIndex  m_fileNode;
    impPcClient::Account m_account;
private slots:
    void slotOwnCloudFoundAuth(const QUrl& url, const QVariantMap &info);
    void slotNoOwnCloudFoundAuth(QNetworkReply *reply);
    void slotNoOwnCloudFoundAuthTimeout(const QUrl&url);
    void slotUpdateDirectories(const QStringList &list);
    void slotCreateRemoteFolderFinished(QNetworkReply::NetworkError error);
    void slotHandleNetworkError(QNetworkReply *reply);
};

#endif // MIGRATIONASSISTANT_H
