### Welcome to SqliteQt module

This module was created to make it easier and more intuitive to connect to SQLite database files, without having to know the great details of SQL syntax. The module is basically composed of three classes, described below.

* **QAbstractSQLite:**  is an abstract class, not in the sense of having one or more `pure virtual` members, but in the sense that it is not very useful by itself, since it has no members to directly access the database. It is recommended to derive this class and implement these members yourself. To access the database, use the `open()` `protected` member, which opens the database and returns a `QSqlQuery` pointer for manipulation. After use, use the `close()` member to end the active query, close the database, and delete the pointer.

* **QTableSQLite:** derived from the `QAbstractSQLite` class, has members deployed to treat data from a SQLite database as tables. That is, an instance of this class corresponds to a table inside a database file. There are members to fetch stored values, insert rows, edit, remove data, or even check for a given value in a particular column. Although the number of options in database manipulation is limited, this class provides a basic database usage without having to know absolutely nothing about SQL syntax.

* **QTreeSQLite:** derived from the `QAbstractSQLite` class, has members deployed to treat data from a SQLite database as a tree. That is, an instance of this class corresponds to a table inside a database file and the values are organized as a hierarchy, where certain values are "children" of other values. Internally a simple model of _Adjacency Lists_ is implemented. There are members to fetch the stored values, insert items, edit, remove items (and all their children) or even check if there is a certain value in the children of an item. Although the number of options in database manipulation is limited, this class provides a basic database usage without having to know absolutely nothing about SQL syntax.

### Installation

You can choose either of two options below:

* Embbed the code into your application. You can download my source code and import it through Qt Creator or whatever source code editor you are using. You have to import at least qabstractsqlite.cpp and qabstractsqlite.h to have minimal support. if you want to use QTableSQLite and/or QTreeSQLite, download and import their files too.

* Compile the code as a library. Just clone my repository using `git clone https://github.com/viniavila/sqliteQt.git`. Enter into the cloned folder and just run these commands:

```
$ qmake sqliteQt.pro
$ make
$ sudo make install
```

The libsqliteQt.so.1.0.0 and its symbolic links, libsqliteQt.so.1.0, libsqliteQt.so.1 and libsqliteQt.so, will be copied to /usr/lib. You can pass PROFILE="new path" to qmake command to change this default path. To use the library, copy all the *.h files to /usr/include or /usr/local/include, or wherever path you want (since you provide this path in your project file, through INCLUDEPATH). To link your project to the .so library, uses the follow:

```
-L"/library_path" -lsqliteQt
```

My code is licensed in LGPL v3 so you can use it either in open source or closed source applications since it concerns with the terms of this license. If you are using my code into your project and finds it useful, do not forget to mention my name in the credits, and if it is worthy, donation can be accepted.

Many thanks for using this code!

## Authors and Contributors
Vinícius de Ávila Jorge (@viniavila). You can contact me through my GitHub profile, or if you prefer, [email](mailto:vinicius.avila.jorge@gmail.com) me.

