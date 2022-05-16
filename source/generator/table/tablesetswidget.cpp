#include "ui_tablesetswidget.h"
#include "tablesetswidget.h"
#include "modes/systemtray.h"

#include <QMessageBox>
#include <QKeyEvent>
#include <QPointer>
#include "QDebug"

const QStringList kCombination[2] = {
                                {"Комплект", "s/n", "ZNP"},
                                {"s/n", "ZNP"} };

/**
    @brief TableSetsWidget::TableSetsWidget
    Конструктор
    @param inSysTray указатель на системный трай
    @param inParent указатель на родительский объект,
    требуется для удаления
*/
TableSetsWidget::TableSetsWidget(SystemTray* inSysTray,
                                    QWidget* inParent)
                    : QDialog(inParent)
                    , ui(new Ui::TableSetsWidget)
                    , m_sysTray{inSysTray}
{
    ui->setupUi(this);

    connectSlots();
    initEventFiter();

    qDebug()<<"Create TableSetsWidget";
}

/**
    @brief TableSetsWidget::~TableSetsWidget
    Деструктор
*/
TableSetsWidget::~TableSetsWidget()
{
    delete ui;
    qDebug()<<"~TableSetsWidget";
}

/**
    @brief TableSetsWidget::connectSlots
    Установка сигнал - слотов
*/
void TableSetsWidget::connectSlots() const
{

    connect(ui->m_serialNumberAndZNP_rb, SIGNAL(clicked(bool)),
            this, SLOT(m_select_gb_clicked()));

    connect(ui->m_numberAndSerialNumberAndZNP_rb, SIGNAL(clicked(bool)),
            this, SLOT(m_select_gb_clicked()));

    connect(ui->m_addItem_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_addItem_pb_clicked()));

    connect(ui->m_removeItem_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_removeItem_pb_clicked()));

    connect(ui->m_tableSet_tw, SIGNAL(cellChanged(int, int)),
            this, SLOT(moveCursorCell()));
}

/**
    @brief TableSetsWidget::initEventFiter
    Установка эвент фильтра, что бы при нажатии на пробел
    (приложение сворачивается в tree), фокус не смещался на виджеты
*/
void TableSetsWidget::initEventFiter()
{
    ui->m_serialNumberAndZNP_rb->installEventFilter(this);
    ui->m_numberAndSerialNumberAndZNP_rb->installEventFilter(this);
    ui->m_addItem_pb->installEventFilter(this);
    ui->m_removeItem_pb->installEventFilter(this);
    ui->m_tableSet_tw->installEventFilter(this);
}

/**
    @brief TableSetsWidget::tableValues
    Данные таблицы
    @return возвращает данные таблицы
*/
QVector<QStringList>& TableSetsWidget::tableValues()
{
    return m_tableValues;
}

/**
    @brief TableSetsWidget::messageError
    Выводит сообщение если ячейка
    таблицы не заполненны
*/
void TableSetsWidget::messageError()
{
    QMessageBox::warning(this,
                         tr("Предупреждение"),
                         tr("Заполните все ячейки!"),
                         QMessageBox::Close);
return;
}

/**
    @brief TableSetsWidget::m_addItem_pb_clicked
    Добавляет пункты в таблицу
*/
void TableSetsWidget::m_addItem_pb_clicked()
{
    ++m_row;
    createTable();
}

/**
    @brief TableSetsWidget::m_removeItem_pb_clicked
    Удаляет пункты из таблицы
*/
void TableSetsWidget::m_removeItem_pb_clicked()
{
    --m_row;
    if(m_row < 0)
        m_row = 0;

    createTable();
}

/**
    @brief TableSetsWidget::m_select_gb_clicked
    Выбор комбинации для заполнения таблицы
*/
void TableSetsWidget::m_select_gb_clicked()
{
    if (ui->m_numberAndSerialNumberAndZNP_rb->isChecked())
        setCombination(0);
    else if (ui->m_serialNumberAndZNP_rb->isChecked())
            setCombination(1);

    createTable();
}

/**
    @brief TableSetsWidget::setCombination
    Установка комбинации для таблицы
    @param inCombination_rb номер комбинации
*/
void TableSetsWidget::setCombination(const int inCombination_rb)
{
    m_combination_rb = inCombination_rb;
}

/**
    @brief TableSetsWidget::createTable
    Создание таблицы
*/
void TableSetsWidget::createTable()
{
    m_column = kCombination[m_combination_rb].count();
    ui->m_tableSet_tw->setRowCount(m_row);
    ui->m_tableSet_tw->setColumnCount(m_column);
    ui->m_tableSet_tw->setHorizontalHeaderLabels(kCombination[m_combination_rb]);
}

//-------------------------------
bool TableSetsWidget::eventFilter(QObject* inTarget,
                                  QEvent* inEvent)
{
    if (inEvent->type() == QEvent::KeyPress) {
          QKeyEvent *keyEvent = dynamic_cast<QKeyEvent* >(inEvent);
          if (keyEvent->key() == Qt::Key_Space) {
              this->hide();
              m_sysTray->setPointWidget(this);
              m_sysTray->showTrayIcon();
              return true;
          }
    }
return false;
}

/**
    @brief TableSetsWidget::moveCursorCell
    Перемещение выделения цвета ячеик в таблице
*/
void TableSetsWidget::moveCursorCell()
{
    qint64 col = ui->m_tableSet_tw->currentColumn();
    qint64 row = ui->m_tableSet_tw->currentRow();
    if(col < m_column-1) {
        QModelIndex newIndex = ui->m_tableSet_tw->model()->index(row, ++col);
        ui->m_tableSet_tw->setCurrentIndex(newIndex);
    }
    else if(row < m_row-1) {
        col = 0;
        QModelIndex newIndex = ui->m_tableSet_tw->model()->index(++row, col);
        ui->m_tableSet_tw->setCurrentIndex(newIndex);
    }
    else {
        row = 0; col = 0;
        QModelIndex newIndex = ui->m_tableSet_tw->model()->index(row, col);
        ui->m_tableSet_tw->setCurrentIndex(newIndex);
    }
}

//-------------------------------
void TableSetsWidget::closeEvent(QCloseEvent* inEvent)
{
    if(saveTableData()) {
        messageError();
    }
    this->hide();
    emit emitBackUi();
}

/**
    @brief TableSetsWidget::saveTableData
    Сохранение таблицы
    @return если ячейка не заполненна вернет true
*/
bool TableSetsWidget::saveTableData()
{
    m_tableValues.clear();
    m_tableValues.reserve(m_row);

    QTableWidgetItem *p_twi = nullptr;
    p_twi = ui->m_tableSet_tw->horizontalHeaderItem(0);
    if(p_twi == nullptr) {
        return true;
    }
    QString str = p_twi->text();
    for(int r = 0; r < m_row; ++r ) {
        QStringList sl;
           if(str == "Комплект") {
             for(int col = 0; col < m_column; ++col ) {
                 p_twi = ui->m_tableSet_tw->item(r, col);
                 if(p_twi == nullptr) {
                    return true;
                 }
                 sl.append(p_twi->text());
              }
            }
            else if(str == "s/n") {
              sl.append("-");
              for(int col = 0; col < m_column; ++col ) {
                  p_twi = ui->m_tableSet_tw->item(r, col);
                  if(p_twi == nullptr) {
                     return true;
                  }
               sl.append(p_twi->text());
              }
            }
        m_tableValues.append(sl);
    }
    m_tableValues.shrink_to_fit();
return false;
}


