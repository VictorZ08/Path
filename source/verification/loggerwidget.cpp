#include "loggerwidget.h"
#include "ui_loggerwidget.h"

#include <QDebug>

LoggerWidget::LoggerWidget(SystemTray* inSysTray,
                           QWidget* inParent)
            : TimerInterface(inSysTray, inParent)
            , ui(new Ui::LoggerWidget)
{
    ui->setupUi(this);
    qDebug()<<"Create LoggerWidget";
}

LoggerWidget::~LoggerWidget()
{
    delete ui;
    qDebug()<<"~LoggerWidget";
}

void LoggerWidget::initEventFiter()
{
    ui->m_reportError_lv->installEventFilter(this);
}

/**
    @brief LoggerWidget::closeEvent
    Закрывает приложение
    @param inEvent Не используется
*/
void LoggerWidget::closeEvent(QCloseEvent* event)
{
    this->hide();
    emit emitBackUi();
}
