#include "migrationassistant.h"
#include "qwebdav/QWebDAV.h"
#include "SyncQtOwnCloud.h"
#include "treebuilder.h"


MigrationAssistant::MigrationAssistant() :m_fileTree(m_namePool)
{

    upload();
}

QSet<QString> MigrationAssistant::loadDirectory(QString directory)
{
    treeBuilder builder;
    builder.mFilesFound.clear();
    builder.scanDirectory(directory);
    return builder.mFilesFound;
}

/*
QString MigrationAssistant::loadDirectory(QString directory)
{

    qDebug() << "started loadDirectory!" << endl;
    //QString directory = QDir::rootPath();
    //QString directory = "/Users/dafnathary/Documents/imp";

    qDebug() << "directory: " + directory.toLatin1() << endl;

    Q_ASSERT(QDir(directory).exists());

    m_fileNode = m_fileTree.nodeFor(directory);
    Q_ASSERT(QDir(directory).exists());

    QXmlQuery query(m_namePool);
    query.bindVariable("fileTree", m_fileNode);
    query.setQuery(QUrl("qrc:/queries/wholeTree.xq"));
    //query.setQuery(QUrl("qrc:/queries/impFileFilter.xq"));

    QByteArray output;
    QBuffer buffer(&output);
    buffer.open(QIODevice::WriteOnly);

    QXmlFormatter formatter(query, &buffer);
    query.evaluateTo(&formatter);

    return QString::fromLatin1(output.constData());

//! [6]
}
*/
void MigrationAssistant::evaluateResult()
{

    QXmlQuery query(m_namePool);
    query.bindVariable("fileTree", m_fileNode);
    query.setQuery(QUrl("qrc:/queries/impFileFilter.xq"));

    QByteArray formatterOutput;
    QBuffer buffer(&formatterOutput);
    buffer.open(QIODevice::WriteOnly);

    QXmlFormatter formatter(query, &buffer);
    query.evaluateTo(&formatter);

    //output->setText(QString::fromLatin1(formatterOutput.constData()));
}

void MigrationAssistant::upload()
{

    /*
    SyncQtOwnCloud *account = new SyncQtOwnCloud("test",new QSet<QString>(), "");
    account->initialize(
                "http://demo.owncloud.org",
                "test",
                "test", "/remote.php/webdav", "/Users/dafnathary/Documents/test-img/",15);


    //account->upload(SyncQtOwnCloud::FileInfo("photo.JPG",100));

*/

}
