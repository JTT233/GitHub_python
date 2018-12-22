#include "form.h"
#include "ui_form.h"
#include <QTextCodec>
#include <QDateTime>
#include <QColorDialog>
#include <QFileDialog>
#include <QBuffer>
#include <QFile>
#include <QPixmap>
#include <QImageReader>
#include <QMovie>
#include <QDateTime>
#include <QDate>



int Form::time_flag = 1;
Mymesgebox *mymesgebox;
int status;


//int Form::init_fan_flag = 0;
//int Form::init_led_flag = 0;
//int Form::init_pwm_flag = 0;
//int Form::init_shu_flag = 0;


//int Form::flag_fan = 0;
//int Form::flag_led = 0;
//int Form::flag_pwm = 0;
//int Form::flag_shu = 0;

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->led_lab->setPixmap(QPixmap("lightoff.png"));
    ui->fan_lab->setPixmap(QPixmap("风扇.png"));

    ui->fan_lab->setScaledContents(true);  //自动缩放图片
    ui->led_lab->setScaledContents(true);  //自动缩放图片

     this->myforminitDateTime();  //显示时间

    ui->fan_on_btn->setAutoRaise(true);//鼠标经过是自动凸起
    ui->fan_on_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//文本在图标的下面
    ui->fan_off_btn->setAutoRaise(true);//鼠标经过是自动凸起
    ui->fan_off_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//文本在图标的下面

    ui->led_on_btn->setAutoRaise(true);//鼠标经过是自动凸起
    ui->led_on_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//文本在图标的下面
    ui->led_off_btn->setAutoRaise(true);//鼠标经过是自动凸起
    ui->led_off_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//文本在图标的下面

    ui->shuma_on_btn->setAutoRaise(true);//鼠标经过是自动凸起
    ui->shuma_on_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//文本在图标的下面
    ui->shuma_off_btn->setAutoRaise(true);//鼠标经过是自动凸起
    ui->shuma_off_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//文本在图标的下面

    ui->pwm_on_btn->setAutoRaise(true);//鼠标经过是自动凸起
    ui->pwm_on_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//文本在图标的下面
    ui->pwm_off_btn->setAutoRaise(true);//鼠标经过是自动凸起
    ui->pwm_off_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//文本在图标的下面

    ui->send_btn->setAutoRaise(true);//鼠标经过是自动凸起
    ui->send_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//文本在图标的下面
    ui->exit_btn->setAutoRaise(true);//鼠标经过是自动凸起
    ui->exit_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//文本在图标的下面

}

Form::~Form()
{
    delete ui;
}

void Form::myforminitDateTime()
{
   t_timer = new QTimer(this);
   connect(t_timer,SIGNAL(timeout()),this,SLOT(formcurrentDateTime()));
   t_timer->start(1000);
}

void Form::formcurrentDateTime()   //显示当前时间和日期
{
    QDateTime time = QDateTime::currentDateTime();
    QString text = time.toString("hh:mm:ss");
    ui->time_lab->setText(text);

    ui->date_lab->setText(QDate::currentDate().toString("yyyy-MM-dd"));
}

