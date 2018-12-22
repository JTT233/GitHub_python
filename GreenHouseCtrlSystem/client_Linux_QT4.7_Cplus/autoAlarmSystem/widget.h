#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include "ui_form.h"
#include "form.h"
#include <QTimer>
#include <QKeyEvent>
#include <QEvent>
#include <QDebug>

extern QString ip;
extern int port;
extern QTcpSocket *tcpsocket_M0_R;
extern QTcpSocket *tcpsocket_M0_W;
extern QTcpSocket *tcpsocket_GSM;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

signals:
    void myconnect();
    void mydisconnect();

private slots:
    void on_connect_btn_clicked();
    void RM0_connected();
    void WM0_connected();
    void GSM_connected();
    void on_disconnect_btn_clicked();
    void initDateTime();
    void currentDateTime();

protected:
    void keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Enter){
            emit myconnect();
           // qDebug()<<'key';
        }
        if (event->key() == Qt::Key_Escape){
            emit mydisconnect();
        }

    }

private:
    Ui::Widget *ui;
    QTimer *t_timer;
};

#endif // WIDGET_H
