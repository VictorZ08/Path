#include "ui_generatorpemiwidget.h"
#include "generator/generatorpemiwidget.h"
#include "generator/table/tablesetswidget.h"
#include "verification/loggerwidget.h"
#include "random/random.h"
#include "qrandom.h"
#include "sortfiles.h"
#include "timer/timer.h"

#include <QMessageBox>
#include <QDir>
#include "QDebug"

GeneratorPemiWidget::GeneratorPemiWidget(SystemTray* inSysTray,
                                         QWidget* inParent)
                : TimerInterface(inSysTray, inParent)
                , ui(new Ui::GeneratorPemiWidget)
                , m_tableSetsWidget(new TableSetsWidget(inSysTray, this))
                , m_logger(new LoggerWidget(inSysTray, this))
{
    ui->setupUi(this);

    initDilog();
    setParentWiget(this);
    connectSlots();
    initEventFiter();
    initSpinBoxAmplituda();

    ui->m_saveSets_le->setEnabled(true);
    ui->m_loadSets_tw->setEnabled(true);

    ui->m_numSets_le->setReadOnly(true);
    setStyleSheet("QLineEdit[readOnly=\"true\"] {""color: #808080;"
                  "background-color: #F0F0F0;""border: 1px solid #B0B0B0;"
                  "border-radius: 2px;}");
    //ui->m_numSets_le->setText("0");
    //ui->m_status_prb->setValue(0);

    setAcceptDrops(true);

    ui->m_loadSets_tw->setAttribute(Qt::WA_AcceptDrops, true);
    qDebug()<<"Create GeneratorPemiWidget";
}

GeneratorPemiWidget::~GeneratorPemiWidget()
{
    delete ui;
    qDebug()<<"~GeneratorPemiWidget";
}

/**
    @brief GeneratorPemiWidget::initDilog
    Инициализация диалогового окна
*/
void GeneratorPemiWidget::initDilog()
{
    QStringList listAntenne = {"", "АИ 5-0 (2161) до 1 ГГц", "АИ 5-0 (2162) до 1 ГГц",
                               "АИ 5-0 (2165) до 1 ГГц", "НБА-02 (21002) до 1 ГГц",
                               "НБА-02 (21003) до 1 ГГц", "USLP 9143 #686 свыше 1 ГГц",
                               "USLP 9143 #543 свыше 1 ГГц", "ЛПА-2 свыше 1 ГГц",
                               "ВВНА 9120 D (01554) свыше 1 ГГц", "ВВНА 9120 D (01555) свыше 1 ГГц",
                               "EMCO 6511", "токосьемник ОСА-А", "токосьемник EZ-17"};

    QStringList listWorkers = {"", "-", "Далевич", "Гришкин", "Зайцев", "Сидоров",
                               "Привалов", "Заренок", "Осипенко", "Родионов",
                               "Арутюнян"};

    QStringList listWorkplace = {"", "Лаборатория", "БЭК-1", "БЭК-2",
                                 "ИП-2", "ИП-3", "АРТМ"};

    ui->m_boxInjener_cb->addItems(listWorkers);
    ui->m_boxTehnic_cb->addItems(listWorkers);
    ui->m_boxAntenneLow1GGz_cb->addItems(listAntenne);
    ui->m_boxAntenneHeigh1GGz_cb->addItems(listAntenne);
    ui->m_boxWorkplace_cb->addItems(listWorkplace);
}

