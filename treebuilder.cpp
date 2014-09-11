#include "treebuilder.h"

#include <QString>
#include <QDir>
#include <QRunnable>
#include <QThreadPool>
#include <QDebug>

treeBuilder::treeBuilder()
{
}

class ScanDirectoryTask : public QRunnable
{
public:
    QString path;
    QThreadPool* pool;
    QSet<QString>* mFilesFound;
    QMutex* poolLocker;
    QMutex* listLocker;
    ScanDirectoryTask(QThreadPool* threadPool,
                      QString dirPath,
                      QMutex* poolL,
                      QMutex* listL,
                      QSet<QString>* FilesFound);

    void addToTree(QFileInfo file);
    bool isFileFiltered(QFileInfo file);
    void run();
};

ScanDirectoryTask::ScanDirectoryTask(QThreadPool* threadPool,
                                          QString dirPath,
                                          QMutex* poolL,
                                          QMutex* listL,
                                          QSet<QString>* FilesFound)
{
    path = dirPath;
    pool = threadPool;
    poolLocker = poolL;
    listLocker = listL;
    mFilesFound = FilesFound;
}

void ScanDirectoryTask::addToTree(QFileInfo file)
{
    listLocker->lock();
    if(file.isFile())
    {
        mFilesFound->insert(file.absoluteFilePath());
        //qDebug() << file.absoluteFilePath();
    }
    listLocker->unlock();
}

bool ScanDirectoryTask::isFileFiltered(QFileInfo file)
{
    if(file.isDir())
    {
        if(file.fileName().contains("."))
            return true;
    }
    return false;
}
void ScanDirectoryTask::run()
{
    QDir dir(path);
    dir.setFilter(QDir::Files |
                  QDir::NoDot |
                  QDir::NoDotDot|
                  QDir::NoSymLinks |
                  QDir::AllEntries /*|
                  QDir::Hidden*/);
    QStringList list = dir.entryList();
    for( int i = 0; i < list.size(); i++ ) {
        QString name = list.at(i);
        QFileInfo file( path + "/" + name );
        if( isFileFiltered(file) ) {
            continue;
        }

        addToTree(file);

        // Check if it is a directory, and if so, process it
        if ( file.isDir() ) {
            poolLocker->lock();
            ScanDirectoryTask* task = new ScanDirectoryTask(pool, file.absoluteFilePath(),poolLocker,listLocker, mFilesFound);
            pool->start(task);
            poolLocker->unlock();
        }
    }
}


void treeBuilder::scanDirectory(QString dirPath)
{
    QThreadPool threadPool;
    QMutex poolLocker;
    QMutex listLocker;
    ScanDirectoryTask* task = new ScanDirectoryTask(&threadPool, dirPath,&poolLocker,&listLocker, &mFilesFound);
    threadPool.start(task);

    while(threadPool.activeThreadCount() > 0);

    return;
}

/*
void treeBuilder::scanDirectory(QString dirPath)
{
    QDir dir(dirPath);
    dir.setFilter(QDir::Files |
                  QDir::NoDot |
                  QDir::NoDotDot|
                  QDir::AllEntries |
                  QDir::Hidden);
    QStringList list = dir.entryList();
    for( int i = 0; i < list.size(); i++ ) {
        QString name = list.at(i);
        if( isFileFiltered(name) ) {
            continue;
        }
        QFileInfo file( dirPath + "/" + name );

        addToTree(file);

        // Check if it is a directory, and if so, process it
        if ( file.isDir() ) {
            scanDirectory(file.absoluteFilePath() );
        }
    }
}
*/

void treeBuilder::addToTree(QFileInfo file)
{
    if(file.isFile())
        mFilesFound.insert(file.absoluteFilePath());
}

bool treeBuilder::isFileFiltered(QString name)
{

    return false;
}
