#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTcpSocket>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QKeyEvent>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected: 
    void keyPressEvent(QKeyEvent *event) override;   
    void keyReleaseEvent(QKeyEvent *event) override; 

private:
    QSerialPort *serial;
    
    // --- KHAI BÁO 2 CỔNG MẠNG ---
    QTcpSocket *socket;        // Cổng nhận lệnh (65432)
    QTcpSocket *video_socket;  // Cổng nhận Video Livestream (65433)
    QByteArray video_buffer;   // Bộ đệm chứa dữ liệu ảnh
    quint32 expected_image_size = 0; 

    QLabel *camera_view;       // Màn hình Camera
    QLabel *logo_label;
    QLabel *status_connection;
    QLabel *status_distance;
    QLabel *status_ai;
    QComboBox *cb_port;
    QPushButton *btn_connect, *btn_disconnect;
    QPushButton *btn_up, *btn_down, *btn_left, *btn_right, *btn_stop;
    QPushButton *btn_fl, *btn_fr, *btn_bl, *btn_br; 
    QSlider *speed_slider;
    QLabel *speed_label;

    int current_speed = 140; 

private slots:
    void connect_serial();
    void disconnect_serial();
    void send_command(QString cmd);
    void read_socket_data();
    void read_video_data();    // Hàm giải mã ảnh JPEG
    void read_serial_data();
    void update_speed(int value);
};

#endif