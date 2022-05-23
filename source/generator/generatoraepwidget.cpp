#include "ui_generatoraepwidget.h"
#include "generatoraepwidget.h"
#include "report/reportaepwidget.h"

#include "sortfiles.h"
#include "random/random.h"
#include "timer/timer.h"

#include <QDir>
#include <QDebug>

static quint64 nameSet;
static quint64 positionFiles;

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
    Set& sets = getSetsInTree();
    for(auto& set: sets.getSetsAep())
        sortFilesToComplectAep(set);

    QVector<pairFiFiL>::iterator itSets = sets.getSetsAep().begin();//Добавить итератор
    quint64 numberSets = ui->m_numSets_le->text().toInt();
    for(quint64 set = 0; set < numberSets; ++set)
        createSet(itSets, sets);

    for(auto& pathFiles: m_outPathFiles)
        generatorsValuesAep(pathFiles);

    shuffleFiles(sets.getSetsAep());
    QVector<QDateTime>::iterator itDateTime = getDateTime().begin();
    for(auto& pathFile: m_outPathFiles)
        Timer::setDateTimeFile(pathFile, *itDateTime++);

    statusGeneratesFiles("color: rgb(255, 255, 255)", "Статус: Готов");

}

/**
    @brief GeneratorAepWidget::createSet
    Создает файлы
    @param it Итератор начала списка копируемых файлов
    @param inSet Комплект (требуется для установки it)
*/
void GeneratorAepWidget::createSet(QVector<pairFiFiL>::iterator it, Set& inSet)
{
    createFolders();
    copyFiles(it->second);
    ++it;
    if(it == inSet.getSetsAep().end())
        it = inSet.getSetsAep().begin();
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
void GeneratorAepWidget::copyFiles(QFileInfoList& inPathFiles)
{
    QString saveSets = ui->m_saveSets_le->text();
    QFileInfoList::ConstIterator it;
    for(it = inPathFiles.begin(); it != inPathFiles.end(); ++it) {
        m_outPathFiles.append(saveSets + QDir::separator() +
                                  QString::number(nameSet)+
                                  QDir::separator() +
                                  it->baseName() + ".txt");
        QFile::copy(it->absoluteFilePath(),
                    m_outPathFiles.at(positionFiles++).absoluteFilePath());
    }
}

/**
    @brief GeneratorAepWidget::generatorsValuesAep
    Генерирует рандомные значения в файлах
    @param inPathFiles лист файлов
*/
void GeneratorAepWidget::generatorsValuesAep(QFileInfo& inPathFiles)
{
    if(inPathFiles.isFile()) {
        QString buffer = read(inPathFiles.absoluteFilePath());
        genGetOneStr(buffer);
        buffer = generStrBuffer(std::move(buffer));
        write(inPathFiles.absoluteFilePath(), buffer);
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
void GeneratorAepWidget::write(const QString& inPathFileOut, QString& inContent)
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
QString GeneratorAepWidget::generStrBuffer(const QString&& inBuff)
{
    double values;
    QString strBuff;
    QStringList  strList = inBuff.split("\n");
    strBuff.resize(inBuff.count());
    strBuff.clear();

    strBuff += strList.at(0) + '\n';
    strList.pop_front();
    strList.removeAll("");

    for(auto&& str : strList) {
        strBuff += str.left(str.indexOf('\t', 1)) + '\t';
        for(int i = 1; i < 4; ++i) {
            values = Random::randValues(str.section('\t', i, i).toDouble(),
                                        m_valAmplituda);
            strBuff += QString::number(values, 'f', 2) + '\t';
        }
        strBuff.chop(1);
        strBuff += '\n';
    }
return strBuff;
}

/**
    @brief GeneratorAepWidget::genGetOneStr
    Генерация значений первой строки файла
    @param inBuff строка значений
*/
void GeneratorAepWidget::genGetOneStr(QString& inBuff)
{
    QString str;
    while(inBuff.at(0) != '\n') {
        str += inBuff.at(0);
        inBuff.remove(0,1);
    }
    QMap<QString, QString>::iterator it_map = m_mapStr.find(str);
    if(!m_mapStr.contains(str)) {
        m_mapStr.insert(str, QString::number(Random::randOneStringAep(str.toDouble())));
        inBuff.prepend(m_mapStr.value(str));
    }
        else {
            inBuff.prepend(m_mapStr.value(str));
            m_mapStr.erase(it_map);
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
   for(auto &set: sets.getSetsAep()) {
        inputSets.append(set.first);
        for(int i = 0; i < set.second.count(); ++i)
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
