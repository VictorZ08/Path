#include "ui_generatoraepwidget.h"
#include "generatoraepwidget.h"
#include "report/reportaepwidget.h"

#include "sortfiles.h"
#include "random/random.h"
#include "timer/timer.h"

#include <QDir>
#include <QDebug>

static int nameSet;
static int positionFiles;

GeneratorAepWidget::GeneratorAepWidget(SystemTray* inSysTray,
                                       QWidget* inParent)
            : TimerInterface(inSysTray, inParent)
            , ui(new Ui::GeneratorAepWidget)
            , m_reportAep(new ReportAepWidget(inSysTray, this))
{
    ui->setupUi(this);

    setParentWiget(this);
    connectSlots();
    initEventFiter();
    initSpinBoxAmplituda();

    ui->m_saveSets_le->setEnabled(true);
    ui->m_loadSets_tw->setEnabled(true);

    setAcceptDrops(true);

    ui->m_loadSets_tw->setAttribute(Qt::WA_AcceptDrops, true);
    statusGeneratesFiles("color: rgb(0, 0, 0)",
                         "Статус:-");

    qDebug()<<"Create GeneratorAepWidget";
}

/**
    @brief GeneratorAepWidget::~GeneratorAepWidget()
    Деструктор
*/
GeneratorAepWidget::~GeneratorAepWidget()
{
    delete ui;
    qDebug()<<"~GeneratorAepWidget";
}

/**
    @brief GeneratorAepWidget::connectSlots
    Установка сигнал - слотов
*/
void GeneratorAepWidget::connectSlots() const
{
    connect(ui->m_clear_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_clear_pb_clicked()));

    connect(ui->m_back_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_back_pb_clicked()));

    connect(ui->m_start_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_start_pb_clicked()));

    connect(ui->m_report_pb, SIGNAL(clicked(bool)),
            this, SLOT(m_report_pb_clicked()));

    connect(ui->m_timeSet_le, SIGNAL(textChanged(const QString& )),
            this, SLOT(m_previewTime_le_changed()));

    connect(ui->m_editTime_dte, SIGNAL(dateTimeChanged(const QDateTime& )),
            this, SLOT(m_previewTime_le_changed()));

    connect(ui->m_numSets_le, SIGNAL(textChanged(const QString& )),
            this, SLOT(m_previewTime_le_changed()));

    connect(ui->m_amplituda_dsb, SIGNAL(valueChanged(double)),
            this, SLOT(m_amplituda_dsb_changed()));

    connect(m_reportAep, SIGNAL(emitBackUi()),
            this, SLOT(showForm()));

    connect(this, SIGNAL(emitPreviewTime()),
            this, SLOT(m_previewTime_le_changed()));
}

/**
    @brief GeneratorAepWidget::initEventFiter
    Установка эвент фильтра, что бы при нажатии на пробел
    (приложение сворачивается в tree), фокус не смещался на виджеты
*/
void GeneratorAepWidget::initEventFiter()
{
    ui->m_clear_pb->installEventFilter(this);
    ui->m_loadSets_tw->installEventFilter(this);
    ui->m_saveSets_le->installEventFilter(this);
    ui->m_numSets_le->installEventFilter(this);
    ui->m_timeSet_le->installEventFilter(this);
    ui->m_editTime_dte->installEventFilter(this);
    ui->m_start_pb->installEventFilter(this);
    ui->m_amplituda_dsb->installEventFilter(this);
    ui->m_back_pb->installEventFilter(this);
    ui->m_report_pb->installEventFilter(this);
}

/**
    @brief GeneratorAepWidget::initSpinBoxAmplituda
    Настройка виджета амплитуды значений для генерации
*/
void GeneratorAepWidget::initSpinBoxAmplituda()
{
    ui->m_amplituda_dsb->setDecimals(1);
    ui->m_amplituda_dsb->setRange(0.5, 5);
    ui->label_ampl_res->setText(tr("Размах: %1").arg(m_valAmplituda*2));
    ui->labelAmplituda->setText(tr("от -%1 до +%1").arg(m_valAmplituda));
    ui->m_amplituda_dsb->setSingleStep(0.5);
    ui->m_amplituda_dsb->setWrapping(true);
}

