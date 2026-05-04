#include "tcpclientwidget.h"

#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

TcpClientWidget::TcpClientWidget(QWidget *parent)
    : QWidget(parent)
    , socket(new QTcpSocket(this))
{
    auto *layout = new QVBoxLayout(this);

    ipEdit = new QLineEdit(this);
    ipEdit->setPlaceholderText("Enter IP address");

    toggleBtn = new QPushButton("Connect", this);

    statusLabel = new QLabel("Disconnected", this);

    layout->addWidget(ipEdit);
    layout->addWidget(toggleBtn);
    layout->addWidget(statusLabel);

    connect(toggleBtn, &QPushButton::clicked,
            this, &TcpClientWidget::toggleConnection);

    connect(socket, &QTcpSocket::connected,
            this, &TcpClientWidget::onConnected);

    connect(socket, &QTcpSocket::disconnected,
            this, &TcpClientWidget::onDisconnected);

    connect(socket, &QTcpSocket::errorOccurred,
            this, &TcpClientWidget::onError);
}

TcpClientWidget::~TcpClientWidget() = default;

bool TcpClientWidget::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState;
}

void TcpClientWidget::toggleConnection()
{
    if (isConnected())
    {
        socket->disconnectFromHost();
        setStatus("Disconnecting...", "orange");
    }
    else
    {
        socket->connectToHost(ipEdit->text(), 4444);
        setStatus("Connecting...", "orange");
    }

    updateButton();
}

void TcpClientWidget::onConnected()
{
    setStatus("Connected", "green");
    updateButton();
}

void TcpClientWidget::onDisconnected()
{
    setStatus("Disconnected", "red");
    updateButton();
}

void TcpClientWidget::onError()
{
    setStatus(socket->errorString(), "red");
    updateButton();
}

void TcpClientWidget::setStatus(const QString &text, const QString &color)
{
    statusLabel->setText(text);
    statusLabel->setStyleSheet("color: " + color + ";");
}

void TcpClientWidget::updateButton()
{
    toggleBtn->setText(isConnected() ? "Disconnect" : "Connect");
}