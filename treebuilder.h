#ifndef TREEBUILDER_H
#define TREEBUILDER_H

#ifdef __linux__
#include<QSet>
#else
#include<Qset>
#endif
#include<QString>
#include<QFileInfo>




class TreeItem
{
public:
    enum ItemType {
            Directory, File
        };

    TreeItem();
    TreeItem(const QString &name, ItemType itemType, TreeItem* parent);

    QString name() const;
    void setName(const QString &name);

    ItemType type() const;
    void setType(const ItemType type);

    TreeItem* parent();
    void setParent(TreeItem* par);

    QString fullPath();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
private:
    QString mName;
    ItemType mType;
    TreeItem *mParent;
};


class FileItem : public TreeItem
{
public:
    FileItem();
    FileItem(const QString &name, const QString &extention, int size, TreeItem* parent);

    QString extention() const;
    void setExtention(const QString &value);

    int size() const;
    void setSize(int value);

    void read(QJsonObject &json);
    void write(QJsonObject &json) const;

private:
    QString mExtention;
    int mSize;
};

class DirectoryItem : public TreeItem
{
public:
    DirectoryItem();
    DirectoryItem(const QString &name, QList<DirectoryItem>* dirChildren, QList<FileItem>* fileChildren, TreeItem* parent);

    QList<DirectoryItem>* dirChildren();
    void setDirChildren(QList<DirectoryItem> &value);

    QList<FileItem>* fileChildren();
    void setFileChildren(QList<FileItem> &value);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

private:
    QList<DirectoryItem> mDirChildren;
    QList<FileItem> mFileChildren;
};

class treeBuilder
{
public:
    treeBuilder();
    void scanDirectory(QString dirPath);

    //QSet<QString> mFilesFound;
    DirectoryItem mRootDir;
private:
    void addToTree(QFileInfo file);
    bool isFileFiltered(QString name);
};


#endif // TREEBUILDER_H
