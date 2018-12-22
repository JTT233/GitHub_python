#include "widget.h"
#include "ui_widget.h"
//#include "form.h"
#include <QDebug>
#include <QDateTime>
#include <QDate>
#include "widget.h"


QString ip;
int port;
QTcpSocket *tcpsocket_M0_R;
QTcpSocket *tcpsocket_M0_W;
QTcpSocket *tcpsocket_GSM;

Form *form;
extern int status;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->initDateTime();

    ui->connect_btn->setAutoRaise(true);//鼠标经过是自动凸起
    ui->connect_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//文本在图标的下面

    ui->disconnect_btn->setAutoRaise(true);//鼠标经过是自动凸起
    ui->disconnect_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//文本在图标的下面

    connect(this, SIGNAL(myconnect()), this, SLOT(on_connect_btn_clicked())); //按键关联
    connect(this, SIGNAL(mydisconnect()), this, SLOT(on_disconnect_btn_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initDateTime()
{
   t_timer = new QTimer(this);
   connect(t_timer,SIGNAL(timeout()),this,SLOT(currentDateTime()));
   t_timer->start(1000);
}

void Widget::currentDateTime()   //显示当前时间和日期
{
    QDateTime time = QDateTime::currentDateTime();
    QString text = time.toString("hh:mm:ss");
    ui->logo1_lab->setText(text);

    ui->data_lab->setText(QDate::currentDate().toString("yyyy-MM-dd"));
}

void Widget::on_connect_btn_clicked() //建立连接
{

    // tcpsocket = NULL;
    // QString ip = ui->ip_edt->text();
    // int port = ui->port_edt->text().toInt();  //从ip_det中获取ip和端口号

    //QString ip = ("192.168.2.20");
    QString ip = ("192.168.2.100");
    //QString ip = ("127.0.0.1");
    //QString ip = ("169.254.7.75");
    int port = 8080;
    qDebug()<<ip<<endl;
    qDebug()<<port<<endl;

    tcpsocket_M0_R = new QTcpSocket(this);
    tcpsocket_M0_W = new QTcpSocket(this);
    tcpsocket_GSM = new QTcpSocket(this);

    tcpsocket_M0_R->connectToHost(ip, port);  //链接服务器
    connect(tcpsocket_M0_R,SIGNAL(connected()),this,SLOT(RM0_connected()));


    tcpsocket_M0_W->connectToHost(ip, port);  //链接服务器
    connect(tcpsocket_M0_W,SIGNAL(connected()),this,SLOT(WM0_connected()));


    tcpsocket_GSM->connectToHost(ip, port);  //链接服务器
    connect(tcpsocket_GSM,SIGNAL(connected()),this,SLOT(GSM_connected()));
    //链接成功，会自动触发connected()这个信号，成功后让其net_connected()

      form = new Form;

    connect(tcpsocket_M0_R,SIGNAL(readyRead()),form,SLOT(RM0_recv_data()));  //套接字中有东西时，会触发readyRead()信号
    //connect(tcpsocket_M0_W,SIGNAL(readyRead()),form,SLOT(WM0_recv_data()));
    connect(tcpsocket_GSM,SIGNAL(readyRead()),form,SLOT(GSM_recv_data()));
}

void Widget::RM0_connected()
{ 
    QByteArray Arr;
    QString str = "RM0";
    Arr.append(str);
    tcpsocket_M0_R->write(Arr);
    qDebug()<<"RM0 connec"<<endl;

    this->close(); //关闭当前的链接页面
    form->show();  //跳转到主界面
}
void Widget::WM0_connected()
{
    QByteArray Arr;
    QString str = "WM0";
    Arr.append(str);
    tcpsocket_M0_W->write(Arr);
    qDebug()<<"WM0 connect"<<endl;
}
void Widget::GSM_connected()
{
    QByteArray Arr;
    QString str = "GSM";
    Arr.append(str);
    tcpsocket_GSM->write(Arr);
    qDebug()<<"gsm connenct"<<endl;

}


void Widget::on_disconnect_btn_clicked()
{
    tcpsocket_GSM->close();
    tcpsocket_M0_W->close();
    tcpsocket_M0_R->close();
    this->close();
}
