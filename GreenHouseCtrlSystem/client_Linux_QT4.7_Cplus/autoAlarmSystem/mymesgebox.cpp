#include "mymesgebox.h"
#include "ui_mymesgebox.h"
#include "widget.h"
#include "form.h"


Widget *wid;
int Mymesgebox::i=10;
int Mymesgebox::time_flag = 2;
extern Form *form;

Mymesgebox::Mymesgebox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mymesgebox)
{
    ui->setupUi(this);
    this->myinitDateTime();  //不要忘记啊，不然是不会进行定时器的槽函数的
    connect(this, SIGNAL(myexit()), this, SLOT(on_exit_btn_clicked()));
    connect(this, SIGNAL(mynoexit()), this, SLOT(on_noexit_btn_clicked()));
}

Mymesgebox::~Mymesgebox()
{
    delete ui;
}

void Mymesgebox::myinitDateTime()
{
   my_timer = new QTimer(this);
   connect(my_timer,SIGNAL(timeout()),this,SLOT(mytimeout()));
   my_timer->start(1000);
}

/*进行倒计时，若在倒计时完后客户还没有选择，则系统自动选择不退出，并关闭当前页面*/
void Mymesgebox::mytimeout()
{

    QString num = QString::number(i--,10);
    ui->time_lab->setText(num);

    if (i < 0 || time_flag == 0)
    {
        i = 10;
       this->close();
    }
    if(time_flag == 1)
    {
        this->close();
        qDebug()<<"mymesageboxMytimeout";
        form->close();
        tcpsocket_GSM->close();
        tcpsocket_M0_W->close();
        tcpsocket_M0_R->close();
        /*
         *wid = new Widget;
         *wid->show();
         *上面两句不能要，本来想生成那个画面，如果加上上面两句结果会多生成一个Widget画面；
         */
        time_flag = 2;
    }
}

void Mymesgebox::on_exit_btn_clicked() //确定退出
{
    this->close();
    qDebug()<<"111111";
    form->close();
    tcpsocket_GSM->close();
    tcpsocket_M0_W->close();
    tcpsocket_M0_R->close();
    wid = new Widget;
    wid->show();
    time_flag = 1;
}

void Mymesgebox::on_noexit_btn_clicked()  //不退出
{
    time_flag = 0;
    this->close();
}
