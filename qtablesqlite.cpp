#include "qtablesqlite.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QStringBuilder>
#include <QVariant>

QTableSQLite::Column::Column(const QString& name, Type type, bool isNull) :
    name(name), type(type), isNull(isNull)
{

}

QTableSQLite::QTableSQLite() : QAbstractSQLite() { }

QTableSQLite::QTableSQLite(const QString& table) :
    QAbstractSQLite(table)
{

}

QTableSQLite::QTableSQLite(const QString& table, const QString& filename) :
     QAbstractSQLite(table, filename)
{

}

bool QTableSQLite::createTable(const ColumnList& columns) {
    bool r = false;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            QStringList list;
            for (const Column& c : columns)
                list << "\"" % c.name % "\" " % type(c.type) % (c.isNull ? "":" NOT NULL");
            r = query->exec("CREATE TABLE IF NOT EXISTS \"" % table() % "\"(" % list.join(", ") % ");");
            if (!r) emit error(query->lastError().text());
            close(query);
        }
    }
    return r;
}

QSqlRecordList QTableSQLite::rowsData(const QString& filter, bool isDistinct) const {
    QSqlRecordList r;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            bool rst = query->exec("SELECT" % QString(isDistinct?" DISTINCT":"") % " * FROM \"" % table() % "\"" % (filter.isEmpty() ? QString("") : (" WHERE " % filter)) % ";");
            if (!rst) emit error(query->lastError().text());
            while (query->next())
                r << query->record();
            close(query);
        }
    }
    return r;
}

QSqlRecord QTableSQLite::rowData(const QString& filter, bool isDistinct) const {
    QSqlRecord r;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            bool rst = query->exec("SELECT" % QString(isDistinct?" DISTINCT":"") % " * FROM \"" % table() % "\" WHERE " % filter % ";");
            if (!rst) emit error(query->lastError().text());
            query->next();
            if (query->isValid())
                r = query->record();
            close(query);
        }
    }
    return r;
}

QStringList QTableSQLite::columnData(const QString& column, const QString& filter, bool isDistinct, bool ignoreEmpties) const {
    QStringList l;
    if (isValid()) {
        QSqlQuery * query = open();
        QString f = filter % (ignoreEmpties ? (QString(filter.isEmpty() ? "" : " && ") % "\"" % column % "\" IS NOT ''") : QString(""));
        if (query) {
            bool r = query->exec("SELECT" % QString(isDistinct ? " DISTINCT" : "") % " \"" % column % "\" FROM \"" % table() % "\"" % (f.isEmpty() ? QString("") : " WHERE " % f) % " ORDER BY \"" % column % "\" ASC;");
            if (!r) emit error(query->lastError().text());
            while (query->next())
                l << query->value(0).toString();
            close(query);
        }
    }
    return l;
}

bool QTableSQLite::contains(const QString& column, const QString& value) const {
    bool r = false;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            bool rst = query->exec("SELECT DISTINCT \"" % column % "\" FROM \"" % table() % "\" WHERE \"" % column % "\"='" % value % "';");
            if (!rst) emit error(query->lastError().text());
            while (query->next() && !r) r = true;
            close(query);
        }
    }
    return r;
}

bool QTableSQLite::insertRow(const QStringList& list) {
    bool r = false;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            QStringList l;
            for (const QString& s : list) l << QString("'" % s % "'");
            r = query->exec("INSERT INTO \"" % table() % "\" VALUES (" % l.join(", ") % ");");
            if (!r) emit error(query->lastError().text());
            close(query);
        }
    }
    return r;
}

bool QTableSQLite::updateRow(const QStringList& columns, const QStringList& values, const QString& filter) {
    bool r = false;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            QStringList l;
            auto citer = columns.begin();
            auto viter = values.begin();
            for (; citer != columns.end() && viter != values.end(); ++citer, ++viter)
                l << "\"" % *citer % "\"='" % *viter % "'";
            r = query->exec("UPDATE \"" % table() % "\" SET " % l.join(", ") % " WHERE " % filter % ";");
            if (!r) emit error(query->lastError().text());
            close(query);
        }
    }
    return r;
}

bool QTableSQLite::deleteData(const QString& filter) {
    bool r = false;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            r = query->exec("DELETE FROM \"" % table() % "\" WHERE " % filter % ";");
            if (!r) emit error(query->lastError().text());
            close(query);
        }
    }
    return r;
}

QString QTableSQLite::type(Type t) const {
    switch(t) {
    case Integer: return "INTEGER";
    case Float: return "FLOAT";
    case DateTime: return "DATETIME";
    case Varchar: return "VARCHAR";
    }
    return QString();
}
