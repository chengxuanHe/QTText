#include "StuDataSql.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
StuDataSql::StuDataSql(QObject *parent) : QObject(parent)
{
      initDatabase();
//     createTable();
//      createClass();

}

StuDataSql::~StuDataSql()
{
     m_db.close();
}

//初始化数据库
void StuDataSql::initDatabase()
{
    //查看可配置的数据库
    //qDebug()<< QSqlDatabase::drivers();

    //  使用SQLITE
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    //设置数据库
    m_db.setDatabaseName("../Student.db");

    //打开数据库
    if(!m_db.open())   //数据库打开失败
    {
       qDebug()<< m_db.lastError().text();
       return;
    }

}


//创建学生表  并初始化数据
void StuDataSql::createTable()
{
    QSqlQuery query(m_db);
    //创建学生表
    //学生(学号(主键)，姓名，学校，班级，年龄，分数)
    query.exec("create table student(id int primary key,name varchar(255),school varchar(255),class varchar(255),age int,score int);");
    query.exec("insert into  student(id,name,school,class,age,score) values (1,'神威马超','蜀汉学院','五虎班',18,59);");
    query.exec("insert into  student(id,name,school,class,age,score) values (2,'龙胆赵云','蜀汉学院','五虎班',20,60);");
    query.exec("insert into  student(id,name,school,class,age,score) values (3,'虎魄张飞','蜀汉学院','五虎班',21,78);");
    query.exec("insert into  student(id,name,school,class,age,score) values (4,'烈魂黄忠','蜀汉学院','五虎班',21,78);");
    query.exec("insert into  student(id,name,school,class,age,score) values (5,'武圣关羽','蜀汉学院','五虎班',21,78);");
    query.exec("insert into  student(id,name,school,class,age,score) values (6,'司马懿','魏都学院','军师班',67,98);");

}

//返回数据库文件
QSqlDatabase StuDataSql::getDatabase()
{
    return  m_db;
}

//创建班级表 并初始化数据
void StuDataSql::createClass()
{
    QSqlQuery query(m_db);
    //创建班级表
    //班级(班号(主键)，班级名称,学校)
    query.exec("create table class(id int primary key,class varchar(255),school varchar(255),time  varchar(255));");
    query.exec("insert into  class(id,class,school,time) values (1,'五虎班','蜀汉学院','三国');");
    query.exec("insert into  class(id,class,school,time) values (2,'丞相班','蜀汉学院','三国');");
    query.exec("insert into  class(id,class,school,time) values (3,'军师班','魏都学院','三国');");
    query.exec("insert into  class(id,class,school,time) values (4,'武将班','魏都学院','三国');");
    bool isOk = query.exec("insert into  class(id,class,school,time) values (5,'都督班','东吴学院','三国');");
     qDebug() << isOk ;
}

//查询所有班级
void StuDataSql::selectClass()
{
    QSqlQuery query(m_db);
    query.exec("select * from class;");
    QString school,className;

      while(query.next())  //遍历一行一行
      {
          //取出当前行的内容
         school = query.value("school").toString();
         className = query.value("class").toString();
         m_classMap.insert(school,className);

      }

}

//获取班级树
QMultiMap<QString,QString> StuDataSql::getClassName()
{
    selectClass();
    return  m_classMap;
}

//增加某班
void StuDataSql::addClass(QString school,QString className)
{
    QSqlQuery query(m_db);
    query.prepare("insert into class(id,class,school,time) values (?,?,?,'三国');");
    query.addBindValue(m_classMap.size()+1);
    query.addBindValue(className);
    query.addBindValue(school);
    query.exec();
    qDebug()<<"add:" <<query.lastError();

}

//删除某班
void StuDataSql::removeClass(QString className)
{
    QSqlQuery query(m_db);
    query.prepare("delete from class where class = (?);");
    query.addBindValue(className);
    query.exec();
    //qDebug()<<"del:" <<query.lastError();
}




