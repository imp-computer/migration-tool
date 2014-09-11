#ifndef MIGRATIONASSISTANT_H
#define MIGRATIONASSISTANT_H

#include <QtXmlPatterns>
#include <QXmlNamePool>

#include "qwebdav/QWebDAV.h"
#include "filetree.h"
#include "SyncQtOwnCloud.h"




class MigrationAssistant
{
public:

    MigrationAssistant();

    QSet<QString> loadDirectory(QString path);
    void upload();
    void evaluateResult();



    const QXmlNamePool  m_namePool;
    const FileTree      m_fileTree;
    QXmlNodeModelIndex  m_fileNode;

};

#endif // MIGRATIONASSISTANT_H
