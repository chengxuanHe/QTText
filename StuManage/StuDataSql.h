#ifndef STUDATASQL_H
#define STUDATASQL_H
#include <QSqlDatabase>
#include <QObject>
#include <QMultiMap>
class StuDataSql : public QObject
{
    Q_OBJECT
public:
    explicit StuDataSql(QObject *parent = nullptr);
    ~StuDataSql();
    QSqlDatabase getDatabase();
    QMultiMap<QString,QString> getClassName();
    void  addClass(QString school,QString className);
    void  removeClass(QString className);



public slots:

private:
    void     initDatabase();
    void     createTable();
    void     createClass();
    void     selectClass();

    QSqlDatabase m_db;
    QMultiMap<QString,QString> m_classMap;

};

#endif // STUDATASQL_H
