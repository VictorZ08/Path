#ifndef TABLEDATA_H
#define TABLEDATA_H

#include "QString"
#include "QVector"

struct sData {
    QString numSet {"-"};
    QString serialNumber {"-"};
    QString znp {"-"};
};

class TableData
{
public:
    TableData();

    void setDataTable();
    QVector<sData> dataTable() const;

private:
    QVector<sData> m_sData;
};

#endif // TABLEDATA_H
