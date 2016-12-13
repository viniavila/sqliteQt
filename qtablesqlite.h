#ifndef QTABLESQLITE_H
#define QTABLESQLITE_H

#include "sqliteqt_global.h"
#include "qabstractsqlite.h"
#include <QSqlRecord>

typedef QList<QSqlRecord> QSqlRecordList;

class SQLITEQTSHARED_EXPORT QTableSQLite : public QAbstractSQLite {
    Q_OBJECT
public:
    enum Type {
        Integer,
        Float,
        DateTime,
        Varchar
    };

    struct Column {
        Column() {}
        Column(const QString& name, Type type, bool isNull = false);
        QString name;
        Type type;
        bool isNull;
    };
    typedef QList<Column> ColumnList;

    QTableSQLite();
    QTableSQLite(const QString& table);
    QTableSQLite(const QString& table, const QString& filename);

    bool createTable(const ColumnList& columns);
    QSqlRecordList rowsData(const QString& filter, bool isDistinct = true) const;
    QSqlRecord rowData(const QString& filter, bool isDistinct = true) const;
    QStringList columnData(const QString& column, const QString& filter = QString(), bool isDistinct = true, bool ignoreEmpties = true) const;
    bool contains(const QString& column, const QString& value) const;
    bool insertRow(const QStringList& list);
    bool updateRow(const QStringList& columns, const QStringList& values, const QString& filter);
    bool deleteData(const QString& filter);

protected:
    QString type(Type t) const;

};

#endif // QTABLESQLITE_H
