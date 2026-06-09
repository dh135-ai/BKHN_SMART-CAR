#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSerialPortInfo>
#include <QPixmap>
#include <QDataStream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setFixedSize(1000, 650);
    this->setStyleSheet("background-color: #0f0f0f; color: white;");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // ==========================================
    // --- BÊN TRÁI: CAMERA & SINH VIÊN ---
    // ==========================================
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    leftLayout->setContentsMargins(10, 20, 10, 10);

    // LỘT XÁC THÀNH MÀN HÌNH CAMERA
    camera_view = new QLabel("CHỜ PYTHON BẬT CAMERA...");
    camera_view->setFixedSize(640, 480);
    camera_view->setStyleSheet("background-color: #111; color: gray; border: 3px solid #00e6e6; font-size: 16px; font-weight: bold;");
    camera_view->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(camera_view);

    leftLayout->addSpacing(25);

    QString studentStyle = "font-size: 18px; font-weight: bold; color: #00e6e6;";
    QLabel *sv1 = new QLabel("Trần Đức Hiếu_20236167");
    QLabel *sv2 = new QLabel("Trần Đắc Công Trường_20236349");
    QLabel *sv3 = new QLabel("Nguyễn Quang Vinh_20236369");

    sv1->setStyleSheet(studentStyle); sv1->setAlignment(Qt::AlignCenter);
    sv2->setStyleSheet(studentStyle); sv2->setAlignment(Qt::AlignCenter);
    sv3->setStyleSheet(studentStyle); sv3->setAlignment(Qt::AlignCenter);

    leftLayout->addWidget(sv1); leftLayout->addWidget(sv2); leftLayout->addWidget(sv3);
    mainLayout->addLayout(leftLayout);

    // ==========================================
    // --- BÊN PHẢI: BẢNG ĐIỀU KHIỂN ---
    // ==========================================
    QFrame *panel = new QFrame();
    panel->setFixedWidth(300);
    panel->setStyleSheet("background-color: #1a1a1c; border: 2px solid #333; border-radius: 10px;");
    QVBoxLayout *rightLayout = new QVBoxLayout(panel);
    rightLayout->setAlignment(Qt::AlignTop);

    logo_label = new QLabel();
    QPixmap logo("bkhn.png");
    logo_label->setPixmap(logo.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logo_label->setAlignment(Qt::AlignCenter);
    logo_label->setStyleSheet("border: none;");
    rightLayout->addWidget(logo_label, 0, Qt::AlignCenter);

    QLabel *title = new QLabel("BKHN SMART CAR");
    title->setStyleSheet("font-size: 18px; font-weight: bold; border: none;");
    title->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(title);
    rightLayout->addSpacing(15);

    status_connection = new QLabel("DISCONNECTED");
    status_connection->setStyleSheet("color: red; font-weight: bold; font-size: 14px; border: none;");
    status_connection->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(status_connection);

    status_distance = new QLabel("Distance: -- cm");
    status_distance->setStyleSheet("color: yellow; font-weight: bold; font-size: 14px; border: none;");
    status_distance->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(status_distance);
    rightLayout->addSpacing(15);

    cb_port = new QComboBox();
    cb_port->setStyleSheet("background-color: #333; color: white; border: 1px solid gray; padding: 4px;");
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) { cb_port->addItem(info.portName()); }
    rightLayout->addWidget(cb_port);

    btn_connect = new QPushButton("CONNECT");
    btn_connect->setStyleSheet("background-color: #009900; color: white; height: 35px; font-weight: bold; border: none; border-radius: 5px;");
    btn_disconnect = new QPushButton("DISCONNECT");
    btn_disconnect->setStyleSheet("background-color: #cc0000; color: white; height: 35px; font-weight: bold; border: none; border-radius: 5px;");
    rightLayout->addWidget(btn_connect); rightLayout->addWidget(btn_disconnect);
    rightLayout->addSpacing(15);

    speed_label = new QLabel("Speed: 140");
    speed_label->setAlignment(Qt::AlignCenter);
    speed_label->setStyleSheet("border: none;");
    rightLayout->addWidget(speed_label);

    speed_slider = new QSlider(Qt::Horizontal);
    speed_slider->setRange(0, 255);
    speed_slider->setValue(140);
    rightLayout->addWidget(speed_slider);
    rightLayout->addSpacing(20);

    QGridLayout *btnGrid = new QGridLayout();
    btn_up = new QPushButton("▲"); btn_down = new QPushButton("▼");
    btn_left = new QPushButton("◀"); btn_right = new QPushButton("▶");
    btn_stop = new QPushButton("STOP");
    btn_fl = new QPushButton("◤"); btn_fr = new QPushButton("◥");
    btn_bl = new QPushButton("◣"); btn_br = new QPushButton("◢");
    
    QString btnStyle = 
        "QPushButton { background-color: #333333; color: white; font-size: 22px; font-weight: bold; height: 45px; border: 2px solid #444; border-radius: 8px; }"
        "QPushButton:hover { background-color: #4a4a4c; border: 2px solid #00e6e6; }"
        "QPushButton:pressed { background-color: #00e6e6; color: black; }";
        
    btn_up->setStyleSheet(btnStyle); btn_down->setStyleSheet(btnStyle);
    btn_left->setStyleSheet(btnStyle); btn_right->setStyleSheet(btnStyle);
    btn_fl->setStyleSheet(btnStyle); btn_fr->setStyleSheet(btnStyle);
    btn_bl->setStyleSheet(btnStyle); btn_br->setStyleSheet(btnStyle);
    btn_stop->setStyleSheet(
        "QPushButton { background-color: #cc0000; color: white; font-size: 14px; font-weight: bold; height: 45px; border: 2px solid #ff4444; border-radius: 8px; }"
        "QPushButton:hover { background-color: #ff3333; }"
        "QPushButton:pressed { background-color: #990000; }");

    btnGrid->addWidget(btn_fl, 0, 0); btnGrid->addWidget(btn_up, 0, 1); btnGrid->addWidget(btn_fr, 0, 2);
    btnGrid->addWidget(btn_left, 1, 0); btnGrid->addWidget(btn_stop, 1, 1); btnGrid->addWidget(btn_right, 1, 2);
    btnGrid->addWidget(btn_bl, 2, 0); btnGrid->addWidget(btn_down, 2, 1); btnGrid->addWidget(btn_br, 2, 2);

    rightLayout->addLayout(btnGrid);
    rightLayout->addStretch();

    status_ai = new QLabel("AI: WAITING...");
    status_ai->setStyleSheet("color: cyan; font-weight: bold; font-size: 14px; border: none;");
    status_ai->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(status_ai);
    mainLayout->addWidget(panel);

    // ==========================================
    // --- KẾT NỐI MẠNG & THIẾT BỊ ---
    // ==========================================
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::read_serial_data);

    // Cổng 1: Nhận Lệnh (65432)
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 65432);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::read_socket_data);

    // Cổng 2: Nhận Livestream Video (65433)
    video_socket = new QTcpSocket(this);
    video_socket->connectToHost("127.0.0.1", 65433);
    connect(video_socket, &QTcpSocket::readyRead, this, &MainWindow::read_video_data);

    connect(btn_connect, &QPushButton::clicked, this, &MainWindow::connect_serial);
    connect(btn_disconnect, &QPushButton::clicked, this, &MainWindow::disconnect_serial);
    connect(speed_slider, &QSlider::valueChanged, this, &MainWindow::update_speed);

    // Click chuột vào nút bấm
    connect(btn_up, &QPushButton::pressed, [=](){ send_command("F"); }); connect(btn_up, &QPushButton::released, [=](){ send_command("S"); });
    connect(btn_down, &QPushButton::pressed, [=](){ send_command("B"); }); connect(btn_down, &QPushButton::released, [=](){ send_command("S"); });
    connect(btn_left, &QPushButton::pressed, [=](){ send_command("L"); }); connect(btn_left, &QPushButton::released, [=](){ send_command("S"); });
    connect(btn_right, &QPushButton::pressed, [=](){ send_command("R"); }); connect(btn_right, &QPushButton::released, [=](){ send_command("S"); });
    connect(btn_fl, &QPushButton::pressed, [=](){ send_command("Q"); }); connect(btn_fl, &QPushButton::released, [=](){ send_command("S"); });
    connect(btn_fr, &QPushButton::pressed, [=](){ send_command("E"); }); connect(btn_fr, &QPushButton::released, [=](){ send_command("S"); });
    connect(btn_bl, &QPushButton::pressed, [=](){ send_command("Z"); }); connect(btn_bl, &QPushButton::released, [=](){ send_command("S"); });
    connect(btn_br, &QPushButton::pressed, [=](){ send_command("C"); }); connect(btn_br, &QPushButton::released, [=](){ send_command("S"); });
    connect(btn_stop, &QPushButton::pressed, [=](){ send_command("S"); });
}

