#ifndef TABLESETSWIDGET_H
#define TABLESETSWIDGET_H

#include <QDialog>

class SystemTray;

namespace Ui {
class TableSetsWidget;
}

/**
    @author Victor Zaytcev
*/
class TableSetsWidget final : public QDialog
{
    Q_OBJECT

public:
    explicit TableSetsWidget(SystemTray* inSysTray = nullptr,
                             QWidget*    inParent  = nullptr);
    ~TableSetsWidget();

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    int getColumn() const { return m_column; } //????
    int getRow() const { return m_row; } //????!

    QVector<QStringList>& getTableValues();

protected:
    void connectSlots() const;
    void initEventFiter();
    void createTable();
    void setCombination(const int inCombination_rb);
    bool saveTableData();
    void messageError();

    virtual bool eventFilter(QObject* inTarget,
                             QEvent* inEvent) override;
    virtual void closeEvent(QCloseEvent* inEvent) override;


public slots:
    void m_addItem_pb_clicked();
    void m_removeItem_pb_clicked();
    void m_select_gb_clicked();
    void moveCursorCell();

signals:
    void emitBackUi();

private:
    Ui::TableSetsWidget*  ui = nullptr;
    SystemTray* m_sysTray = nullptr;
    QVector<QStringList> m_tableValues;

    int m_row = 0;
    int m_column = 0;
    int m_combination_rb = -1;
};

#endif // TABLESETSWIDGET_H
