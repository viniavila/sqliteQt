#ifndef QTREESQLITE_H
#define QTREESQLITE_H

#include "sqliteqt_global.h"
#include "qabstractsqlite.h"
#include <QDateTime>

class QTreeSQLite : public QAbstractSQLite {
    Q_OBJECT
public:
    struct Node {
        QString id;
        QDateTime timestamp;
        QString value;
        QString parentID;
        QStringList childrenID;
    };
    typedef QList<Node> NodeList;

    QTreeSQLite();
    QTreeSQLite(const QString& table);
    QTreeSQLite(const QString& table, const QString& filename);

    bool createTable();
    Node node(const QString& id) const;
    NodeList children(const QString& parentID = QString()) const;
    bool insertNode(const QString& value, const QString& parentID = QString());
    bool updateNode(const QString& newValue, const QString& id);
    bool removeNode(const QString& id);
    bool contains(const QString& value) const;
    bool isChild(const QString& id, const QString& childID) const;
    bool isDescendant(const QString& id, const QString& descendantID) const;
    bool hasChild(const QString& id, const QString& childValue) const;

    static bool nodeIsValid(const Node& node);

protected:
    QStringList childrenID(const QString& id) const;
    QString parentID(const QString& id) const;
    QStringList hierarchy(const QString& id) const;
};

#endif // QTREESQLITE_H
