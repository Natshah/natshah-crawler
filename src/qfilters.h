#ifndef QFILTERS_H
#define QFILTERS_H

#include <QObject>
#include <QXmlReader>
#include <QDomDocument>
#include <QDir>
#include <QStringList>

class QFilters : public QObject
{
    Q_OBJECT
public:
    explicit QFilters(QObject *parent = 0);
    void load(QString fileName);
    void print(QString theTitle);
    QStringList list;

signals:

public slots:

};

#endif // QFILTERS_H
