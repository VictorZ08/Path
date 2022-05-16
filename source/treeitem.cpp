#include "treeitem.h"
#include "set.h"

#include <QTreeWidget>

/**
    @brief drawingFilesAepInTree
    отрисовыве комплект в трее
    @param sets комплекты
    @param treeWidget трее виджет
*/
void TreeItem::drawingFilesAepInTree(Set& sets,
                                     QTreeWidget* treeWidget)
{
    QTreeWidgetItem* ptwgItemFolder = nullptr;
    for(auto& set: sets.getSetsAep()) {
        ptwgItemFolder = itemsFolders(treeWidget, set.first, ":image/folder.png");
        for(auto& nameFile: set.second)
            itemsFiles(ptwgItemFolder, nameFile, ":image/text.png");
    }
}

/**
    @brief drawingFilesPemiInTree
    отрисовыве файлы в трее
    @param sets комплекты
    @param treeWidget трее виджет
*/
void TreeItem::drawingFilesPemiInTree(Set& inSetsPemi,
                                      QTreeWidget* treeWidget)
{
    for(auto& set: inSetsPemi.getSetsPemi()) {
        itemsFolders(treeWidget, set, ":image/exel.png");
    }
}

/**
    @brief itemsFolders
    отрисовыве каталоги в трее
    @param tree комплекты
    @param pathFolders название каталогов
    @param pathImage иконка для каталогов
*/
QTreeWidgetItem* TreeItem::itemsFolders(QTreeWidget* tree,
                                        const QFileInfo& pathFolders,
                                        const QString& pathImage)
{
    QTreeWidgetItem* itemFolder = new QTreeWidgetItem(tree);
    itemFolder->setText(0, pathFolders.fileName());
    itemFolder->setIcon(0, QPixmap(pathImage));
    return itemFolder;
}

/**
    @brief itemsFiles
    отрисовыве файлы в трее
    @param tree комплекты
    @param pathFile название файлов
    @param pathImage иконка для файлов
*/
void TreeItem::itemsFiles(QTreeWidgetItem* itemFolder,
                          const QFileInfo& pathFile,
                          const QString& pathImage)
{
    QTreeWidgetItem* ptwgItemFiles = nullptr;
    ptwgItemFiles = new QTreeWidgetItem(itemFolder);
    ptwgItemFiles->setText(0, pathFile.fileName());
    ptwgItemFiles->setIcon(0, QPixmap(pathImage));
}

