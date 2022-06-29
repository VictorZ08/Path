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

void LoggerWidget::setError(const QStringList& inErrors)
{
    ui->m_reportError_te->clear();
    ui->m_reportError_te->setTextColor(QColor("black"));

    QStringList::const_iterator it = inErrors.begin();
    for(; it != inErrors.end(); ++it) {
        ui->m_reportError_te->insertPlainText(*it+"\n");
    }
}

void LoggerWidget::initEventFilter()
{
    ui->m_reportError_te->installEventFilter(this);
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
