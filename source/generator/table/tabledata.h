#ifndef TABLEDATA_H
#define TABLEDATA_H

#include "QString"
#include "QVector"

struct sData {
    QString oneColumn {"-"};
    QString twoColumn {"-"};
    QString threColumn{"-"};
};

class TableData
{
public:
    TableData();
    void setDataTable();
    QVector<sData> &dataTable();

private:
    QVector<sData> m_sData;
};

#endif // TABLEDATA_H
