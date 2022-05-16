#include "style.h"

#include <QStyleFactory>
#include <QPainter>
#include <QDebug>

Style::Style() :
    QProxyStyle(QStyleFactory::create("windows"))
{
    setObjectName("NorwegianWood");
    qDebug()<<"Create Style";
}

//-----------------------------
Style::~Style()
{
    qDebug()<<"~Style";
}

//-----------------------------
QPalette Style::standardPalette() const {
    if (!m_standardPalette.isBrushSet(QPalette::Disabled, QPalette::Mid)) {
        QColor brown(212, 140, 95);
        QColor beige(236, 182, 120);//Внутри виджета
        QColor slightlyOpaqueBlack(0, 0, 0, 63);

        QImage backgroundImage(":/image/woodbackground.png");
        QImage buttonImage(":/image/woodbutton.png");
        QImage midImage = buttonImage.convertToFormat(QImage::Format_RGB32);

        QPainter painter;
        painter.begin(&midImage);
        painter.setPen(Qt::NoPen);
        painter.fillRect(midImage.rect(), slightlyOpaqueBlack);
        painter.end();

        QPalette palette(brown);

        palette.setBrush(QPalette::BrightText, Qt::white);
        palette.setBrush(QPalette::ButtonText, Qt::white);
        palette.setBrush(QPalette::Base, beige);
        palette.setBrush(QPalette::Highlight, Qt::darkGreen);

        setTexture(palette, QPalette::Button, buttonImage);
        setTexture(palette, QPalette::Mid, midImage);
        setTexture(palette, QPalette::Window, backgroundImage);

        QBrush brush = palette.window();
        brush.setColor(brush.color().darker());

        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush);

        m_standardPalette = palette;
    }
return m_standardPalette;
}

//-------------------------------
void Style::setTexture(QPalette& palette,
                       QPalette::ColorRole role,
                       const QImage& image) {
    for (int i = 0; i < QPalette::NColorGroups; ++i) {
        QBrush brush(image);
        brush.setColor(palette.brush(QPalette::ColorGroup(i), role).color());
        palette.setBrush(QPalette::ColorGroup(i), role, brush);
    }
}