/**
    @brief GeneratorPemiWidget::connectSlots
    Установка сигнал - слотов
*/
void GeneratorPemiWidget::connectSlots() const
{
    connect(ui->m_clear_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_clear_pb_clicked()));

    connect(ui->m_back_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_back_pb_clicked()));

    connect(ui->m_start_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_start_pb_clicked()));

    connect(ui->m_createTable_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_createTable_pb_clicked()));

    connect(ui->m_timeSet_le, SIGNAL(textChanged(const QString&)),
            this, SLOT(m_previewTime_le_textChanged()));

    connect(ui->m_editTime_dte, SIGNAL(dateTimeChanged(QDateTime)),
            this, SLOT(m_previewTime_le_textChanged()));

    connect(ui->m_boxInjener_cb, SIGNAL(activated(int)),
            this, SLOT(m_injener_le_textChanged()));

    connect(ui->m_boxTehnic_cb, SIGNAL(activated(int)),
            this, SLOT(m_tehnic_le_textChanged()));

    connect(ui->m_boxAntenneLow1GGz_cb, SIGNAL(activated(int)),
            this, SLOT(m_antenneLow1GGz_le_textChanged()));

    connect(ui->m_boxAntenneHeigh1GGz_cb, SIGNAL(activated(int)),
            this, SLOT(m_antenneHeigh1GGz_le_textChanged()));

    connect(ui->m_boxWorkplace_cb, SIGNAL(activated(int)),
            this, SLOT(m_workplace_le_textChanged()));

    connect(ui->m_amplituda_sb, SIGNAL(valueChanged(double)),
            this, SLOT(m_amplituda_sb_textChanged()));

    connect(m_tableSetsWidget, SIGNAL(emitBackUi()),
            this, SLOT(showForm()));

    connect(ui->m_fixedTime_ckb, SIGNAL(stateChanged(int)),
            this, SLOT(m_previewTime_le_textChanged()));

    connect(ui->m_startCheckData_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_startCheckData_pb_clicked()));

    connect(ui->m_reportCheck_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_reportCheck_pb_clicked()));

    connect(this, SIGNAL(emitPreviewTime()),
            this, SLOT(m_previewTime_le_textChanged()));

    connect(this, SIGNAL(emitStatus_prb()), this,
            SLOT(m_progress_prb_tempStart()), Qt::DirectConnection);

    connect(m_logger, SIGNAL(emitBackUi()),
            this, SLOT(showForm()));
}

/**
    @brief GeneratorPemiWidget::initEventFiter
    Установка эвент фильтра, что бы при нажатии на пробел
    (приложение сворачивается в tree), фокус не смещался на виджеты
*/
void GeneratorPemiWidget::initEventFiter()
{
    ui->m_clear_pb->installEventFilter(this);
    ui->m_back_pb->installEventFilter(this);
    ui->m_createTable_pb->installEventFilter(this);
    ui->m_start_pb->installEventFilter(this);

    ui->m_loadSets_tw->installEventFilter(this);
    ui->m_editTime_dte->installEventFilter(this);

    ui->m_saveSets_le->installEventFilter(this);
    ui->m_timeSet_le->installEventFilter(this);
    ui->m_previewTime_le->installEventFilter(this);
    ui->m_injener_le->installEventFilter(this);
    ui->m_tehnic_le->installEventFilter(this);
    ui->m_antenneLow1GGz_le->installEventFilter(this);
    ui->m_antenneHeigh1GGz_le->installEventFilter(this);
    ui->m_workplace_le->installEventFilter(this);

    ui->m_boxInjener_cb->installEventFilter(this);
    ui->m_boxTehnic_cb->installEventFilter(this);
    ui->m_boxAntenneLow1GGz_cb->installEventFilter(this);
    ui->m_boxAntenneHeigh1GGz_cb->installEventFilter(this);
    ui->m_boxWorkplace_cb->installEventFilter(this);

    ui->m_amplituda_sb->installEventFilter(this);
    ui->m_multiplisity_ckb->installEventFilter(this);
}

/**
    @brief GeneratorPemiWidget::initSpinBoxAmplituda
    Инициализация виджета sb
*/
void GeneratorPemiWidget::initSpinBoxAmplituda()
{
    ui->m_amplituda_sb->setDecimals(1);
    ui->m_amplituda_sb->setRange(0.5, 5);
    ui->label_ampl_res->setText(tr("Размах: %1").arg(m_valAmplituda*2));
    ui->labelAmplituda->setText(tr("от -%1 до +%1").arg(m_valAmplituda));
    ui->m_amplituda_sb->setSingleStep(0.5);
    ui->m_amplituda_sb->setWrapping(true);
}

/**
    @brief GeneratorPemiWidget::m_clear_pb_clicked
    Очищает форму
*/
void GeneratorPemiWidget::m_clear_pb_clicked()
{
    clearWiget();

    ui->m_loadSets_tw->clear();
    ui->m_saveSets_le->clear();
    m_outExelFiles.clear();
    ui->m_saveSets_le->clear();
    ui->m_numSets_le->clear();
    ui->m_timeSet_le->setText(QString::number(kTimeSetAep));
    ui->m_status_prb->setValue(0);
}

