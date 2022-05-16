#ifndef PASSWIDGET_H
#define PASSWIDGET_H

#include <QWidget>

namespace Ui {
class PassWidget;
}

class PassWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PassWidget(QWidget* parent = nullptr);
    ~PassWidget();

    void setPointWidget(QWidget* ptrWidget);

protected:
    void connectSlots() const;

private slots:
    void m_passEnter_le_retPres();

private:
    Ui::PassWidget* ui = nullptr;
    QWidget* m_ptrForm = nullptr;

    const QString m_password;
};

#endif // PASSWIDGET_H
