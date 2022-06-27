#ifndef LOGGERWIDGET_H
#define LOGGERWIDGET_H

#include <timer/timerinterface.h>

namespace Ui {
class LoggerWidget;
}

class LoggerWidget final : public TimerInterface
{
    Q_OBJECT

public:
    explicit LoggerWidget(SystemTray* inSysTray = nullptr,
                          QWidget* inParent  = nullptr);
    ~LoggerWidget();

protected:
    void initEventFiter();
    virtual void closeEvent(QCloseEvent* inEvent) override;

signals:
    void emitBackUi();

private:
    Ui::LoggerWidget *ui;
};

#endif // LOGGERWIDGET_H
