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


