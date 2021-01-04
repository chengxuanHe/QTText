#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QMenu>
#include <QSqlRecord>
#include <QMultiMap>
#include "MyDialog.h"
#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    ui->setupUi(this);
    m_stuData = new StuDataSql(this);
    m_pTableModel = new QSqlTableModel(this,m_stuData->getDatabase());
    m_pTreeModel = new QStandardItemModel(0,1,this);
    m_pAddStuDialog = new MyDialog(this);
    initWidget();
    initTreeView();
    initTableView();

    connect(ui->stuTableView,&Widget::customContextMenuRequested,this,&Widget::stuTableMenuRequested);
    connect(ui->classTreeView,&Widget::customContextMenuRequested,this,&Widget::classViewMenuRequested);
    connect(ui->classTreeView,&QTreeView::clicked,this,&Widget::classViewClick);
    connect(m_pAddStuDialog,&MyDialog::setNewStudent,this,&Widget::addNewStudent);
}

Widget::~Widget()
{

    delete ui;

}


void Widget::initWidget()
{
    setWindowTitle(QStringLiteral("学生管理系统"));
    setWindowIcon(QIcon(":/icon/student.ico"));
    this->setMaximumSize(1000,800);
    this->setMinimumSize(1000,800);
    ui->stuTableView->setMaximumSize(690,800);
    ui->stuTableView->setMinimumSize(690,800);
    ui->classTreeView->setMinimumSize(290,800);
    ui->classTreeView->setMinimumSize(290,800);
}

void Widget::initTableView()
{

    m_pTableModel->setTable("student");
    m_pTableModel->setHeaderData(m_pTableModel->fieldIndex("id"),Qt::Horizontal,"学号");
    m_pTableModel->setHeaderData(m_pTableModel->fieldIndex("name"),Qt::Horizontal,"姓名");
    m_pTableModel->setHeaderData(m_pTableModel->fieldIndex("school"),Qt::Horizontal,"学校");
    m_pTableModel->setHeaderData(m_pTableModel->fieldIndex("class"),Qt::Horizontal,"班级");
    m_pTableModel->setHeaderData(m_pTableModel->fieldIndex("age"),Qt::Horizontal,"年龄");
    m_pTableModel->setHeaderData(m_pTableModel->fieldIndex("score"),Qt::Horizontal,"分数");
    ui->stuTableView->setModel(m_pTableModel);
    //右键菜单
    ui->stuTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    //无法编辑
    ui->stuTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //整行选中
    ui->stuTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_pTableModel->select();



}

void Widget::initTreeView()
{
       ui->classTreeView->setColumnHidden(0,50);
       m_pTreeModel->setHeaderData(0,Qt::Horizontal, tr("学校"));

       ui->classTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
       ui->classTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
       ui->classTreeView->setModel(m_pTreeModel);

       //初始化数据
       QMultiMap<QString,QString> classMap = m_stuData->getClassName();
       QList<QString> school;
       QList<QString> classes;
       //开始遍历
       QMutableMapIterator<QString, QString> i(classMap);
       while (i.hasNext())
       {
           if (i.next().key().endsWith(" ")) i.remove();
           if(school.contains(i.key()))
           {
               continue;
           }
           school.append(i.key());
           QStandardItem* itemSchools = new QStandardItem(i.key());
           itemSchools->setData(MARK_SCHOOL,ROLE_MARK_TYPE);
           m_pTreeModel->appendRow(itemSchools);
           classes  = classMap.values(i.key());
           QList<QStandardItem *>  itemClasses;
           for (int j = 0; j < classes.size(); j++)
           {
                QStandardItem *item = new QStandardItem(classes.at(j));
                item->setData(MARK_CLASS,ROLE_MARK_TYPE);
                itemClasses.push_back(item);
           }
           itemSchools->appendRows(itemClasses);
       }
}

void Widget::stuTableMenuRequested(const QPoint &point)
{

  //QModelIndex index = ui->classTreeView->indexAt(point);
    QModelIndex index = ui->stuTableView->currentIndex();
  //   qDebug() << ":"<< index.row() ;
     if (index.isValid())
    {
        QMenu menu;
        QAction *action_Del = new QAction("删除学生",this);
        menu.addAction(action_Del);
       //绑定槽函数
       connect(action_Del,&QAction::triggered,this,&Widget::delStuData);
       menu.exec(QCursor::pos());
    }
}

