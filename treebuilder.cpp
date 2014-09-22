#include "treebuilder.h"

#include <QString>
#include <QDir>
#include <QRunnable>
#include <QThreadPool>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>




/*  Tree Item  */
/********************************************************************************************************************/

TreeItem::TreeItem(const QString &name, ItemType itemType, TreeItem* parent)
    :mName(name)
    , mType(itemType)
    , mParent(parent)
{
}

TreeItem::TreeItem(){
    TreeItem("", TreeItem::Directory, NULL);
}

QString TreeItem::name() const
{
    return mName;
}

void TreeItem::setName(const QString &name)
{
    mName = name;
}

TreeItem::ItemType TreeItem::type() const
{
    return mType;
}

void TreeItem::setType(ItemType itemType)
{
    mType = itemType;
}

TreeItem* TreeItem::parent()
{
    return mParent;
}

void TreeItem::setParent(TreeItem* par)
{
    mParent = par;
}

QString TreeItem::fullPath()
{
    if(parent() == NULL)
        return mName;
    return mParent->fullPath() + mName;
}

void TreeItem::read(const QJsonObject &json)
{
    mName = json["name"].toString();
    mType = ItemType(qRound(json["type"].toDouble()));
}

void TreeItem::write(QJsonObject &json) const
{
    json["name"] = mName;
    json["type"] = mType;
}

/*  File Item  */
/********************************************************************************************************************/
FileItem::FileItem(const QString &name, const QString &extention, int size, TreeItem* parent)
    :TreeItem(name, TreeItem::File, parent)
    , mExtention(extention)
    , mSize(size)
{
}

FileItem::FileItem(){
    FileItem(QString(), QString(), 0, NULL);
}

void FileItem::read(QJsonObject &json)
{
    mExtention = json["extention"].toString();
    mSize = json["size"].toInt();
    TreeItem::read(json);
}

void FileItem::write(QJsonObject &json) const
{
    json["extention"] = mExtention;
    json["size"] = mSize;
    TreeItem::write(json);
}

QString FileItem::extention() const
{
return mExtention;
}

void FileItem::setExtention(const QString &value)
{
    mExtention = value;
}
int FileItem::size() const
{
    return mSize;
}

void FileItem::setSize(int value)
{
    mSize = value;
}

/*  Directory Item  */
/********************************************************************************************************************/
DirectoryItem::DirectoryItem(const QString &name, QList<DirectoryItem>* dirChildren, QList<FileItem>* fileChildren, TreeItem* parent)
    :TreeItem(name, TreeItem::Directory, parent)
    , mDirChildren(*dirChildren)
    , mFileChildren(*fileChildren)
{
}

DirectoryItem::DirectoryItem(){
    DirectoryItem("", new QList<DirectoryItem>(), new QList<FileItem>(), NULL);
}

void DirectoryItem::read(const QJsonObject &json)
{
    TreeItem::read(json);
    QJsonArray dirArr = json["dirChildren"].toArray();
    QJsonArray fileArr = json["fileChildren"].toArray();
    mDirChildren.clear();
    mFileChildren.clear();
    for (int i =0; i< dirArr.size(); i++)
    {
        QJsonObject obj = dirArr[i].toObject();
        DirectoryItem dir;
        dir.read(obj);
        mDirChildren.append(dir);
    }
    for (int i =0; i< fileArr.size(); i++)
    {
        QJsonObject obj = fileArr[i].toObject();
        FileItem file;
        file.read(obj);
        mFileChildren.append(file);
    }
}

void DirectoryItem::write(QJsonObject &json) const
{
    QJsonArray fileArr;
    foreach (const FileItem file, mFileChildren) {
        QJsonObject obj;
        file.write(obj);
        fileArr.append(obj);
    }
    json["fileChildren"] = fileArr;

    QJsonArray dirArr;
    foreach (const DirectoryItem dir, mDirChildren) {
        QJsonObject obj;
        dir.write(obj);
        dirArr.append(obj);
    }
    json["dirChildren"] = dirArr;

    TreeItem::write(json);
}

