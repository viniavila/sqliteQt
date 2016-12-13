#include "qtreesqlite.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QStringBuilder>
#include <QVariant>

QTreeSQLite::QTreeSQLite() : QAbstractSQLite() { }

QTreeSQLite::QTreeSQLite(const QString& table) :
    QAbstractSQLite(table)
{

}

QTreeSQLite::QTreeSQLite(const QString& table, const QString& filename) :
    QAbstractSQLite(table, filename)
{

}

bool QTreeSQLite::nodeIsValid(const Node& node) {
    bool r = node.id.isEmpty() || !node.timestamp.isValid() || node.value.isEmpty();
    return !r;
}

bool QTreeSQLite::createTable() {
    bool r = false;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            r = query->exec("CREATE TABLE IF NOT EXISTS \"" % table() % "\"(\"ID\" VARCHAR NOT NULL, \"TIMESTAMP\" DATETIME NOT NULL, \"VALUE\" VARCHAR NOT NULL, \"PARENT\" VARCHAR NOT NULL, \"HIERARCHY\" VARCHAR NOT NULL);");
            if (!r) emit error(query->lastError().text());
            close(query);
        }
    }
    return r;
}


QTreeSQLite::Node QTreeSQLite::node(const QString& id) const {
    Node n;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            bool r = query->exec("SELECT \"TIMESTAMP\",\"VALUE\",\"PARENT\" FROM \"" % table() % "\" WHERE \"ID\"='" % id % "';");
            if (!r) emit error(query->lastError().text());
            while (query->next()) {
                n.id = id;
                n.timestamp = QDateTime::fromString(query->value(0).toString(), timestampFormat());
                n.value = query->value(1).toString();
                n.parentID = query->value(2).toString();
            }
            close(query);
            n.childrenID = childrenID(id);
        }
    }
    return n;
}

QTreeSQLite::NodeList QTreeSQLite::children(const QString& parentID) const {
    QStringList cid = childrenID(parentID);
    NodeList l;
    for (const QString& id : cid)
        l << node(id);
    return l;
}

bool QTreeSQLite::insertNode(const QString& value, const QString& parentID) {
    bool r = false;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            r = query->exec("INSERT INTO \"" % table() % "\" VALUES ('" % id() % "','" % timestamp() % "','" % value % "','" % parentID % "', '" % hierarchy(parentID).join("/") % "/" % parentID % "')");
            if (!r) emit error(query->lastError().text());
            close(query);
        }
    }
    return r;
}

bool QTreeSQLite::updateNode(const QString& newValue, const QString& id) {
    bool r = false;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            r = query->exec("UPDATE \"" % table() % "\" SET \"VALUE\"='" % newValue % "' WHERE \"ID\"='" % id % "';");
            if (!r) emit error(query->lastError().text());
            close(query);
        }
    }
    return r;
}

bool QTreeSQLite::removeNode(const QString& id) {
    bool r = false;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            r = query->exec("DELETE FROM \"" % table() % "\" WHERE \"ID\"='" % id % "';");
            if (!r) emit error(query->lastError().text());
            close(query);
            QStringList cids = childrenID(id);
            for (const QString& cid : cids)
                r |= removeNode(cid);
        }
    }
    return r;
}

bool QTreeSQLite::contains(const QString& value) const {
    bool r = false;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            bool rst = query->exec("SELECT DISTINCT \"VALUE\" FROM \"" % table() % "\" WHERE \"VALUE\"='" % value % "';");
            if (!rst) emit error(query->lastError().text());
            while (query->next() && !r) r = true;
            close(query);
        }
    }
    return r;
}

bool QTreeSQLite::isChild(const QString& id, const QString& childID) const {
    bool r = false;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            bool rst = query->exec("SELECT DISTINCT * FROM \"" % table() % "\" WHERE \"ID\"='" % childID % "' && \"PARENT\"='" % id % "';");
            if (!rst) emit error(query->lastError().text());
            while (query->next() && !r) r = true;
            close(query);
        }
    }
    return r;
}

bool QTreeSQLite::isDescendant(const QString& id, const QString& descendantID) const {
    bool r = false;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            bool rst = query->exec("SELECT DISTINCT * FROM \"" % table() % "\" WHERE \"ID\"='" % descendantID % "' && \"HIERARCHY\" LIKE '%" % id % "%';");
            if (!rst) emit error(query->lastError().text());
            while (query->next() && !r) r = true;
            close(query);
        }
    }
    return r;
}

bool QTreeSQLite::hasChild(const QString& id, const QString& childValue) const {
    bool r = false;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            bool rst = query->exec("SELECT DISTINCT * FROM \"" % table() % "\" WHERE \"PARENT\"='" % id % "' && \"VALUE\"='" % childValue % "';");
            if (!rst) emit error(query->lastError().text());
            while (query->next() && !r) r = true;
            close(query);
        }
    }
    return r;
}

QStringList QTreeSQLite::childrenID(const QString& id) const {
    QStringList l;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            bool r = query->exec("SELECT \"ID\" FROM \"" % table() % "\" WHERE \"PARENT\"='" % id % "';");
            if (!r) emit error(query->lastError().text());
            while (query->next())
                l << query->value(0).toString();
            close(query);
        }
    }
    return l;
}

QString QTreeSQLite::parentID(const QString& id) const {
    QString p;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            bool r = query->exec("SELECT \"PARENT\" FROM \"" % table() % "\" WHERE \"ID\"='" % id % "';");
            if (!r) emit error(query->lastError().text());
            while (query->next())
                p = query->value(0).toString();
            close(query);
        }
    }
    return p;
}

QStringList QTreeSQLite::hierarchy(const QString& id) const {
    QStringList h;
    if (isValid()) {
        QSqlQuery * query = open();
        if (query) {
            bool r = query->exec("SELECT \"HIERARCHY\" FROM \"" % table() % "\" WHERE \"ID\"='" % id % "';");
            if (!r) emit error(query->lastError().text());
            while (query->next())
                h = query->value(0).toString().split("/");
            close(query);
        }
    }
    return h;
}