/**
    @brief GeneratorAepWidget::m_clear_pb_clicked
    Очищает форму
*/
void GeneratorAepWidget::m_clear_pb_clicked()
{
    clearWiget();

    nameSet = 0;
    positionFiles = 0;
    m_mapStr.clear();
    m_outPathFiles.clear();
    ui->m_saveSets_le->clear();
    ui->m_numSets_le->clear();
    statusGeneratesFiles("color: rgb(0, 0, 0)",
                         "Статус:-");
}

/**
    @brief GeneratorAepWidget::m_back_pb_clicked
    Возвращает в начальную форму
*/
void GeneratorAepWidget::m_back_pb_clicked()
{
    this->hide();
    emit emitBackUi();
}

/**
    @brief GeneratorAepWidget::m_previewTime_le_changed
    Выводит предварительные значения даты и времени
*/
void GeneratorAepWidget::m_previewTime_le_changed()
{
    if(ui->m_numSets_le->text() != "")
        previewTime();
}

/**
    @brief GeneratorAepWidget::m_start_pb_clicked
    Запуск перетасовки файлов, присвоения им время,
    копироваии файлов и генерации значений в файлах
*/
void GeneratorAepWidget::m_start_pb_clicked()
{
    m_outPathFiles.clear();

    Set& setsInTree = getSetsInTree();
    sortFilesToComplectAep(setsInTree);
    createSet(setsInTree);
    randValuesInFilesAep(m_outPathFiles);
    shuffleFiles(setsInTree.getSetsAep());

    QVector<QDateTime>::iterator itDT = getDateTime().begin();
    for(auto& pathFile : m_outPathFiles)
        setDateTimeFiles(pathFile, *itDT++);

    statusGeneratesFiles("color: rgb(255, 255, 255)", "Статус: Готов");

}

/**
    @brief GeneratorAepWidget::createSet
    Создает комплекты
    @param inSetInTree Комплект из трейвиджета
*/
void GeneratorAepWidget::createSet(Set& inSetInTree)
{
    QVector<pairFiFiL>::iterator itSets = inSetInTree.getSetsAep().begin();
    int numberSets = ui->m_numSets_le->text().toInt();
    for(int numSet = 0; numSet < numberSets; ++numSet) {
        createFolders();
        copyFiles(itSets->second);
        ++itSets;
        if(itSets == inSetInTree.getSetsAep().end())
            itSets = inSetInTree.getSetsAep().begin();
    }
}

/**
    @brief GeneratorAepWidget::createFolders
    Создает каталоги
*/
void GeneratorAepWidget::createFolders()
{
    QDir dir;
    QString saveSets = ui->m_saveSets_le->text();
    m_outPathFiles.append(saveSets +
                          QDir::separator() +
                          QString::number(++nameSet));
    dir.mkpath(m_outPathFiles.at(positionFiles++).absoluteFilePath());
}

/**
    @brief GeneratorAepWidget::copyFiles
    Копирует файлы из трея
    @param inPathFiles лист для сохранения путей файлов
*/
void GeneratorAepWidget::copyFiles(const QFileInfoList& inPathFiles)
{
    QString saveSets = ui->m_saveSets_le->text();
    QFileInfoList::ConstIterator it = inPathFiles.begin();
    for(; it != inPathFiles.end(); ++it) {
        m_outPathFiles.append(saveSets + QDir::separator() +
                                  QString::number(nameSet)+
                                  QDir::separator() +
                                  it->baseName() + ".txt");
        QFile::copy(it->absoluteFilePath(),
                    m_outPathFiles.at(positionFiles++).absoluteFilePath());
    }
}

/**
    @brief GeneratorAepWidget::randValuesInFilesAep
    Генерирует рандомные значения в файлах
    @param inPathFiles лист файлов
*/
void GeneratorAepWidget::randValuesInFilesAep(const QFileInfoList& inPathFiles)
{
    for(auto& pathFile : inPathFiles) {
        if(pathFile.isFile()) {
            QString buffer = read(pathFile.absoluteFilePath());
            buffer = generStrBuffer(buffer);
            write(pathFile.absoluteFilePath(), buffer);
        }
    }
}

