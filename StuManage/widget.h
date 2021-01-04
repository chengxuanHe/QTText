#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include "StuDataSql.h"
#include "MyDialog.h"
#define ROLE_MARK_TYPE Qt::UserRole + 100
#define MARK_SCHOOL  1
#define MARK_CLASS   2
#define MARK_STUDENT 3
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void fillterClass(QString name);   //筛选显示相关班级学生

public slots:
    void stuTableMenuRequested(const QPoint &point);           //学生信息表格右键弹出菜单
    void classViewMenuRequested(const QPoint &point);          //班级信息表格右键弹出菜单
    void classViewClick(const QModelIndex &index);             //班级点击槽函数
    void addNewStudent(QString name,int age,int score);        //获取对话框输入的学生信息并插入表格
private:
    void initWidget();                 //初始化表单
    void initTreeView();               //初始化学校树
    void initTableView();              //初始化学生表格


    void delStuData();                 //删除学生信息

    void addClassData();               //新增班级
    void displayAllClassData();        //显示全部信息
    void addStuDialog();               //新增学生信息弹出对话框
    void delClassData();               //删除班级





private:
    Ui::Widget *ui;

    StuDataSql *m_stuData;              //数据类实例
    QSqlTableModel *m_pTableModel;      //学生表格模型
    QStandardItemModel *m_pTreeModel;   //学校树模型

    MyDialog *m_pAddStuDialog;

};
#endif // WIDGET_H
