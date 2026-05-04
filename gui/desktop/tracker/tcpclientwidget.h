#pragma once

#include <QWidget>

class QTcpSocket;
class QLineEdit;
class QPushButton;
class QLabel;
class QTextEdit;

class TcpClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TcpClientWidget(QWidget *parent = nullptr);
    ~TcpClientWidget();

private slots:
    void toggleConnection();
    void onConnected();
    void onDisconnected();
    void onError();
    void onReadyRead();

private:
    bool isConnected() const;
    void setStatus(const QString &text, const QString &color);
    void log(const QString &msg);
    void updateButton();

private:
    QTcpSocket *socket;

    QLineEdit *ipEdit;
    QPushButton *toggleBtn;
    QLabel *statusLabel;
    QTextEdit *logBox;
};