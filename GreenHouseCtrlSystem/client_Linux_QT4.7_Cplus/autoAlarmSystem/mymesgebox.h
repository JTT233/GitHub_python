#ifndef MYMESGEBOX_H
#define MYMESGEBOX_H

#include <QWidget>
#include "widget.h"
#include "form.h"
#include <QTimer>
#include <QKeyEvent>
#include <QEvent>

namespace Ui {
class Mymesgebox;
}

class Mymesgebox : public QWidget
{
    Q_OBJECT

public:
    explicit Mymesgebox(QWidget *parent = 0);
    ~Mymesgebox();
signals:
    void myexit();
    void mynoexit();

private slots:
    void on_exit_btn_clicked();

    void on_noexit_btn_clicked();

    void myinitDateTime();

    void mytimeout();

private:
    Ui::Mymesgebox *ui;
    QTimer *my_timer;
    static int i;
    static int time_flag;
protected:
    void keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Enter){
            emit myexit();
           // qDebug()<<'key';
        }
        if (event->key() == Qt::Key_Escape){
            emit mynoexit();
        }

    }


};

#endif // MYMESGEBOX_H
