#include "MyDialog.h"
#include "ui_mydialog.h"
#include <QDebug>
#include <QMessageBox>


MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);
    initDialog();
    initTableWidget();

    connect(ui->btnEnsure,&QPushButton::clicked,this,&MyDialog::confirmData);
    connect(ui->btnCancel,&QPushButton::clicked,this,&MyDialog::cancelData);

}

MyDialog::~MyDialog()
{
    delete ui;
}

void MyDialog::initDialog()
{
    setWindowTitle("请输入新增的学生信息");
}


void MyDialog::initTableWidget()
{

    ui->newStudentTable->setColumnCount(3);
    ui->newStudentTable->setRowCount(1);
    QStringList strs = {"姓名","年龄","分数"};
    ui->newStudentTable->setHorizontalHeaderLabels(strs);

    ui->newStudentTable->horizontalHeader()->setStretchLastSection(true);   //最后一列自动填充
    ui->newStudentTable->verticalHeader()->setVisible(false);               //首列消失
}


void MyDialog::confirmData()
{
     if(ui->newStudentTable->item(0,0)==nullptr||
             (ui->newStudentTable->item(0,0)&&ui->newStudentTable->item(0,0)->text()==tr("")))
     {
        QMessageBox::information(this, "提示", "姓名不能为空",QMessageBox::Yes);

     }else if(ui->newStudentTable->item(0,1)==nullptr||
              (ui->newStudentTable->item(0,1)&&ui->newStudentTable->item(0,1)->text()==tr("")))
     {
         QMessageBox::information(this, "提示", "年龄不能为空",QMessageBox::Yes);

     }else if(ui->newStudentTable->item(0,2)==nullptr||
              (ui->newStudentTable->item(0,2)&&ui->newStudentTable->item(0,2)->text()==tr("")))
     {
         QMessageBox::information(this, "提示", "分数不能为空", QMessageBox::Yes);
     }
     else if(ui->newStudentTable->item(0,1)->text().toInt() == false ||
             ui->newStudentTable->item(0,1)->text().toInt()<0 ||
             ui->newStudentTable->item(0,1)->text().toInt()>150)
     {
        QMessageBox::information(this, "提示", "请输入合适的年龄 数字（0，150）",QMessageBox::Yes);
     }
     else if(ui->newStudentTable->item(0,2)->text().toInt() == false ||
             ui->newStudentTable->item(0,2)->text().toInt()<0 ||
             ui->newStudentTable->item(0,2)->text().toInt()>150)
     {
        QMessageBox::information(this, "提示", "请输入合适的分数 数字（0，100）",QMessageBox::Yes);
     }else
     {
             QString name,school,className,age,score;
             name  = ui->newStudentTable->item(0,0)->text();
             age   = ui->newStudentTable->item(0,1)->text();
             score = ui->newStudentTable->item(0,2)->text();
             qDebug()<< name << age <<score;
             emit setNewStudent(name,age.toInt(),score.toInt());
             ui->newStudentTable->clearContents();
             close();
     }
}

void MyDialog::cancelData()
{
   close();
}


