#include <QApplication>

#include "style.h"
#include "modes/modeswidget.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(path);
    QApplication::setStyle(new Style);
    QApplication::setPalette(QApplication::style()->standardPalette());

    QApplication a(argc, argv);

    std::unique_ptr<ModesWidget> mw{ new ModesWidget};

    return a.exec();
}

/* 1.Выполнить проверку файлов и комплектов на drag&drop
 * 2.SetAEP и SetPEMI выполнить в шаблоне
 * либо в одном классе перегруженным методом
 * 3.TimerAepWidget::previewDateTime() - generatorTimeAep != generatorTimePemi
 * на выходе одних и тех же условий, generatorTimeAep всегда имеет одно
 * значение(ИСПРАВИТЬ!!!)
 * 4.GeneratorAepWidget::slotCountComplect() - если сначала в форме поменять
 * m_sizeComplect (ERROR!!!)
 * 5.Организовать семантику перемещения
 * 6.Выполнить ограничения полей на ввод, так же на 0
 * 7.generatorTimeAep - слишком большой метод
 * 8.GeneratorAepWidget::createFolders доделать переменные в методе
 * 9.previewDateTime() - доработать проверки
 * 10.pushButtonReport_clicked() - алгоритм можно вынести в pushButtonStart,
 * оставив при этом openInfoOut(....), тогда вычисления будут 1 раз
 * 11.Ослабить зависимости хедеров путем реализации опережающей ссылки и
 * вынос зависимых хедеров в файл реализации классов
 * 12.Не работает вечернее время
 * 13.TableSetsWidget::saveTableData, TreeItem::itemsFolders,
 * TreeItem::itemsFiles - устранить утечку памяти
 * 14.Изменить алгоритм обновления БД, т.е. находим все названия заказов и
 * позиции в них. Далее сравниваем пути с БД, пути которые совпадают
 * удаляем, остальные заносим в БД.
 * 15.GeneratorPemiWidget::editingTitul(...) лишнее копирование, доработать
 * алгоритм.
 * 16.SortFilesPemi::shuffleExelFiles пересмотреть аргументы под будующий
 * шаблон, придётся удалить одну из.
 * 17.TableSetsWidget при нажатии первым + или -, рушится.
*/

