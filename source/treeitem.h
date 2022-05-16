#ifndef TREEITEM_H
#define TREEITEM_H

class Set;
class QString;
class QFileInfo;
class QTreeWidget;
class QTreeWidgetItem;

/**
    @author Victor Zaytcev
*/
class TreeItem
{
public:
    static void drawingFilesAepInTree(Set& sets,
                                      QTreeWidget* treeW);

    static void drawingFilesPemiInTree(Set& inSetsPemi,
                                 QTreeWidget * treeW);
private:
    static QTreeWidgetItem* itemsFolders(QTreeWidget* treeW,
                                         const QFileInfo& pathFolders,
                                         const QString& pathImage);

    static void itemsFiles(QTreeWidgetItem* itemFolder,
                           const QFileInfo& pathFile,
                           const QString& pathImage);
};

#endif // TREEITEM_H