QList<DirectoryItem>* DirectoryItem::dirChildren()
{
    return &mDirChildren;
}

void DirectoryItem::setDirChildren( QList<DirectoryItem> &value)
{
    mDirChildren = value;
}

QList<FileItem>* DirectoryItem::fileChildren()
{
    return &mFileChildren;
}

void DirectoryItem::setFileChildren(QList<FileItem> &value)
{
    mFileChildren = value;
}







treeBuilder::treeBuilder()
{
}

class ScanDirectoryTask : public QRunnable
{
public:
    DirectoryItem* rootDir;
    QThreadPool* pool;
    QMutex* poolLocker;
    QMutex* dirLocker;
    ScanDirectoryTask(QThreadPool* threadPool,
                      QMutex* poolL,
                      QMutex* dirL,
                      DirectoryItem* dir);

    void addFileToTree(QFileInfo file);
    bool isFileFiltered(QFileInfo file);
    void run();
};

ScanDirectoryTask::ScanDirectoryTask(QThreadPool* threadPool,
                                          QMutex* poolL,
                                          QMutex* dirL,
                                          DirectoryItem* dir)
{
    pool = threadPool;
    poolLocker = poolL;
    dirLocker = dirL;
    rootDir = dir;
}

void ScanDirectoryTask::addFileToTree(QFileInfo file)
{
    dirLocker->lock();

    rootDir->fileChildren()->append(FileItem(file.baseName(), file.suffix(), file.size(), (TreeItem*)rootDir));

    dirLocker->unlock();
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
    qDebug() << "Starting task on dir: " << rootDir->fullPath();
    qDebug() << ">> dir name: " << rootDir->name();

    QDir dir(rootDir->fullPath());
    dir.setFilter(QDir::Files |
                  QDir::NoDot |
                  QDir::NoDotDot|
                  QDir::NoSymLinks |
                  QDir::AllEntries );
    QStringList list = dir.entryList();

    qDebug() << ">> dir has : " << list.size() << " children.";
    for( int i = 0; i < list.size(); i++ )
    {
        QString name = list.at(i);
        QFileInfo file( rootDir->fullPath() + "/" + name );
        if( isFileFiltered(file) )
            continue;

        if(file.isFile())
            addFileToTree(file);

        // Check if it is a directory, and if so, process it
        else if ( file.isDir() )
        {
            qDebug() << ">> found dir : " << file.baseName();
            //DirectoryItem* directory = new DirectoryItem(file.baseName(), QList<DirectoryItem>(), QList<FileItem>(), rootDir);
            dirLocker ->lock();
            rootDir->dirChildren()->append(DirectoryItem(file.baseName() + "/",new QList<DirectoryItem>(),new QList<FileItem>(), rootDir));
            qDebug() << ">> the root: " << rootDir->name() << " has " << rootDir->dirChildren()->size() << " dirChildren";
            dirLocker->unlock();
            poolLocker->lock();

            ScanDirectoryTask* task = new ScanDirectoryTask(pool,poolLocker,dirLocker, &(rootDir->dirChildren()->last()));
            pool->start(task);
            poolLocker->unlock();
        }
    }
}


void treeBuilder::scanDirectory(QString dirPath)
{
    QThreadPool threadPool;
    QMutex poolLocker;
    QMutex dirLocker;
    QString path = dirPath;

    if(!path.endsWith("/"))
    {
        path.append("/");
    }

    mRootDir.setName(path);
    mRootDir.setParent(NULL);
    //threadPool.setMaxThreadCount(1);
    ScanDirectoryTask* task = new ScanDirectoryTask(&threadPool ,&poolLocker,&dirLocker, &mRootDir);
    threadPool.start(task);

    while(threadPool.activeThreadCount() > 0);

    return;
}

bool treeBuilder::isFileFiltered(QString name)
{

    return false;
}

