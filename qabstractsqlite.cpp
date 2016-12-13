#include "qabstractsqlite.h"
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringBuilder>
#include <QVariant>

#define HEX_NUMBERS "0123456789abcdef"
#define HEX_SIZE 16

class QAbstractSQLitePrivate {
    Q_DECLARE_PUBLIC(QAbstractSQLite)
public:
    QAbstractSQLitePrivate(QAbstractSQLite * parent) :
        q_ptr(parent),
        connection("__connection__" % parent->id() % "__"),
        db(QSqlDatabase::addDatabase("QSQLITE", connection)),
        queryCounter(0)
    {
    }

    QAbstractSQLite * const q_ptr;
    QString table;
    QString filename;
    QString connection;
    QSqlDatabase db;
    int queryCounter;
};


QAbstractSQLite::QAbstractSQLite() :
    QObject(0),
    d_ptr(new QAbstractSQLitePrivate(this))
{
}

QAbstractSQLite::QAbstractSQLite(const QString& table) :
    QObject(0),
    d_ptr(new QAbstractSQLitePrivate(this))
{
    Q_D(QAbstractSQLite);
    d->table = table;
}

QAbstractSQLite::QAbstractSQLite(const QString& table, const QString& filename) :
    QObject(0),
    d_ptr(new QAbstractSQLitePrivate(this))
{
    Q_D(QAbstractSQLite);
    d->table = table;
    d->filename = filename;
    d->db.setDatabaseName(filename);
}

QAbstractSQLite::~QAbstractSQLite() {
    Q_D(QAbstractSQLite);
    QString connection = d->connection;
    delete d_ptr;
    QSqlDatabase::removeDatabase(connection);
}

void QAbstractSQLite::setFilename(const QString& filename) {
    Q_D(QAbstractSQLite);
    if (d->filename != filename) {
        d->filename = filename;
        if (d->db.isOpen()) d->db.close();
        d->db.setDatabaseName(filename);
    }
}

void QAbstractSQLite::setTable(const QString& table) {
    Q_D(QAbstractSQLite);
    d->table = table;
}

QString QAbstractSQLite::filename() const {
    Q_D(const QAbstractSQLite);
    return d->filename;
}

QString QAbstractSQLite::table() const {
    Q_D(const QAbstractSQLite);
    return d->table;
}

bool QAbstractSQLite::isValid() const {
    Q_D(const QAbstractSQLite);
    return !(d->table.isEmpty() || d->filename.isEmpty());
}

bool QAbstractSQLite::tableExists() const {
    Q_D(const QAbstractSQLite);
    QSqlQuery *query;
    if ((query = open())) {
        QString t;
        bool r = query->exec("SELECT name FROM sqlite_master WHERE type='table' AND name='" % d->table % "';");
        if (!r) emit error(query->lastError().text());
        while (query->next()) t = query->value(0).toString();
        close(query);
        return !t.isEmpty();
    }
    return false;
}

QSqlQuery* QAbstractSQLite::open() const {
    QSqlQuery * query = 0;
    if (d_ptr->db.isOpen() || d_ptr->db.open()) {
        query = new QSqlQuery(d_ptr->db);
        d_ptr->queryCounter++;
    }
    return query;
}

QSqlQuery* QAbstractSQLite::close(QSqlQuery* query) const {
    if (d_ptr->db.isOpen()) {
        if (query) {
            query->finish();
            delete query;
            query = 0;
            d_ptr->queryCounter--;
        }
        if (!d_ptr->queryCounter)
            d_ptr->db.close();
    }
    return query;
}

QString QAbstractSQLite::id() const {
    char * c = (char*)malloc(33*sizeof(char));
    char * pc = c;
    for (short i=0; i<32; ++i,++pc) {
        if (i == 8 || i == 13 || i == 18 || i == 23) *pc = '-';
        else *pc = HEX_NUMBERS[qrand() % HEX_SIZE];
    }
    *pc = '\0';
    QString result(c);
    free(c);
    return result;
}

QString QAbstractSQLite::timestamp() const {
    return QDateTime::currentDateTimeUtc().toString(timestampFormat());
}

QString QAbstractSQLite::timestampFormat() const {
    return "yyyy-MM-dd HH:mm:ss t";
}
