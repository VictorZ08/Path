#include "ui_timerpemiwidget.h"
#include "timerpemiwidget.h"

#include "sortfiles.h"
#include "timer.h"

#include "chronometr.h"//Удалить после измерений
#include <QDebug>

TimerPemiWidget::TimerPemiWidget(SystemTray* inSysTray,
                                 QWidget* inParent)
            : TimerInterface(inSysTray, inParent)
            , ui{new Ui::TimerPemiWidget}
{
    ui->setupUi(this);

    setParentWiget(this);
    connectSlots();
    initEventFiter();

    ui->m_loadSets_tw->setAttribute(Qt::WA_AcceptDrops, true);
    statusGeneratesFiles("color: rgb(0, 0, 0)",
                         "Статус:-");

    qDebug()<<"Create TimerPemiWidget";
}

//-------------------------------
TimerPemiWidget::~TimerPemiWidget()
{
    delete ui;
    qDebug()<<"~TimerPemiWidget";
}

/**
    @brief TimerPemiWidget::connectSlots
    Установка сигнал - слотов
*/
void TimerPemiWidget::connectSlots() const
{
    connect(ui->m_clear_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_clear_pb_clicked()));

    connect(ui->m_back_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_back_pb_clicked()));

    connect(ui->m_start_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_start_pb_clicked()));

    connect(ui->m_editTime_dte, SIGNAL(dateTimeChanged(const QDateTime& )),
            this, SLOT(m_previewTime_le_changed()));

    connect(ui->m_timeSet_le, SIGNAL(textChanged(const QString& )),
            this, SLOT(m_previewTime_le_changed()));

    connect(this, SIGNAL(emitPreviewTime()),
            this, SLOT(m_previewTime_le_changed()));
}

/**
    @brief TimerPemiWidget::initEventFiter
    Установка эвент фильтра, что бы при нажатии на пробел
    (приложение сворачивается в tree), фокус не смещался на виджеты
*/
void TimerPemiWidget::initEventFiter()
{
    ui->m_clear_pb->installEventFilter(this);
    ui->m_loadSets_tw->installEventFilter(this);
    ui->m_back_pb->installEventFilter(this);
    ui->m_timeSet_le->installEventFilter(this);
    ui->m_editTime_dte->installEventFilter(this);
    ui->m_previewTime_le->setEnabled(false);
    ui->m_start_pb->installEventFilter(this);
}

/**
    @brief TimerPemiWidget::m_clear_pb_clicked
    Очищает форму
*/
void TimerPemiWidget::m_clear_pb_clicked()
{
    clearWiget();
    statusGeneratesFiles("color: rgb(0, 0, 0)", "Статус:-");
}

/**
    @brief TimerPemiWidget::m_previewTime_le_changed
    Выводит предварительные значения даты и времени
*/
void TimerPemiWidget::m_previewTime_le_changed()
{
    previewTime();
}

/**
    @brief TimerPemiWidget::m_back_pb_clicked
    Возвращает в начальную форму
*/
void TimerPemiWidget::m_back_pb_clicked()
{
    this->hide();
    emit emitBackUi();
}

/**
    @brief TimerPemiWidget::m_start_pb_clicked
    Запуск перетасовки файлов и присвоения им время
*/
void TimerPemiWidget::m_start_pb_clicked()
{
    Set& sets = getSetsInTree();

    QVector<QDateTime>::iterator itDT = getDateTime().begin();
    shuffleFiles(sets.getSetsPemi());
    for(auto& set : sets.getSetsPemi()) {
        setDateTimeFiles(set, *itDT++);
    }
    statusGeneratesFiles("color: rgb(255, 255, 255)",
                         "Статус: Готов");
}

/**
    @brief TimerPemiWidget::testBenchmark
    Тест на произвоительность метода
*/
void TimerPemiWidget::testBenchmark()
{

}
