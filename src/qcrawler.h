#ifndef QCRAWLER_H
#define QCRAWLER_H


#include <QObject>
#include <QSemaphore>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QtSql>
#include <QDir>
#include <QtNetwork>
#include <QTextStream>
#include <QStringList>
#include <QTimer>

#include "configurations.h"
#include "qfilters.h"
#include "qextensions.h"

class QFile;
class QHttp;
class QHttpResponseHeader;

class QSslError;
class QAuthenticator;
class QNetworkReply;
class QStringList;


class QCrawler : public QObject
{
    Q_OBJECT
public:
    explicit QCrawler(QObject *parent = 0);



    void load();
    bool this_URL_in_the_database(QString theURL);
    bool this_domain_in_the_database(QString theDomain);
    int getURLID(QString theURL);
    void addSubURLs(QString theURL);
    void getHTML(QString theURL);

    QString getFirstNotCrawledURL();
    QString getFirstNotDoneURL();

    QString PathConvert( QString path );
    void getSubURLS(QString theURL);
    bool URL_is_HTML(QString theURL);

    bool isURL(QString theURL);
    bool isDoneURL(QString theURL);
    bool isThisURLCrawled(QString theURL);

    void deleteAllURLs();
    void deleteAllDomains();
    void deleteAllEmails();

    void addDomain(QString theDomain);
    void addNewURL(QString theURL);
    QString getURLData(QString theURL);

     void setURLAsCrawled(QString theURL);
     void setURLAsDone(QString theURL);
     bool filterd( QString theURLLink);

    long int getNumberOfURLs();
    long int getNumberOfCrawledURLs();
    long int getNumberOfDoneURLs();
    long int getNumberOfArchivedURLs();
    long int getNumberOfEmptyDataURLs();




    void download_the_HTML_code_for_the_URL_in_a_file(QString theURL);
    void read_the_HTML_data_from_the_file();
    void save_the_HTML_data_in_the_database(QString theURL);

    void beforeContinue();

    bool createConnection();
    void printHelp();
    void printConfig();
    void doDelete();
    void doDeleteBeforeStartCrawling();
    void printTotlaInfo();

    void printFilters();
    void printExtensions();

    QString htmlData;
    QString rootURL;

    configurations config;
    QFilters filters;
    QExtensions extensions;
    QSqlDatabase db;

signals:

public slots:
    void cancelDownload();
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);
    void slotAuthenticationRequired(const QString &, quint16, QAuthenticator *);
    void progress();

#ifndef QT_NO_OPENSSL
    void sslErrors(const QList<QSslError> &errors);
#endif
private:
    QHttp *http;
    QFile *file;
    QTimer *timer;
    int httpGetId;
    bool httpRequestAborted;
    bool notFinieshed;
    bool notCopied;
    bool notSaved;
    QString myFileName;
    QSemaphore still;
    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;

    QTimer *timerGLOSURL;

};


#endif // QCRAWLER_H