//M0
void Form::on_fan_on_btn_clicked()
{
    const unsigned char buf[36] = {0xdd, 0x09, 0x24, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    tcpsocket_M0_W->write((char *)buf,sizeof(buf));
    tcpsocket_M0_W->flush();

    movie = new QMovie("风扇.gif");   //显示动态图
    ui->fan_lab->setMovie(movie);
    movie->start();
}

void Form::on_fan_off_btn_clicked()
{
    const unsigned char buf[36] = {0xdd, 0x09, 0x24, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    tcpsocket_M0_W->write((char *)buf,sizeof(buf));
    tcpsocket_M0_W->flush();

    movie->stop();  //让动态图停止
}

void Form::on_led_on_btn_clicked()
{
    const unsigned char buf[36] = {0xdd, 0x09, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    tcpsocket_M0_W->write((char *)buf,sizeof(buf));
    tcpsocket_M0_W->flush();

    ui->led_lab->setPixmap(QPixmap("lighton.png"));
}

void Form::on_led_off_btn_clicked()
{
    const unsigned char buf[36] = {0xdd, 0x09, 0x24, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    tcpsocket_M0_W->write((char *)buf,sizeof(buf));
    tcpsocket_M0_W->flush();

    ui->led_lab->setPixmap(QPixmap("lightoff.png"));
}

void Form::on_pwm_on_btn_clicked()
{
    const unsigned char buf[36] = {0xdd, 0x09, 0x24, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    tcpsocket_M0_W->write((char *)buf,sizeof(buf));
    tcpsocket_M0_W->flush();
}

void Form::on_pwm_off_btn_clicked()
{
    const unsigned char buf[36] = {0xdd, 0x09, 0x24, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    tcpsocket_M0_W->write((char *)buf,sizeof(buf));
    tcpsocket_M0_W->flush();
}

void Form::on_shuma_on_btn_clicked()
{
    const unsigned char buf[36] = {0xdd, 0x09, 0x24, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    tcpsocket_M0_W->write((char *)buf,sizeof(buf));
    tcpsocket_M0_W->flush();
}

void Form::on_shuma_off_btn_clicked()
{
    const unsigned char buf[36] = {0xdd, 0x09, 0x24, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    tcpsocket_M0_W->write((char *)buf,sizeof(buf));
    tcpsocket_M0_W->flush();
}


void Form::RM0_recv_data()
{
    QByteArray arr =tcpsocket_M0_R->readAll();
    QString str = arr.toHex().toLower();
    qDebug()<< "str: "<<str <<endl;
    int id = str[3].toLatin1() - '0';
    qDebug()<<"id: "<< id <<endl;
    int tmp =0;
    //16进制转10
    if (str[10].toLatin1() >= 'a' && str[10].toLatin1() <= 'f')
    {
        tmp += (str[10].toLatin1() - 'a' + 10) * 16;
    }
    else
    {
        tmp += (str[10].toLatin1() - '0') * 16;
    }
    if (str[11].toLatin1() >= 'a' && str[11].toLatin1() <= 'f')
    {
        tmp += (str[11].toLatin1() - 'a' + 10);

    }
    else
    {
        tmp += (str[11].toLatin1() - '0');
    }
    qDebug()<< "tmp: "<<tmp <<endl;

    int hum =0;
    if (str[14].toLatin1() >= 'a' && str[14].toLatin1() <= 'f')
    {
        hum += (str[14].toLatin1() - 'a' + 10) * 16;
    }
    else
    {
        hum += (str[14].toLatin1() - '0') * 16;
    }
    if (str[15].toLatin1() >= 'a' && str[15].toLatin1() <= 'f')
    {
        hum += (str[15].toLatin1() - 'a' + 10);
    }
    else
    {
        hum += (str[15].toLatin1() - '0');
    }
    // int hum =  str[6].toUpper().toLatin1() + str[7].toUpper().toLatin1();
    // float hume = hum.toFloat();
    qDebug()<< "hum: " << hum <<endl;

    int ill =0;
    if (str[40].toLatin1() >= 'a' && str[40].toLatin1() <= 'f')
    {
        ill += (str[40].toLatin1() - 'a' + 10) * 16;
    }
    else
    {
        ill += (str[40].toLatin1() - '0') * 16;
    }
    if (str[41].toLatin1() >= 'a' && str[41].toLatin1() <= 'f')
    {
        ill += (str[41].toLatin1() - 'a' + 10);
    }
    else
    {
        ill += (str[41].toLatin1() - '0');
    }
    if (str[42].toLatin1() >= 'a' && str[42].toLatin1() <= 'f')
    {
        ill += (str[42].toLatin1() - 'a' + 10) * 16 * 16 * 16;
    }
    else
    {
        ill += (str[42].toLatin1() - '0') * 16 * 16 * 16;
    }
    if (str[43].toLatin1() >= 'a' && str[43].toLatin1() <= 'f')
    {
        ill += (str[43].toLatin1() - 'a' + 10) * 16 * 16;
    }
    else
    {
        ill += (str[43].toLatin1() - '0') * 16 * 16;
    }
    qDebug()<< "ill: " <<ill <<endl;


   /*界面显示部分 */
    QString illstr;
    QString strill = "   "+illstr.number(ill) + "   Lx";  //
    ui->light_lab->setText(strill);

    QString tmpstr;
    QString strtmp = "   "+tmpstr.number(tmp) + "   ℃";
    ui->T_lab1->setText(strtmp);

    QString humstr;
    QString strhum = "   "+humstr.number(hum) + " %RH";
    ui->shidu_lab1->setText(strhum);
/* 主要功能实现部分*/
/*
    if(50 <= tmp)
    {
        const unsigned char buf[36] = {0xdd, 0x09, 0x24, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        tcpsocket_M0_W->write((char *)buf,sizeof(buf));
        tcpsocket_M0_W->flush();
        //on_send_btn_clicked();
    }
    else
    {
        const unsigned char buf[36] = {0xdd, 0x09, 0x24, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        tcpsocket_M0_W->write((char *)buf,sizeof(buf));
        tcpsocket_M0_W->flush();
    }
*/
    if(50 > ill)
    {
        const unsigned char buf[36] = {0xdd, 0x09, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        tcpsocket_M0_W->write((char *)buf,sizeof(buf));
        tcpsocket_M0_W->flush();
        //on_send_btn_clicked();
    }
    else
    {
        const unsigned char buf[36] = {0xdd, 0x09, 0x24, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        tcpsocket_M0_W->write((char *)buf,sizeof(buf));
        tcpsocket_M0_W->flush();
    }

}


//GSM
void Form::GSM_recv_data()
{
}
void Form::on_send_btn_clicked()
{
    //QString str = ui->send_edt->text()+ui->central_edt->text()+ui->rcv_edt->text()+ ui->flag_edt->text();// ->toPlainText();
    QString str = ui->rcv_edt->text()+ ui->flag_edt->text();// ->toPlainText();
    QByteArray arr;
    qDebug()<<"gsm_str: "<<str<<endl;
    arr.append(str);
    tcpsocket_GSM->write(arr);
    tcpsocket_GSM->flush();
}



void Form::on_exit_btn_clicked()
{
    mymesgebox = new Mymesgebox;
    mymesgebox->show();
}

void Form::on_useMapButton_clicked()
{
     QMessageBox::about(this,tr("提示信息"),tr("自动检测就是根据环境温度高低来判断是否要发出警报或者发送短信， "
                                                             "主动控制就是自己可以点击相应按钮做出相应反应"));
}

