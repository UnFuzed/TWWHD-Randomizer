#pragma once

#include <QWidget>
#include <QTcpSocket>

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

    bool isConnected() const;

signals:
    void dataReceived(const QByteArray& data);

private slots:
    void toggleConnection();
    void onConnected();
    void onDisconnected();
    void onError();
    void onReadyRead();

private:
    void setStatus(const QString &text, const QString &color);
    void log(const QString &msg);
    void updateButton();

    QTcpSocket *socket;
    QByteArray buffer;

    QLineEdit *ipEdit;
    QPushButton *toggleBtn;
    QLabel *statusLabel;
    QTextEdit *logBox;
};