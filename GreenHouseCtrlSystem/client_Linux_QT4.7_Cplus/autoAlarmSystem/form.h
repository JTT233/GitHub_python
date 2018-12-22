#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QTcpSocket>
#include "widget.h"
#include <QTimer>
#include <QMessageBox>
#include "mymesgebox.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private slots:
    void RM0_recv_data();

    void GSM_recv_data();

    void on_fan_on_btn_clicked();

    void on_fan_off_btn_clicked();

    void on_led_on_btn_clicked();

    void on_led_off_btn_clicked();

    void on_pwm_on_btn_clicked();

    void on_pwm_off_btn_clicked();

    void on_shuma_on_btn_clicked();

    void on_shuma_off_btn_clicked();


    void myinitDateTime();
    void mytimeout();
    void myforminitDateTime();
    void formcurrentDateTime();
    void on_send_btn_clicked();

    void on_exit_btn_clicked();

    void on_useMapButton_clicked();


private:
     Ui::Form *ui;
//     QPixmap pix;
//     char pic_buf[100000];
  //   int pic_len;
  //   static int i;
     QMovie *movie;
     QTimer *m_timer;
     QTimer *t_timer;
     //QMessageBox *mesgbox;
     //Mymesgebox *mymesgebox;
     static int time_flag;

     static int init_fan_flag;
     static int init_led_flag;
     static int init_pwm_flag;
     static int init_shu_flag;


     static int flag_fan;
     static int flag_led;
     static int flag_pwm;
     static int flag_shu;

};

#endif // FORM_H