void Widget::delStuData()
{
    int row = ui->stuTableView->currentIndex().row();
    QMessageBox:: StandardButton result=  QMessageBox::warning(this, "提示", "确定删除？",
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    switch (result)
    {
    case QMessageBox::Yes:
        m_pTableModel->removeRow(row);
        qDebug()<<"确定删除" <<m_pTableModel->lastError();
        break;
    case QMessageBox::No:

        qDebug()<<"取消"<<m_pTableModel->lastError();
        break;
    default:
        break;
    }
    m_pTableModel->select();
}

void Widget::classViewMenuRequested(const QPoint &point)
{
          QModelIndex index = ui->classTreeView->indexAt(point);
          QMenu menu;
          if (index.isValid()) {
             if (index.data(ROLE_MARK_TYPE) == MARK_SCHOOL )
             {
                 QAction *action_AddClass = new QAction("新增班级",this); 
                 menu.addAction(action_AddClass);
                 connect(action_AddClass,&QAction::triggered,this,&Widget::addClassData);
             }else if(index.data(ROLE_MARK_TYPE) == MARK_CLASS)
             {
                 QAction *action_DelClass = new QAction("删除班级",this);
                 QAction *action_Add = new QAction("新增学生",this);
                 menu.addAction(action_DelClass);
                 menu.addAction(action_Add);
                 connect(action_DelClass,&QAction::triggered,this,&Widget::delClassData);
                 connect(action_Add,&QAction::triggered,this,&Widget::addStuDialog);
             }

        }else
        {
            QAction *action_displayAllClass = new QAction("显示全部",this);
             menu.addAction(action_displayAllClass);
             connect(action_displayAllClass,&QAction::triggered,this,&Widget::displayAllClassData);
        }
        menu.exec(QCursor::pos());
}

void Widget::addClassData()
{
    bool isOk;
    QString inputName = QInputDialog::getText(this, "新增班级",
                                                       "输入新增班级名称",
                                                       QLineEdit::Normal,
                                                       "班级名称",
                                                       &isOk);
    if(!isOk || inputName==tr(""))
    {
          QMessageBox::information(this, "提示", "新增班级名称不能为空",QMessageBox::Yes);
    }else
    {
        QModelIndex currentindex = ui->classTreeView->currentIndex();
        QStandardItem *currentItem= m_pTreeModel->itemFromIndex(currentindex);
        QStandardItem *itemChild = new QStandardItem(inputName);
        itemChild->setData(MARK_CLASS,ROLE_MARK_TYPE);
        currentItem->appendRow(itemChild);

        QString school =  ui->classTreeView->currentIndex().data().toString();
        qDebug() << inputName <<school;
        m_stuData->addClass(school,inputName);
    }
}

void Widget::delClassData()
{ 
    QModelIndex currentindex = ui->classTreeView->currentIndex();
    m_stuData->removeClass(currentindex.data().toString());

    QStandardItem *currentItem = m_pTreeModel->itemFromIndex(currentindex);
    QStandardItem *parent = currentItem->parent();
    parent->removeRow(currentindex.row());        //删除树节点

}

void Widget::displayAllClassData()
{
    for(int i=0;i<m_pTableModel->rowCount();i++)
        ui->stuTableView->setRowHidden(i,false);
}

void Widget::addStuDialog()
{
    //弹出输入对话框
    m_pAddStuDialog->show();
}

void Widget::classViewClick(const QModelIndex &index)
{
    QString str = index.data().toString();
    qDebug()<< str;
    fillterClass(str);
}

void Widget::fillterClass(QString name)
{
     name.remove(QRegExp("\\s"));
     for(int i=0;i< m_pTableModel->rowCount();i++)
     {
           ui->stuTableView->setRowHidden(i,true);
           QString r="";
           QAbstractItemModel *model=m_pTableModel;
           QModelIndex index;
           for(int j=0;j<m_pTableModel->columnCount();j++)
           {
               index=model->index(i,j);
               r+=model->data(index).toString();
           }
           r.remove(QRegExp("\\s"));
           if(r.contains(name,Qt::CaseSensitive))
              ui->stuTableView->setRowHidden(i,false);
     }
}

void Widget::addNewStudent(QString name,int age,int score)
{
    int id = m_pTableModel->rowCount()+1;
    QString className =  ui->classTreeView->currentIndex().data().toString();
    QString school    =  ui->classTreeView->currentIndex().parent().data().toString();
    qDebug()<< id << name <<  school << className  << age <<score;
    //获取空记录
    QSqlRecord record = m_pTableModel->record();
    record.setValue("id",id);
    record.setValue("name",name);
    record.setValue("school",school);
    record.setValue("class",className);
    record.setValue("age",age);
    record.setValue("score",score);
    m_pTableModel->insertRecord( m_pTableModel->rowCount(),record);

    qDebug() << "255"<<m_pTableModel->lastError().text();
    m_pTableModel->submitAll();
    initTableView();
}