/**
    @brief GeneratorPemiWidget::m_previewTime_le_textChanged
    Выводит предварительные значения даты и времени
*/
void GeneratorPemiWidget::m_previewTime_le_textChanged()
{
    if(ui->m_numSets_le->text() != "0")
        previewTime();
}

/**
    @brief GeneratorPemiWidget::m_startCheckData_pb_clicked
    Проверка данных в tree на ошибки
*/
void GeneratorPemiWidget::m_startCheckData_pb_clicked()
{
    startCheckData();
}

/**
    @brief GeneratorPemiWidget::m_progress_prb_tempStart
    Увеличивает состояние прогрессбар
*/
void GeneratorPemiWidget::m_progress_prb_tempStart()
{
    progressTempStart();
}

/**
    @brief GeneratorPemiWidget::m_reportCheck_pb_clicked
    Вывод отчета
*/
void GeneratorPemiWidget::m_reportCheck_pb_clicked()
{
    reportCheck(m_logger);
}

/**
    @brief GeneratorPemiWidget::searchTitulInExel
    Поиск титульной таблицы в exel файле
    @param inCoord координаты поиска таблицы
    @param nameCell поиск таблицы по названию ячейки
    @param xlsxFile exel файл с таблицей
    @return координаты найденной таблицы
*/
QPoint GeneratorPemiWidget::searchTitulInExel(const sCoordSerchTable& inCoord,
                                              const QString& nameCell,
                                              const QXlsx::Document& xlsxFile)
{
    QXlsx::Cell* cell;
    QString val;
    for(int row = inCoord.rowStart; row < inCoord.rowEnd; ++row) {
        for(int col = inCoord.columnStart; col < inCoord.columnEnd; ++col) {
            cell = xlsxFile.cellAt(row, col);
            if(cell == nullptr)
               continue;
            val.append(cell->value().toString());
            if(val.contains(nameCell, Qt::CaseInsensitive)) {
               QPoint point(row, col);
               return point;
            }
         }
     }
QPoint point(0, 0);
return point;
}

/**
    @brief GeneratorPemiWidget::m_createTable_pb_clicked
    Открывает форму для занесения данных по комплектам и
    сворачивает старую
*/
void GeneratorPemiWidget::m_createTable_pb_clicked()
{
    this->hide();
    m_tableSetsWidget->show();
}

/**
    @brief GeneratorPemiWidget::m_injener_le_textChanged
    Заносит данные в форму
*/
void GeneratorPemiWidget::m_injener_le_textChanged() {
    ui->m_injener_le->setText(ui->m_boxInjener_cb->currentText());
}

//-------------------------------
void GeneratorPemiWidget::m_tehnic_le_textChanged()
{
    ui->m_tehnic_le->setText(ui->m_boxTehnic_cb->currentText());
}
//-------------------------------
void GeneratorPemiWidget::m_antenneLow1GGz_le_textChanged()
{
    ui->m_antenneLow1GGz_le->setText(ui->m_boxAntenneLow1GGz_cb->currentText());
}
//-------------------------------
void GeneratorPemiWidget::m_antenneHeigh1GGz_le_textChanged()
{
    ui->m_antenneHeigh1GGz_le->setText(ui->m_boxAntenneHeigh1GGz_cb->currentText());
}
//-------------------------------
void GeneratorPemiWidget::m_workplace_le_textChanged()
{
    ui->m_workplace_le->setText(ui->m_boxWorkplace_cb->currentText());
}

/**
    @brief GeneratorPemiWidget::m_amplituda_sb_textChanged
    Установка значения от sp
*/
void GeneratorPemiWidget::m_amplituda_sb_textChanged()
{
    m_valAmplituda = ui->m_amplituda_sb->value();
    ui->label_ampl_res->setText(tr("Размах: %1").arg(m_valAmplituda*2));
    ui->labelAmplituda->setText(tr("от -%1 до +%1").arg(m_valAmplituda));
}

