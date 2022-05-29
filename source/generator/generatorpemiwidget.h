#ifndef GENERATORPEMIWIDGET_H
#define GENERATORPEMIWIDGET_H

#include <timer/timerinterface.h>
#include "xlsxdocument.h"

class TableSetsWidget;

struct CoordSerchTable {
    int rowStart = 5;
    int rowEnd = 13;
    int columnStart = rowStart;
    int columnEnd = 13;

};

QT_BEGIN_NAMESPACE
namespace Ui {class GeneratorPemiWidget; }
QT_END_NAMESPACE

/**
    @author Victor Zaytcev
*/
class GeneratorPemiWidget final : public TimerInterface
{
    Q_OBJECT

public:
    explicit GeneratorPemiWidget(SystemTray* inSysTray = nullptr,
                                 QWidget* inParent = nullptr);
    ~GeneratorPemiWidget();

protected:
    void connectSlots() const;
    void initEventFiter();
    void initSpinBoxAmplituda();
    void writeSerialsNumbersTxt(void);
    void editTitul(const QPoint& inPositionTable, //!!!!!!!!!!!!!
                   QXlsx::Document& inExcelFile,
                   const QStringList& inData);
    void editSheet(QXlsx::Document& inExcelFile,
                   QString& inExcelSeetNames);
    void copyFiles(const QFileInfo& inFile,
                   const QString& outFile,
                   const QString& nameFile);
    void initDilog();
    void initDataTitul();
    void writeSNsetsInTxtFile(const QString& outFile,
                              const QString& nameFile,
                              const QVector<QStringList>& dataSets);
    void messageError(); //!!!!!!!!!!!!!!!!
    QPoint searchTitulInExel(const CoordSerchTable& inCoord,
                             const QString& nameCell,
                             const QXlsx::Document& xlsxFile);

protected slots:
    void m_clear_pb_clicked();
    void m_back_pb_clicked();
    void m_start_pb_clicked();
    void m_createTable_pb_clicked();
    void m_previewTime_le_textChanged();

    void m_injener_le_textChanged();
    void m_tehnic_le_textChanged();
    void m_antenneLow1GGz_le_textChanged();
    void m_antenneHeigh1GGz_le_textChanged();
    void m_workplace_le_textChanged();
    void m_amplituda_sb_textChanged();
    void showForm();

signals:
    void emitBackUi();

private:
    Ui::GeneratorPemiWidget* ui = nullptr;
    TableSetsWidget* m_tableSetsWidget = nullptr;

    QPair<QStringList, QStringList> m_prDataTitul;
    QFileInfoList m_outExelFiles;

    double  m_valAmplituda = 0.5;
};

#endif // GENERATORPEMIWIDGET_H
