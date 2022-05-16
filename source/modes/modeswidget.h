#ifndef MODESWIDGET_H
#define MODESWIDGET_H

#include <QWidget>

namespace Ui {
class ModesWidget;
}
class GeneratorAepWidget;
class TimerAepWidget;
class GeneratorPemiWidget;
class TimerPemiWidget;
class SearchFilesWidget;
class SystemTray;

class ModesWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit ModesWidget(QWidget* parent = nullptr);
    ~ModesWidget();

protected:
    void connectSlots() const;
    void initEventFiter();
    void createSystemTray();
    void createWidgets();

    virtual bool eventFilter(QObject* inTarget,
                             QEvent* inEvent) override;

public slots:
    void m_generAEP_pb_clicked();
    void m_timerAEP_pb_clicked();
    void m_generPEMI_pb_clicked();
    void m_timerPEMI_pb_clicked();
    void m_search_pb_clicked();
    void showModes();

private:
    Ui::ModesWidget*        ui                      = nullptr;
    GeneratorAepWidget*     m_generatorAepWidget    = nullptr;
    TimerAepWidget*         m_timerAepWidget        = nullptr;
    GeneratorPemiWidget*    m_generatorPemiWidget   = nullptr;
    TimerPemiWidget*        m_timerPemiWidget       = nullptr;
    SearchFilesWidget*      m_searchFileWidget      = nullptr;
    SystemTray*             m_sysTray               = nullptr;
};

#endif // MODESWIDGET_H
