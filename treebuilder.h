#ifndef TREEBUILDER_H
#define TREEBUILDER_H

#include<Qset>
#include<QString>
#include<QFileInfo>


class treeBuilder
{
public:
    treeBuilder();
    void scanDirectory(QString dirPath);

    QSet<QString> mFilesFound;
private:
    void addToTree(QFileInfo file);
    bool isFileFiltered(QString name);
};

#endif // TREEBUILDER_H
