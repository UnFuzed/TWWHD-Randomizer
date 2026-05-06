#include "tcpclientwidget.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

TcpClientWidget::TcpClientWidget(QWidget *parent)
    : QWidget(parent)
    , socket(new QTcpSocket(this))
{
    auto *mainLayout = new QVBoxLayout(this);
    auto *topLayout = new QHBoxLayout();

    auto *leftLayout = new QVBoxLayout();

    ipEdit = new QLineEdit(this);
    ipEdit->setPlaceholderText("Enter IP");

    toggleBtn = new QPushButton("Connect", this);

    statusLabel = new QLabel("Disconnected", this);

    leftLayout->addWidget(ipEdit);
    leftLayout->addWidget(toggleBtn);
    leftLayout->addWidget(statusLabel);

    logBox = new QTextEdit(this);
    logBox->setReadOnly(true);
    logBox->setMinimumWidth(500);

    topLayout->addLayout(leftLayout);
    topLayout->addWidget(logBox, 1);

    mainLayout->addLayout(topLayout);
    setLayout(mainLayout);

    connect(toggleBtn, &QPushButton::clicked,
            this, &TcpClientWidget::toggleConnection);

    connect(socket, &QTcpSocket::connected,
            this, &TcpClientWidget::onConnected);

    connect(socket, &QTcpSocket::disconnected,
            this, &TcpClientWidget::onDisconnected);

    connect(socket, &QTcpSocket::errorOccurred,
            this, &TcpClientWidget::onError);

    connect(socket, &QTcpSocket::readyRead,
            this, &TcpClientWidget::onReadyRead);
}

TcpClientWidget::~TcpClientWidget() = default;

bool TcpClientWidget::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState;
}

void TcpClientWidget::toggleConnection()
{
    if (isConnected() || socket->state() == QAbstractSocket::ConnectingState)
    {
        log("Disconnect requested");
        setStatus("Disconnecting...", "orange");
        socket->abort();
        return;
    }

    QString ip = ipEdit->text();

    if (ip.isEmpty())
    {
        setStatus("No IP entered", "red");
        log("No IP entered");
        return;
    }

    log("Connecting to " + ip + ":4444");
    setStatus("Connecting...", "orange");

    socket->connectToHost(ip, 4444);
}

void TcpClientWidget::onConnected()
{
    setStatus("Connected", "green");
    log("Connected");
    updateButton();
}

void TcpClientWidget::onDisconnected()
{
    setStatus("Disconnected", "red");
    log("Disconnected");
    updateButton();
}

void TcpClientWidget::onError()
{
    setStatus("Error: " + socket->errorString(), "red");
    log("Error: " + socket->errorString());
    updateButton();
}

void TcpClientWidget::onReadyRead()
{
    buffer += socket->readAll();

    while (buffer.contains('\n'))
    {
        int index = buffer.indexOf('\n');
        QByteArray line = buffer.left(index).trimmed();
        buffer.remove(0, index + 1);

        log("RX: " + QString::fromUtf8(line));
        emit dataReceived(line);
    }
}

void TcpClientWidget::setStatus(const QString &text, const QString &color)
{
    statusLabel->setText(text);
    statusLabel->setStyleSheet("color: " + color + ";");
}

void TcpClientWidget::log(const QString &msg)
{
    logBox->append(msg);
}

void TcpClientWidget::updateButton()
{
    toggleBtn->setText(isConnected() ? "Disconnect" : "Connect");
}