/**
    @brief GeneratorAepWidget::read
    Читает значения из файла
    @param inPathFile читаемый файл
    @return возвращает прочитанную строку
*/
QString GeneratorAepWidget::read(const QString& inPathFile) const
{
    QString content;
    QFile file(inPathFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);

        while (!in.atEnd())
            content = in.readAll();
    }
    file. close();
    return content;
}

/**
    @brief GeneratorAepWidget::write
    Запись в файл
    @param inPathFileOut записываемый файл
    @param inContent значения записываемые в файл
*/
void GeneratorAepWidget::write(const QString& inPathFileOut, const QString& inContent)
{
    QFile file(inPathFileOut);
    if (file.open(QIODevice::WriteOnly |
                  QIODevice::Text |
                  QIODevice::Truncate)){

        QTextStream out(&file);
        out << inContent;
    }
    file. close();
}

/**
    @brief GeneratorAepWidget::generStrBuffer
    Генерация значений из буфера
    @param inBuff строка значений
    @return возвращает сгенерированную строку
*/
QString GeneratorAepWidget::generStrBuffer(const QString& inBuff)
{
    double randValues;
    QString tempBuff;
    QStringList strList = inBuff.split('\n');
    tempBuff.resize(inBuff.count());
    tempBuff.clear();
qDebug()<< inBuff;
    tempBuff += generOneStrBuffer(inBuff) + '\n';
    strList.pop_front();
    strList.removeAll("");

    for(auto& str : strList) {
        tempBuff += str.left(str.indexOf('\t', 1)) + '\t';
        for(int tab = 1; tab < 4; ++tab) {
            randValues = Random::randValues(str.section('\t', tab, tab).toDouble(),
                                            m_valAmplituda);
            tempBuff += QString::number(randValues, 'f', 2) + '\t';
        }
        tempBuff.chop(1);
        tempBuff += '\n';
    }
return tempBuff;
}

/**
    @brief GeneratorAepWidget::generOneStrBuffer
    Генерация значений первой строки файла
    @param inBuff строка значений
*/
QString GeneratorAepWidget::generOneStrBuffer(QStringView inBuff)
{
    QString tempBuffOneStr = inBuff.left(inBuff.indexOf('\n', 1)).toString();

    QMap<QString, QString>::iterator itMap = m_mapStr.find(tempBuffOneStr);
    if(!m_mapStr.contains(tempBuffOneStr)) {
        m_mapStr.insert(tempBuffOneStr,
                        QString::number(Random::randOneStringAep(tempBuffOneStr.toDouble())));
        return m_mapStr.value(tempBuffOneStr);
    }
    else {
        tempBuffOneStr = m_mapStr.value(tempBuffOneStr);
        m_mapStr.erase(itMap);
        return tempBuffOneStr;
    }
}

/**
    @brief GeneratorAepWidget::m_report_pb_clicked
    Выводит отчет сгенерированных файлов
*/
void GeneratorAepWidget::m_report_pb_clicked()
{
   this->hide();
   m_reportAep->show();
   QFileInfoList inputSets;
   Set& sets = getSetsInTree();
   for(auto& set : sets.getSetsAep()) {
        inputSets.append(set.first);
        for(auto i = 0; i < set.second.count(); ++i)
            inputSets.append(set.second.at(i));
   }
   m_reportAep->openInfoOut(m_outPathFiles, inputSets);
   return;
}

/**
    @brief GeneratorAepWidget::m_amplituda_dsb_changed
    Установка значения амплитуды генерируемых значений
*/
void GeneratorAepWidget::m_amplituda_dsb_changed()
{
    m_valAmplituda = ui->m_amplituda_dsb->value();
    ui->label_ampl_res->setText(tr("Размах: %1").arg(m_valAmplituda*2));
    ui->labelAmplituda->setText(tr("от -%1 до +%1").arg(m_valAmplituda));
}

/**
    @brief GeneratorAepWidget::showForm
    Вывод формы (требуется для ReportAepWidget)
*/
void GeneratorAepWidget::showForm()
{
    this->show();
}
