#include "set.h"

#include <QDir>
#include <QUrl>
#include <QDirIterator>

/**
    @brief Set::Set
    конструктор копирования
    @param rhs копируемые данные
*/
Set::Set(const QFileInfoList& rhs)
{
    m_setsPemi = rhs;
}

/**
    @brief Set::addSetsPemi
    добавляет список данных в контейнер
    @param inUrlList список данных
    @return возвращает false если в списке найдено
    нужное расширение файла
*/
bool Set::addSetsPemi(const QList<QUrl>& inUrlList)
{
    size_t count = inUrlList.size();
    m_setsPemi.reserve(count);
    QFileInfo file;
    for(auto& url: inUrlList) {
        file.setFile(url.toLocalFile());
        m_setsPemi.append(file);
    }
    for(auto& fileExel: m_setsPemi) {
        if(fileExel.suffix() != "xlsx") {
            return true;
        }
    }
    return false;
}

/**
    @brief Set::addSetsPemi
    копирует один список в другой
    @param rhs список данных
*/
bool Set::addSetsPemi(const Set &rhs)
{
    m_setsPemi.append(rhs.m_setsPemi);
    return false;
}

/**
    @brief Set::addSetsAep
    добавляет список данных в контейнер
    @param inUrlList список данных
    @return возвращает false если в списке найдено
    нужное расширение файла
*/
bool Set::addSetsAep(const QList<QUrl>& inUrlList)
{
    QDir dir;
    QStringList filter;
    pairFiFiL setsAep;

    filter << "*.txt";
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot );
    for(auto &url: inUrlList) {
        setsAep.second.clear();
        dir.setPath(url.toLocalFile());

        setsAep.first.setFile(dir.absolutePath());
        if(setsAep.first.isDir() == false)
            return true;

        dir.setNameFilters(filter);
        QFileInfoList filesTxt = dir.entryInfoList();
        setsAep.second.append(filesTxt);

        m_setsAep.push_back(setsAep);
    }
return false;
}

/**
    @brief Set::addSetsAep
    копирует один список в другой
    @param rhs список данных
*/
bool Set::addSetsAep(const Set& rhs)
{
    m_setsAep.append(rhs.m_setsAep);
    return false;
}

/**
    @brief Set::clear
    очищает списки
*/
void Set::clear()
{
    m_setsPemi.clear();
    m_setsAep.clear();
}

