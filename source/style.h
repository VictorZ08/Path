#ifndef STYLE_H
#define STYLE_H

#include <QProxyStyle>

class Style : public QProxyStyle
{
    Q_OBJECT
public:
    Style();
    ~Style();
    QPalette standardPalette() const override;

private:
    static void setTexture(QPalette& palette,
                           QPalette::ColorRole role,
                           const QImage& image);

private:
    mutable QPalette m_standardPalette;
};

#endif // STYLE_H
