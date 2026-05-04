#pragma once

#include <QWidget>
#include <QTcpSocket>

class QLineEdit;
class QPushButton;
class QLabel;

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

private:
    QTcpSocket *socket;

    QLineEdit *ipEdit;
    QPushButton *toggleBtn;
    QLabel *statusLabel;

    bool isConnected() const;

    void setStatus(const QString &text, const QString &color);
    void updateButton();
};