#ifndef QEXTENSIONS_H
#define QEXTENSIONS_H

#include <QObject>
#include <QXmlReader>
#include <QDomDocument>
#include <QDir>
#include <QStringList>

class QExtensions : public QObject
{
    Q_OBJECT
public:
    explicit QExtensions(QObject *parent = 0);
    void load(QString fileName);
    void print(QString theTitle);
    QStringList list;
signals:

public slots:

};

#endif // QEXTENSIONS_H
