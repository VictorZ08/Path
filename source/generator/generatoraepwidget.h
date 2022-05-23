#ifndef GENERATORAEPWIDGET_H
#define GENERATORAEPWIDGET_H

#include <timer/timerinterface.h>
#include <QMap>

class ReportAepWidget;

QT_BEGIN_NAMESPACE
namespace Ui {class GeneratorAepWidget; }
QT_END_NAMESPACE

/**
    @author Victor Zaytcev
*/
class GeneratorAepWidget final : public TimerInterface
{
    Q_OBJECT

public:
    explicit GeneratorAepWidget(SystemTray* inSysTray = nullptr,
                               QWidget* inParent = nullptr);
    ~GeneratorAepWidget();

protected:
    void connectSlots() const;
    void initEventFiter();
    void initSpinBoxAmplituda();
    void createSet(QVector<pairFiFiL>::iterator it, Set& inSet);
    void createFolders();
    void copyFiles(QFileInfoList& inPathFiles);
    void generatorsValuesAep(QFileInfo& inPathFiles);
    QString read(const QString& inPathFile) const;
    void write(const QString& inPathFileOut, QString& inContent);
    void genGetOneStr(QString& inBuff);

    QString generStrBuffer(const QString&& inBuff);

protected slots:
    void m_clear_pb_clicked();
    void m_back_pb_clicked();
    void m_start_pb_clicked();
    void m_report_pb_clicked();
    void m_previewTime_le_changed();
    void m_amplituda_dsb_changed();
    void showForm();

signals:
    void emitBackUi();

private:
    Ui::GeneratorAepWidget* ui   = nullptr;
    ReportAepWidget* m_reportAep = nullptr;

    QMap<QString, QString>  m_mapStr;
    QFileInfoList           m_outPathFiles;

    double  m_valAmplituda = 0.5;
};

#endif // GENERATORAEPWIDGET_H
