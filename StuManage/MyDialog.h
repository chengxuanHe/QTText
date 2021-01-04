#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QObject>
#include <QMultiMap>
#include "StuDataSql.h"

namespace Ui {
class MyDialog;
}

class MyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyDialog(QWidget *parent = nullptr);
    ~MyDialog();
    void initDialog();
    void initTableWidget();
    void confirmData();
    void cancelData();


signals:
    void  setNewStudent(QString name,int age,int score);

private:
    Ui::MyDialog *ui;
    QMultiMap<QString,QString> m_schoolData;

};

#endif // MYDIALOG_H
