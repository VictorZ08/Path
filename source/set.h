#ifndef SET_H
#define SET_H

#include <QFileInfo>

using pairFiFiL = QPair<QFileInfo, QFileInfoList>;

/**
    @author Victor Zaytcev
*/
class Set
{
public:
    Set(){};
    Set(const QFileInfoList& rhs);
    bool addSetsPemi(const QList<QUrl>& inUrlList);
    bool addSetsPemi(const Set& rhs);
    bool addSetsAep(const QList<QUrl>& inUrlList);
    bool addSetsAep(const Set& rhs);
    void clear();

    size_t countSetsPemi() { return m_setsPemi.count(); }
    size_t countSetsAep() { return m_setsAep.count(); }
    size_t countFilesAllSetsAep();
    QFileInfoList& getSetsPemi() { return m_setsPemi; } //const
    QVector<pairFiFiL>& getSetsAep() { return m_setsAep; } //const

private:
    QVector<pairFiFiL> m_setsAep;
    QFileInfoList m_setsPemi;
};

#endif // SET_H
