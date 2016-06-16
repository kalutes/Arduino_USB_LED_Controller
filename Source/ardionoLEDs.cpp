#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QPalette>
#include <QtWidgets>
#include "arduinoLEDs.h"
#include "ui_arduinoLEDs.h"

ColorSelect::ColorSelect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorSelect)
{
    ui->setupUi(this);
    portAvailable = false;
    portName = "";
    connect(ui->redSlider, SIGNAL(valueChanged(int)), SLOT(onColorChanged()));
    connect(ui->greenSlider, SIGNAL(valueChanged(int)), SLOT(onColorChanged()));
    connect(ui->blueSlider, SIGNAL(valueChanged(int)), SLOT(onColorChanged()));
    onColorChanged();
    arduino = new QSerialPort;

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
        {
            if(serialPortInfo.vendorIdentifier() == unoVendorID)
            {
                if(serialPortInfo.productIdentifier() == unoProductID)
                {
                    portName = serialPortInfo.portName();
                    portAvailable = true;
                }
            }
        }
    }
    if(portAvailable)
    {
        arduino->setPortName(portName);
        arduino->open(QSerialPort::WriteOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
    }
    else
    {
        QMessageBox::warning(this, "Port Error", "Port not found!");
    }
}

ColorSelect::~ColorSelect()
{
    if(arduino->isOpen())
    {
        arduino->close();
    }
    delete ui;
}

void ColorSelect::onColorChanged()
{
    m_color.setRgb(ui->redSlider->value(), ui->greenSlider->value(), ui->blueSlider->value());
    QPalette pal = ui->displayWidget->palette();
    pal.setColor(QPalette::Window, m_color);
    ui->displayWidget->setPalette(pal);
    emit colorChanged(m_color);
}

void ColorSelect::on_applyButton_clicked()
{
    QString command;
    int checkSum = 0;
    command += "*";
    command += QString::number(ui->modeBox->currentIndex());
    checkSum += ui->modeBox->currentIndex();
    command += " ";
    command += QString::number(ui->delaySpinner->value());
    checkSum += ui->delaySpinner->value();
    command += " ";
    command += QString::number(ui->redSpinBox->value());
    checkSum += ui->redSpinBox->value();
    command += " ";
    command += QString::number(ui->greenSpinBox->value());
    checkSum += ui->greenSpinBox->value();
    command += " ";
    command += QString::number(ui->blueSpinBox->value());
    checkSum += ui->blueSpinBox->value();
    command += " ";
    command += QString::number(checkSum);
    command += "!";
    if(arduino->isWritable())
    {
        arduino->write(command.toStdString().c_str());
    }
}