/**
    @brief GeneratorPemiWidget::m_start_pb_clicked
    Запуск генерации файлов
*/
void GeneratorPemiWidget::m_start_pb_clicked()
{
    initDataTitul();
    m_outExelFiles.clear();
    QString saveSets = ui->m_saveSets_le->text();

    /************************************/
    //Копируем файлы exel и присваиваем им имена(serialNumSet)
    QFileInfoList sets = getSetsInTree().getSetsPemi();
    QFileInfoList::iterator it = sets.begin();
    QString serialNumSet;
    QVector<QStringList> dataSets = m_tableSetsWidget->getTableValues();
    ui->m_status_prb->setMaximum(dataSets.count() + m_outExelFiles.count()*2);
    for(auto& dataSet : dataSets) {
        if(dataSet.at(0) == "-")
            serialNumSet = dataSet.at(1);
        else
            serialNumSet = dataSet.at(0);

        copyFiles(*it, saveSets, serialNumSet);
        ++it;
        if(it == sets.end()) {
            it = sets.begin();
        }
        emit emitStatus_prb();
    }
    /************************************/
    /*Редактируем файлы exel*/
    QPoint posCell;
    QStringList exelSeetsNames;
    QVector<QStringList>::iterator itTableValues = m_tableSetsWidget->getTableValues().begin();//можно взять вначале метода
    sCoordSerchTable coord;
    for(auto& exelFile : m_outExelFiles) {
        QXlsx::Document xlsxOpenFile(exelFile.absoluteFilePath());
        posCell = searchTitulInExel(coord,
                                    "Комплект",
                                    xlsxOpenFile);
        if(posCell.isNull()) {
            messageError();
            return;
        }
        editTitul(posCell, xlsxOpenFile, *itTableValues);
        exelSeetsNames = xlsxOpenFile.sheetNames();
        for(auto& exelSeet : exelSeetsNames) {
            editSheet(xlsxOpenFile, exelSeet);
        }
        xlsxOpenFile.save();
        ++itTableValues;
        emit emitStatus_prb();
    }
    /************************************/
    //Присваиваем двту и время
    QVector<QDateTime>::iterator itDT = getDateTime().begin();
    shuffleFiles(m_outExelFiles);
    for(auto& exelFile : m_outExelFiles) {
        setDateTimeFiles(exelFile, *itDT++);
        emit emitStatus_prb();
    }
    //Создаем файл и записываем в него s/n комплектов
    writeSNsetsInTxtFile(saveSets, "s-n", dataSets);
}

/**
    @brief GeneratorPemiWidget::initDataTitul
    Инициализация контейнера данными для титульной рамки
*/
void GeneratorPemiWidget::initDataTitul()
{
    m_prDataTitul.first.clear();
    m_prDataTitul.second.clear();
    m_prDataTitul.first.append("Комплект");  m_prDataTitul.second.append("-");
    m_prDataTitul.first.append("s/n");       m_prDataTitul.second.append("-");
    m_prDataTitul.first.append("ЗР/ЗНП");    m_prDataTitul.second.append("-");
    m_prDataTitul.first.append("Инженер");   m_prDataTitul.second.append(ui->m_injener_le->text());
    m_prDataTitul.first.append("Техник");    m_prDataTitul.second.append(ui->m_tehnic_le->text());
    m_prDataTitul.first.append("Антенна");   m_prDataTitul.second.append(ui->m_antenneLow1GGz_le->text());
    m_prDataTitul.first.append("Антенна");   m_prDataTitul.second.append(ui->m_antenneHeigh1GGz_le->text());
    m_prDataTitul.first.append("Площадка");  m_prDataTitul.second.append(ui->m_workplace_le->text());
}

/**
    @brief GeneratorPemiWidget::copyFiles
    Копирование excel файлов
    @param inExcelFile Копируемый excel файл из treeWidget
    @param outPathFile Путь куда копируем
    @param nameExcelFile Название скопированного excel файла
*/
void GeneratorPemiWidget::copyFiles(const QFileInfo& inExcelFile,
                                    const QString& outPathFile,
                                    const QString& nameExcelFile)
{
    QString saveSet (outPathFile + QDir::separator() + nameExcelFile + ".xlsx");
    QFile::copy(inExcelFile.absoluteFilePath(), saveSet);
    m_outExelFiles.append(saveSet);
}

/**
    @brief GeneratorPemiWidget::editTitul
    Редактирование титульной рамки
    @param inPositionTable Координаты начала титульной рамки
    @param inExcelFile Файл exel для редактирования
    @param inData Данные по каждому комплекту
*/
void GeneratorPemiWidget::editTitul(const QPoint& inPositionTable,
                                    QXlsx::Document& inExcelFile,
                                    const QStringList& inData)
{
    QPoint posCell = inPositionTable;
    int row = m_prDataTitul.first.count();

    for(int i = 0; i < 3; ++i) {
        m_prDataTitul.second.replace(i, inData.at(i));
    }
    for(int r = 0; r < row; ++r) {
        inExcelFile.write(posCell.rx(), posCell.ry(), m_prDataTitul.first.at(r));
        inExcelFile.write(posCell.rx(), posCell.ry()+1, m_prDataTitul.second.at(r));
        ++posCell.rx();
    }
}

