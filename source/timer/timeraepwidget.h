#ifndef TIMERAEPWIDGET_H
#define TIMERAEPWIDGET_H

#include <timer/timerinterface.h>

QT_BEGIN_NAMESPACE
namespace Ui { class TimerAepWidget; }
QT_END_NAMESPACE

/**
    @author Victor Zaytcev
*/
class TimerAepWidget final : public TimerInterface
{
    Q_OBJECT

public:
    explicit TimerAepWidget(SystemTray* inSysTray = nullptr,
                            QWidget* inParent  = nullptr);
    ~TimerAepWidget();

protected:
    void connectSlots() const;
    void initEventFiter();

private slots:
    void m_clear_pb_clicked();
    void m_back_pb_clicked();
    void m_start_pb_clicked();
    void m_previewTime_le_changed();

    void m_progress_prb_tempStart();

signals:
    void emitBackUi();
    void emitStatus_prb();

private:
    Ui::TimerAepWidget* ui = nullptr;
};

#endif // TIMERAEPWIDGET_H
