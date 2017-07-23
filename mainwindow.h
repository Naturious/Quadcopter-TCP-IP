#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QDate>
#include <QFile>
#include <QSound>
#include <QTimer>
#include <QtMultimedia/QMediaPlayer>
#include "myserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_tcp_ip_button_clicked();

    void connected();

    void disconnected();

    void on_arm_button_clicked();

    void on_clear_screen_button_clicked();

    void on_save_log_button_clicked();

    void on_throttle_slider_valueChanged(int value);

    void on_roll_slider_valueChanged(int value);

    void on_pitch_slider_valueChanged(int value);

    void on_roll_slider_sliderReleased();

    void on_pitch_slider_sliderReleased();

    void send_point();

private:
    Ui::MainWindow *ui;
    MyServer mServer;

    QTimer *timer;
    QTimer *arm_timer;

    void log(const QString &str);

    const int PITCH_CODE = 130;
    const int ROLL_CODE = 140;
    const int THROTTLE_CODE = 150;

    QMediaPlayer *connected_sound;
    QMediaPlayer *disconnected_sound;
};

#endif // MAINWINDOW_H
