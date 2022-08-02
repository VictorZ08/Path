#ifndef TIMERPEMIWIDGET_H
#define TIMERPEMIWIDGET_H

#include <timer/timerinterface.h>

QT_BEGIN_NAMESPACE
namespace Ui { class TimerPemiWidget; }
QT_END_NAMESPACE

/**
    @author Victor Zaytcev
*/
class TimerPemiWidget final : public TimerInterface
{
    Q_OBJECT

public:
    explicit TimerPemiWidget(SystemTray* inSysTray = nullptr,
                             QWidget* inParent = nullptr);
    ~TimerPemiWidget();

protected:
    void connectSlots() const;
    void initEventFiter();

protected slots:
    void m_clear_pb_clicked();
    void m_back_pb_clicked();
    void m_start_pb_clicked();
    void m_previewTime_le_changed();

    void m_startCheckData_pb_clicked();
    void m_reportCheck_pb_clicked();
    void m_progress_prb_tempStart();

    void showForm();

signals:
    void emitBackUi();

private:
    Ui::TimerPemiWidget* ui = nullptr;
    LoggerWidget* m_logger = nullptr;

    /*РЈРґР°Р»РёС‚СЊ РїРѕСЃР»Рµ РёР·РјРµСЂРµРЅРёР№*/
    /***********************************************/
    void testBenchmark();
    //QList<QDateTime>    m_dateTime;
    /***********************************************/
};
#endif // TIMERPEMIWIDGET_H
