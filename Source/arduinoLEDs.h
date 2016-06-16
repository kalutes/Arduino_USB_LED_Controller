#ifndef COLORSELECT_H
#define COLORSELECT_H

#include <QWidget>
#include <QColor>
#include <QSerialPort>

namespace Ui {
class ColorSelect;
}

class ColorSelect : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color NOTIFY colorChanged)

public:
    explicit ColorSelect(QWidget *parent = 0);
    ~ColorSelect();

    QColor color() const
    {
        return m_color;
    }

signals:
    void colorChanged(QColor arg);

private slots:
    void onColorChanged();


    void on_applyButton_clicked();

private:
    Ui::ColorSelect *ui;
    QSerialPort *arduino;
    static const quint16 unoVendorID = 9025;
    static const quint16 unoProductID = 67;
    QColor m_color;
    QString portName;
    bool portAvailable;
};

#endif // COLORSELECT_H
