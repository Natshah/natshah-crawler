#ifndef configurations_H
#define configurations_H

#include <QObject>
#include <QXmlReader>
#include <QDomDocument>
#include <QDir>

class configurations : public QObject
{
    Q_OBJECT
public:
    explicit configurations(QObject *parent = 0);

    void load(QString fileName);

signals:

public slots:

public:

    QString databaseType;
    QString hostName;
    QString databaseName;
    QString userName;
    QString password;
    QString port;
    QString prefix;
    QString urlsTableName;
    QString domainsTableName;
    QString emailsTableName;

    int WAITE_TIME;
    int PROGRESS_PAR_SHOW_TIME;
};

#endif // configurations_H
