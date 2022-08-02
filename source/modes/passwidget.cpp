#include "ui_passwidget.h"
#include "passwidget.h"

#include "QDebug"

//-------------------------------
PassWidget::PassWidget(QWidget* parent)
            : QWidget(parent)
            , ui(new Ui::PassWidget)
            , m_password("СИшник")
{
    ui->setupUi(this);
    connectSlots();

    qDebug()<<"Create PassWidget";
}

//-------------------------------
PassWidget::~PassWidget()
{
    delete ui;
    qDebug()<<"~PassWidget";
}

//-------------------------------
void PassWidget::setPointWidget(QWidget* ptrWidget)
{
    m_ptrForm = qobject_cast<QWidget* >(ptrWidget);
}

//-------------------------------
void PassWidget::connectSlots() const
{
    connect(ui->m_passEnter_le, SIGNAL(returnPressed()),
            this, SLOT(m_passEnter_le_retPres()));
}

//-------------------------------
void PassWidget::m_passEnter_le_retPres()
{
    if(ui->m_passEnter_le->text() == m_password) {
        ui->m_passEnter_le->clear();
        this->hide();
        m_ptrForm->show();
    }
}