// Giải mã Video Stream gửi từ Python
void MainWindow::read_video_data() {
    video_buffer.append(video_socket->readAll());
    while (true) {
        if (expected_image_size == 0) {
            if (video_buffer.size() < 4) return; // Đợi đủ 4 bytes đọc dung lượng ảnh
            QDataStream stream(&video_buffer, QIODevice::ReadOnly);
            stream >> expected_image_size;
            video_buffer.remove(0, 4);
        }
        if (video_buffer.size() < expected_image_size) return; // Đợi load đủ ảnh

        QByteArray imageData = video_buffer.left(expected_image_size);
        video_buffer.remove(0, expected_image_size);
        expected_image_size = 0;

        QPixmap pixmap;
        if (pixmap.loadFromData(imageData, "JPEG")) {
            camera_view->setPixmap(pixmap.scaled(camera_view->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
    }
}

// Bàn phím WASD
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return; 
    switch (event->key()) {
        case Qt::Key_W: btn_up->setDown(true); send_command("F"); break;
        case Qt::Key_S: btn_down->setDown(true); send_command("B"); break;
        case Qt::Key_A: btn_left->setDown(true); send_command("L"); break;
        case Qt::Key_D: btn_right->setDown(true); send_command("R"); break;
        case Qt::Key_Q: btn_fl->setDown(true); send_command("Q"); break;
        case Qt::Key_E: btn_fr->setDown(true); send_command("E"); break;
        case Qt::Key_Z: btn_bl->setDown(true); send_command("Z"); break;
        case Qt::Key_C: btn_br->setDown(true); send_command("C"); break;
        case Qt::Key_R: speed_slider->setValue(qMin(255, speed_slider->value() + 1)); break; 
        case Qt::Key_F: speed_slider->setValue(qMax(0, speed_slider->value() - 1)); break;   
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;
    switch (event->key()) {
        case Qt::Key_W: btn_up->setDown(false); send_command("S"); break;
        case Qt::Key_S: btn_down->setDown(false); send_command("S"); break;
        case Qt::Key_A: btn_left->setDown(false); send_command("S"); break;
        case Qt::Key_D: btn_right->setDown(false); send_command("S"); break;
        case Qt::Key_Q: btn_fl->setDown(false); send_command("S"); break;
        case Qt::Key_E: btn_fr->setDown(false); send_command("S"); break;
        case Qt::Key_Z: btn_bl->setDown(false); send_command("S"); break;
        case Qt::Key_C: btn_br->setDown(false); send_command("S"); break;
    }
}

void MainWindow::update_speed(int value) { current_speed = value; speed_label->setText("Speed: " + QString::number(value)); }
void MainWindow::connect_serial() {
    serial->setPortName(cb_port->currentText());
    serial->setBaudRate(QSerialPort::Baud115200);
    if(serial->open(QIODevice::ReadWrite)) {
        status_connection->setText("CONNECTED"); status_connection->setStyleSheet("color: lime; font-weight: bold; font-size: 14px; border: none;");
    }
}
void MainWindow::disconnect_serial() {
    serial->close();
    status_connection->setText("DISCONNECTED"); status_connection->setStyleSheet("color: red; font-weight: bold; font-size: 14px; border: none;");
    status_distance->setText("Distance: -- cm");
}
void MainWindow::send_command(QString cmd) {
    if(serial->isOpen()) {
        QString full_cmd = cmd + QString::number(current_speed) + "\n";
        serial->write(full_cmd.toUtf8());
        status_ai->setText("SENT: " + cmd);
    }
}
void MainWindow::read_socket_data() {
    QByteArray data = socket->readAll();
    QString cmd = QString(data).trimmed();
    if (!cmd.isEmpty()) send_command(cmd);
}
void MainWindow::read_serial_data() {
    while (serial->canReadLine()) {
        QString data = QString(serial->readLine()).trimmed();
        if (data.contains("Distance:")) {
            status_distance->setText(data); status_distance->setStyleSheet("color: yellow; font-weight: bold; font-size: 14px; border: none;");
        } else if (data.contains("Obstacle")) {
            status_distance->setText("OBSTACLE DETECTED!"); status_distance->setStyleSheet("color: red; font-weight: bold; font-size: 14px; border: none;");
        }
    }
}

MainWindow::~MainWindow() {}