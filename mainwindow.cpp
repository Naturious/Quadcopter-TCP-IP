#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QException>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/maker_lab.png"));

    ui->arm_button->setEnabled(false);
    ui->throttle_slider->setEnabled(false);
    ui->pitch_slider->setEnabled(false);
    ui->roll_slider->setEnabled(false);
    ui->arm_button->setStyleSheet("background-color: white");

    ui->tcp_ip_button->setStyleSheet("background-color:green");

    ui->textBrowser->setReadOnly(true);

    ui->pitch_slider->setValue(50);
    ui->roll_slider->setValue(50);

    timer = new QTimer(this);
    arm_timer = new QTimer(this);

    connected_sound = new QMediaPlayer(this);
    disconnected_sound = new QMediaPlayer(this);

    connected_sound->setMedia(QUrl::fromLocalFile("/home/naturious/codes/qt/Quadcopter_TCP/connected.wav"));
    disconnected_sound->setMedia(QUrl::fromLocalFile("/home/naturious/codes/qt/Quadcopter_TCP/disconnected.wav"));

    connected_sound->setVolume(25);
    disconnected_sound->setVolume(25);

    QObject::connect(&mServer,SIGNAL(newConnectionSignal()),this,SLOT(connected()));
    QObject::connect(&mServer,SIGNAL(endedConnectionSignal()),this,SLOT(disconnected()));
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(send_point()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::log(const QString &str)
{
    ui->textBrowser->append("["+ QTime::currentTime().toString()+"] - "+str);
}

void MainWindow::on_tcp_ip_button_clicked()
{
    if(!mServer.listening())
    {
        if(mServer.start())
        {
            log(QString("Server started"));
            ui->tcp_ip_button->setStyleSheet("background-color:red");
            ui->tcp_ip_button->setText("Close TCP/IP server");
            ui->tcp_ip_button->setWhatsThis("Stops listening for connections");
            ui->tcp_ip_button->setStatusTip("Stops listening for connections");
        }
        else
        {
            log(QString("Server couldn't start"));
        }
    }
    else
    {
        if(mServer.isConnected())
            mServer.sendData(120);//SEND OFF?
        mServer.close();
        log("Server closed");
        ui->tcp_ip_button->setStyleSheet("background-color:green");
        ui->tcp_ip_button->setText("Start TCP/IP server");
        ui->tcp_ip_button->setWhatsThis("Starts listening for connections");
        ui->tcp_ip_button->setStatusTip("Starts listening for connections");
    }
}

void MainWindow::connected()
{
    log(QString("Connected!"));
    ui->arm_button->setEnabled(true);
    ui->throttle_slider->setEnabled(true);
    ui->pitch_slider->setEnabled(true);
    ui->roll_slider->setEnabled(true);
    ui->arm_button->setStyleSheet("background-color: green");

    ui->throttle_slider->setValue(0);
    ui->pitch_slider->setValue(50);
    ui->roll_slider->setValue(50);

    connected_sound->play();

    timer->start(3000);
}

void MainWindow::disconnected()
{
    log(QString("Disconnected!"));
    ui->arm_button->setEnabled(false);
    ui->throttle_slider->setEnabled(false);
    ui->pitch_slider->setEnabled(false);
    ui->roll_slider->setEnabled(false);
    ui->arm_button->setText("ARM");
    ui->arm_button->setStyleSheet("background-color: white");

    disconnected_sound->play();
}

void MainWindow::on_arm_button_clicked()
{
    if(ui->arm_button->text() == "ARM")
    {
        mServer.sendData(110);
        log("Sent ON signal");
        ui->arm_button->setText("DISARM");
        ui->arm_button->setStyleSheet("background-color: red");
        ui->arm_button->setStatusTip("Disarms the quadcopter");
    }
    else
    {
        mServer.sendData(120);
        log("Sent OFF signal");
        ui->arm_button->setText("ARM");
        ui->arm_button->setStyleSheet("background-color: green");
        ui->arm_button->setStatusTip("Arms the quadcopter");
    }
}

void MainWindow::on_clear_screen_button_clicked()
{
    /*
     * QMessageBox msgbox;
     * msgbox.setText("Message box");
     * msgbox.exec();
     */
    ui->textBrowser->setText("");
}

void MainWindow::on_save_log_button_clicked()
{
    QString filename = QTime::currentTime().toString()+ " --- " + QDate::currentDate().toString() + " log";
    if(!QFile::exists(filename))
    {
        //SAVE FILE WITH CURRENT DATE AND TIME AS NAME
        //SAVE FILE IN CUSTOM DIRECTORY?
        QFile logfile(filename+".txt");
        if(!logfile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            log("Couldn't open file to save into");
        }
        else
        {
            QTextStream out(&logfile);
            out << ui->textBrowser->toPlainText();
            out.flush();
            logfile.close();
            log("Saved log");
        }
    }
}

void MainWindow::on_throttle_slider_valueChanged(int value)
{
    ui->throttle_label->setText(QString::number(value));
    if(mServer.isConnected())
    {
        mServer.sendData(THROTTLE_CODE);
        mServer.sendData(value);
    }
}

void MainWindow::on_roll_slider_valueChanged(int value)
{
    ui->roll_label->setText(QString::number(value));
    if(mServer.isConnected())
    {
        mServer.sendData(ROLL_CODE);
        mServer.sendData(value);
    }
}

void MainWindow::on_pitch_slider_valueChanged(int value)
{
    ui->pitch_label->setText(QString::number(value));
    if(mServer.isConnected())
    {
        mServer.sendData(PITCH_CODE);
        mServer.sendData(value);
    }
}

void MainWindow::on_roll_slider_sliderReleased()
{
    ui->roll_slider->setValue(50);
}

void MainWindow::on_pitch_slider_sliderReleased()
{
    ui->pitch_slider->setValue(50);
}

void MainWindow::send_point()
{
    if(mServer.isConnected())
    {
        mServer.sendData('.');
    }
}
