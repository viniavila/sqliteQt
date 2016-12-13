#ifndef QABSTRACTSQLITE_H
#define QABSTRACTSQLITE_H

#include <QObject>

class QSqlQuery;
class QAbstractSQLitePrivate;
class QAbstractSQLite : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAbstractSQLite)
public:
    QAbstractSQLite();
    QAbstractSQLite(const QString& table);
    QAbstractSQLite(const QString& table, const QString& filename);
    virtual ~QAbstractSQLite();
    void setFilename(const QString& filename);
    void setTable(const QString& table);
    QString filename() const;
    QString table() const;
    bool isValid() const;
    bool tableExists() const;

signals:
    void error(const QString& message) const;

protected:
    QSqlQuery* open() const;
    QSqlQuery* close(QSqlQuery*) const;

    virtual QString id() const;
    virtual QString timestamp() const;
    virtual QString timestampFormat() const;

private:
    QAbstractSQLite(const QAbstractSQLite&);
    QAbstractSQLite& operator=(const QAbstractSQLite&);
    QAbstractSQLitePrivate * const d_ptr;
};

#endif // QABSTRACTSQLITE_H