/**
    @brief GeneratorPemiWidget::editSheet
    Редактирование листов
    @param inExcelFile Файл exel для редактирования
    @param inExcelSeetNames Данные по каждому комплекту
*/
void GeneratorPemiWidget::editSheet(QXlsx::Document& inExcelFile,
                                    QString& inExcelSeetNames)
{
    const uint kOneCol = 1;
    if(inExcelSeetNames == "Описание")
        return;

    uint row = 1;
    inExcelFile.selectSheet(inExcelSeetNames);
    if(!inExcelFile.read(row,kOneCol).isValid()) {
       inExcelFile.write(row, kOneCol, "с метра не обнаружено");
       return;
    }
    while(inExcelFile.read(row,kOneCol).isValid()) {
        uint col = 2;
        QVariant varValues = inExcelFile.read(row, col);
        if(varValues.isValid()) {
            /*Генерим с кратностью 0.05*/
            if(ui->m_multiplisity_ckb->isChecked()) {
                varValues = Random::randValues(varValues.toDouble(), m_valAmplituda);
                varValues = varValues.toDouble() - remainder(varValues.toDouble(), 0.1)+0.05;
                inExcelFile.write(row, col++, varValues);
                varValues = inExcelFile.read(row, col);
                varValues = Random::randValues(varValues.toDouble(), m_valAmplituda);
                varValues = varValues.toDouble() - remainder(varValues.toDouble(), 0.1)+0.05;
                inExcelFile.write(row, col, varValues);
            }
            else {
                varValues = Random::randValues(varValues.toDouble(), m_valAmplituda);
                varValues = varValues.toDouble() - remainder(varValues.toDouble(), 0.01);
                inExcelFile.write(row, col++, varValues);
                varValues = inExcelFile.read(row, col);
                varValues = Random::randValues(varValues.toDouble(), m_valAmplituda);
                varValues = varValues.toDouble() - remainder(varValues.toDouble(), 0.01);
                inExcelFile.write(row, col, varValues);
            }
        }
        ++row;
    }
}

/**
    @brief GeneratorPemiWidget::writeSNsetsInTxtFile
    Запись s/n комплектов в .txt файл
    @param outPath Путь куда копируем
    @param inNameFile Название .txt файла
    @param dataSets Данные всех комплектов (комплект, s/n, znp)
*/
void GeneratorPemiWidget::writeSNsetsInTxtFile(const QString& outPath,
                                               const QString& inNameFile,
                                               const QVector<QStringList>& dataSets)
{
    quint64 twoColumn = 1;
    QString saveTxtFile (outPath + QDir::separator() + inNameFile + ".txt");
    QFile file( saveTxtFile );
    if (file.open(QIODevice::WriteOnly |
                  QIODevice::Text |
                  QIODevice::Truncate)){

        QTextStream out(&file);
        for(auto& numerSet : dataSets)
            out << numerSet.at(twoColumn) << "\n";
    }
    file. close();
}

/**
    @brief GeneratorPemiWidget::messageError
    Выводит сообщение в случае не нахождении
    титульной рамки
*/
void GeneratorPemiWidget::messageError()
{
    QMessageBox::critical(this, "Ошибка","Таблица не попадает\n"
                                         "в диапазон ячеек\n"
                                         "      F5 - J5 \n"
                                         "      ........\n"
                                         "      F30- J30\n"
                                         "", QMessageBox::Ok);
}

/**
    @brief GeneratorPemiWidget::m_back_pb_clicked
    Возвращает в начальную форму
*/
void GeneratorPemiWidget::m_back_pb_clicked()
{
    this->hide();
    emit emitBackUi();
}

/**
    @brief GeneratorPemiWidget::showForm
    Раскрывает форму
*/
void GeneratorPemiWidget::showForm()
{
    this->show();
    int row = m_tableSetsWidget->getRow();
    ui->m_numSets_le->setText(QString::number(row));
    m_previewTime_le_textChanged();
}
