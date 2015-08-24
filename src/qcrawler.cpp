#include "qcrawler.h"
#include <iostream>
using namespace std;


/*************************************************************************************************
 *  the Constracter for the Class QCrawler
 *  @parent : the Object Parent link.
 *************************************************************************************************/
QCrawler::QCrawler(QObject *parent) :
    QObject(parent)
{
    http = new QHttp(this);
    timer = new QTimer(this);
    timerGLOSURL = new QTimer(this);
    notFinieshed = false;

     connect(http, SIGNAL(requestFinished(int, bool)),
             this, SLOT(httpRequestFinished(int, bool)));
     connect(http, SIGNAL(dataReadProgress(int, int)),
             this, SLOT(updateDataReadProgress(int, int)));
     connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
             this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
     connect(http, SIGNAL(authenticationRequired(const QString &, quint16, QAuthenticator *)),
             this, SLOT(slotAuthenticationRequired(const QString &, quint16, QAuthenticator *)));
 #ifndef QT_NO_OPENSSL
     connect(http, SIGNAL(sslErrors(const QList<QSslError> &)),this, SLOT(sslErrors(const QList<QSslError> &)));
 #endif
     connect(timer, SIGNAL(timeout()), this, SLOT(cancelDownload()));
     connect(timerGLOSURL, SIGNAL(timeout()), this, SLOT(progress()));
     htmlData = "";
}


/*************************************************************************************************
 *  Load All Crawler Stttings .
 *************************************************************************************************/
void QCrawler::load()
{
    // Load Database configurations.
    config.load("configurations.xml");

    // Load URL Filters.
    filters.load("filters.xml");

    // Load file extensions.
    extensions.load("extensions.xml");

    cout <<"Crawler Settings have been loaded\n\n";

}


/*************************************************************************************************
 *  Download the HTML code for the URL in a file.
 *  @theURL : the Link of the URL as String
 *************************************************************************************************/
void QCrawler::download_the_HTML_code_for_the_URL_in_a_file(QString theURL)
{
    getHTML(theURL);
    this->notFinieshed = true;
    timer->start(config.WAITE_TIME);
      while( notFinieshed )
       {
          QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
          cout << "\r\tCrawleing\t\t\t\t\t[  ////  ]";
          cout << "\r\tCrawleing\t\t\t\t\t[  \\\\\\\\  ]";
          cout << "\r\tCrawleing\t\t\t\t\t[  ||||  ]";
       }
     timer->stop();
}

/*************************************************************************************************
 *  Read the HTML data from the File
 *************************************************************************************************/
void QCrawler::read_the_HTML_data_from_the_file()
{
    QDir::setCurrent(":/");
        QString FileData ="";
        if (QFile::exists("data.xml"))
        {
            QFile htmlfile("data.xml");
            if (!htmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
                htmlData = "E" ;

            QTextStream in(&htmlfile);
            while (!in.atEnd()) {
                QString line = in.readLine();
                FileData += line;
            }
            htmlfile.close();
        }
    htmlData= FileData;
}

/*************************************************************************************************
 *  save the HTML Data in the Database
 *  @theURL : the Link of the URL as String.
 *  @HTMLData : the HTML Data for the URL.
 *************************************************************************************************/
void QCrawler::save_the_HTML_data_in_the_database(QString theURL)
{
     QSqlQuery query;
     query.prepare("UPDATE "+config.prefix+config.urlsTableName+" SET data = :htmlData WHERE url = '"+theURL+"' ");
     query.bindValue(":htmlData",htmlData);

     query.exec();
     if (!query.isActive())
         cout << "\n\tDBE - QCrawler::save_the_HTML_data_in_the_database : " << query.lastError().text().toStdString() <<"\n";
}


 /*************************************************************************************************
  *  add Sub URLs to the Database
  *  @theURL : the Link of the URL as String
  *************************************************************************************************/
void QCrawler::addSubURLs(QString theURL)
{
        // Get List of Sub URLs for this
        // --------------------------------------------------------------
            cout << "\tGLOSURL";
            getSubURLS(theURL);
            cout << "\r\tGLOSURL\t\t\t\t\t\t[  Done  ]\n";
        // --------------------------------------------------------------


        // Go for all Sub URLs for this URL
        // --------------------------------------------------------------



                QString FirstNotCrawledURL = this->getFirstNotCrawledURL();
                while(FirstNotCrawledURL!="")
                {
                        cout <<"\n\n\n\t[*]- "<< FirstNotCrawledURL.toStdString()<<"\n" ;

                            cout << "\r\tCrawleing Start\t     ";
                            download_the_HTML_code_for_the_URL_in_a_file(FirstNotCrawledURL);
                            cout <<"\r\tCrawleing\t\t\t\t\t[  Done  ]\n";

                            cout << "\tRTHTMLDFTF  Start";
                            read_the_HTML_data_from_the_file() ;
                            cout << "\r\tRTHTMLDFTF\t\t\t\t\t[  Done  ]\n";

                            cout << "\tSTHTMLDITD Start";
                            save_the_HTML_data_in_the_database(FirstNotCrawledURL);
                            cout << "\r\tSTHTMLDITD\t\t\t\t\t[  Done  ]\n";

                    // Set Crawled to True.
                    setURLAsCrawled(FirstNotCrawledURL);



                    FirstNotCrawledURL = getFirstNotCrawledURL();
                }

                // Go throw All Sub URLs and Add there Sub URLs.
                 QString FirstNotDoneURL = this->getFirstNotDoneURL();
                 while( FirstNotDoneURL !="" )
                 {
                        cout <<"\n\n\n\t[*]- "<< FirstNotDoneURL.toStdString()<<"\n" ;
                        this->setURLAsDone(FirstNotDoneURL);
                        addSubURLs(FirstNotDoneURL);
                        FirstNotDoneURL = this->getFirstNotDoneURL();
                 }




}


/*************************************************************************************************
 * If the URL is HTML Code and not other type of data.
 * @theURL : the URL link as String.
 *************************************************************************************************/
void QCrawler::getSubURLS(QString theURL)
{

    QString htmlData = getURLData(theURL);
    QStringList afterHref  = htmlData.split("href",QString::SkipEmptyParts,Qt::CaseInsensitive);


    afterHref.replaceInStrings("=","");
    afterHref.replaceInStrings("\'","\"");
    afterHref.replaceInStrings("&quot;","\"");
    afterHref.replaceInStrings(".../","/");
    afterHref.replaceInStrings("../","/");
    afterHref.replaceInStrings("./","/");
    afterHref.replaceInStrings(" ","");


    cout << "\n== START SUB URLs FORE========================================";
    cout << "\n   + "<<theURL.toStdString() ;
    cout << "\n==============================================================";

   for(int i=1; i<afterHref.size(); i++)
     {
        QStringList afterDobuleCotation;
        afterDobuleCotation = afterHref[i].split("\"",QString::SkipEmptyParts,Qt::CaseSensitive);
        QString urlLink = afterDobuleCotation[0];
        QString finalURL = "";

        if (URL_is_HTML(urlLink) && (urlLink != theURL) && (urlLink != theURL+"/") && (urlLink != rootURL) && (urlLink != rootURL+"/") )
        {
            if (urlLink.startsWith("/") )
                finalURL  = rootURL + urlLink;
            else if( !urlLink.startsWith("/") && filterd(urlLink))
                finalURL  = rootURL+"/"+urlLink;
            else if (urlLink.startsWith(rootURL,Qt::CaseInsensitive)  && !urlLink.startsWith(rootURL+rootURL,Qt::CaseInsensitive) && !urlLink.startsWith(rootURL+"javascript:",Qt::CaseInsensitive) && !urlLink.startsWith(rootURL+"mailto:" ,Qt::CaseInsensitive) && !urlLink.startsWith(rootURL+"http://" ,Qt::CaseInsensitive) && !urlLink.startsWith(rootURL+"https://" ,Qt::CaseInsensitive) && !urlLink.startsWith(rootURL+"ftp://" ,Qt::CaseInsensitive) && !urlLink.startsWith(rootURL+"file://" ,Qt::CaseInsensitive) )
                finalURL  = urlLink;

            if (!this_URL_in_the_database(finalURL) && finalURL !="" && !finalURL.startsWith(" ") )
            {
              this->addNewURL(finalURL);
              cout << "\n\t\t"<<finalURL.toStdString() ;
            }
        }



        if (i%2 == 0) cout << "\r\tGLOSURL\t\t\t\t\t\t[  ////  ]";
        if (i%3 == 0) cout << "\r\tGLOSURL\t\t\t\t\t\t[  \\\\\\\\  ]";
        if (i%4 == 0) cout << "\r\tGLOSURL\t\t\t\t\t\t[  ||||  ]";
    }

   cout << "\n== End SUB URLs FORE==========================================";
   cout << "\n   + "<<theURL.toStdString() ;
   cout << "\n==============================================================\n";

}

/*************************************************************************************************
 * Filter all non wanted URLs
 * @theURLLink : the URL link as String.
 * Return
 *************************************************************************************************/
bool QCrawler::filterd( QString theURLLink)
{
     for(int i=0;i<filters.list.size();i++)
       if(theURLLink.startsWith(filters.list.at(i),Qt::CaseInsensitive))
          return false;

    return true;
}

/*************************************************************************************************
 * If the URL is HTML Code and not other type of data.
 * @theURL : the URL link as String.
 * Return True if it was HTML code and False if it was not.
 *************************************************************************************************/
bool QCrawler::URL_is_HTML(QString theURL)
{

  for(int i=0;i<extensions.list.size();i++)
    if(theURL.endsWith(extensions.list.at(i),Qt::CaseInsensitive))
       return false;

 return true;
}



/*************************************************************************************************
 * Is this URL in the Database.
 * @theURL : the URL link as String.
 * Return True if the URL is in the database and False if not.
 *************************************************************************************************/
bool QCrawler::this_URL_in_the_database(QString theURL)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(id) FROM "+config.prefix+config.urlsTableName+" WHERE url= '"+theURL+"' ");
    query.exec();

    if (!query.isActive())
        cout << "\n\tDBE - QCrawler::this_URL_in_the_database : " << query.lastError().text().toStdString() <<"\n";

    query.next();
    if (query.value(0).toInt()<1)
        return false;
    else
        return true;
}

/*************************************************************************************************
 * Is this Domian in the Database.
 * @theDomain : the Domain name as String.
 * Return True if the URL is in the database and False if not.
 *************************************************************************************************/
bool QCrawler::this_domain_in_the_database(QString theDomain)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(id) FROM "+config.prefix+config.domainsTableName+" WHERE domain= '"+theDomain+"' ");
    query.exec();

    if (!query.isActive())
        cout << "\n\tDBE - QCrawler::this_domain_in_the_database : " << query.lastError().text().toStdString() <<"\n";

    query.next();
    if (query.value(0).toInt()<1)
        return false;
    else
        return true;
}


/*************************************************************************************************
 * Get URL ID from the database
 * @theURL : the URL link as String.
 * Retrun URL ID as int.
 *************************************************************************************************/
int QCrawler::getURLID(QString theURL)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM "+config.prefix+config.urlsTableName+" WHERE url='"+theURL+"' ");
    query.exec();

    if (!query.isActive())
        cout << "\n\tDBE - QCrawler::getURLID : " << query.lastError().text().toStdString() <<"\n";
    query.next();
    if (query.value(0).toInt()<1)
        return -1;
    else
        return query.value(0).toInt();
}


/*************************************************************************************************
 * Get URL HTML Data from the database
 * @theURL : the URL link as String.
 * Retrun URL HTML Data as String.
 *************************************************************************************************/
QString QCrawler::getURLData(QString theURL)
{
    QSqlQuery query;
    query.prepare("SELECT data FROM "+config.prefix+config.urlsTableName+" WHERE url='"+theURL+"' ");
    query.exec();

    if (!query.isActive())
        cout << "\n\tDBE - QCrawler::getURLData : " << query.lastError().text().toStdString() <<"\n";
    query.next();

        return query.value(0).toString();
}


/*************************************************************************************************
 * Get URL HTML Data from the Internet and save it to the database.
 * @theURL : the URL link as String.
 *************************************************************************************************/
void QCrawler::getHTML(QString theURL)
 {

     QUrl url(theURL);
     QFileInfo fileInfo(url.path());
     QString fileName = fileInfo.fileName();
     myFileName = fileName;

             if (fileName.isEmpty())
                 fileName = "data.xml";
             QDir::setCurrent("/tmp");


             if (QFile::exists("data.xml"))
             {
                 QFile::remove("data.xml");
             }

             file = new QFile("data.xml");


             if (!file->open(QIODevice::WriteOnly)) {
                 delete file;
                 file = 0;
                 return ;
             }

             QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
             http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

             if (!url.userName().isEmpty())
                 http->setUser(url.userName(), url.password());

             httpRequestAborted = false;
             QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
             if (path.isEmpty())
                 path = "/";
             httpGetId = http->get(path, file);


 }



 void QCrawler::cancelDownload()
 {
     httpRequestAborted = true;
     http->abort();

      // set notFinished to false.
      timer->stop();
      notFinieshed = false;

 }

 void QCrawler::progress()
 {
     cout <<"#";
 }

 void QCrawler::httpRequestFinished(int requestId, bool error)
 {
     if (requestId != httpGetId)
         return;
     if (httpRequestAborted) {
         if (file) {
             file->close();
             file->remove();
             delete file;
             file = 0;
         }

         return;
     }

     if (requestId != httpGetId)
         return;
    file->close();


     if (error) {
         file->remove();
     } else {

     }

     delete file;
     file = 0;

     // set notFinished to false.
     timer->stop();
     notFinieshed = false;
   }

 void QCrawler::readResponseHeader(const QHttpResponseHeader &responseHeader)
 {
 switch (responseHeader.statusCode()) {
     case 200:                   // Ok
     case 301:                   // Moved Permanently
     case 302:                   // Found
     case 303:                   // See Other
     case 307:                   // Temporary Redirect

         // these are not error conditions
         break;

     default:
         httpRequestAborted = true;
         http->abort();
     }
 }

 void QCrawler::updateDataReadProgress(int bytesRead, int totalBytes)
 {
     if (httpRequestAborted)
         return;

 }


 void QCrawler::slotAuthenticationRequired(const QString &hostName, quint16, QAuthenticator *authenticator)
 {

 }

 #ifndef QT_NO_OPENSSL
 void QCrawler::sslErrors(const QList<QSslError> &errors)
 {
     QString errorString;
     foreach (const QSslError &error, errors) {
         if (!errorString.isEmpty())
             errorString += ", ";
         errorString += error.errorString();
     }
         http->ignoreSslErrors();
 }
 #endif


 /*************************************************************************************************
  * Delete All URLs in the Database.
  * Delete all URLs HTML data as will.
  *************************************************************************************************/
 void QCrawler::deleteAllURLs()
 {
     QSqlQuery query;
     query.prepare("DELETE FROM "+config.prefix+config.urlsTableName);
     query.exec();
     if (!query.isActive())
         cout <<"\n\tDBE -  QCrawler::deleteAllURLs : "<< query.lastError().text().toStdString()<<"\n" ;
 }

 /*************************************************************************************************
  * Delete All Domains in the Database.
  *************************************************************************************************/
 void QCrawler::deleteAllDomains()
 {
     QSqlQuery query;
     query.prepare("DELETE FROM "+config.prefix+config.domainsTableName);
     query.exec();
     if (!query.isActive())
         cout <<"\n\tDBE -  QCrawler::deleteAllDomains : "<< query.lastError().text().toStdString()<<"\n" ;
 }

 /*************************************************************************************************
  * Delete All Emails in the Database.
  *************************************************************************************************/
 void QCrawler::deleteAllEmails()
 {
     QSqlQuery query;
     query.prepare("DELETE FROM "+config.prefix+config.emailsTableName);
     query.exec();
     if (!query.isActive())
         cout <<"\n\tDBE -  QCrawler::deleteAllEmails : "<< query.lastError().text().toStdString()<<"\n" ;
 }


 /*************************************************************************************************
  * Is the String a URL
  * @theURL : the URL link as String.
  * Return true if URL and False if not URL.
  *************************************************************************************************/
 bool QCrawler::isURL(QString theURL)
 {
     // url Validations
     QRegExp urlRegExp("^(((ht|f)tp(s?))\://)?(www.|[a-zA-Z].)[a-zA-Z0-9\-\.]+\.(([a-z][a-z0-9\-]+(\.|\-*\.))+[a-z]{2,6})(\:[0-9]+)*(/($|[a-zA-Z0-9\.\,\;\?\'\\\+&amp;%\$#\=~_\-]+))*$");
     return urlRegExp.exactMatch(theURL);
 }

 /*************************************************************************************************
  *  Add the Domain to the database
  *  @theDomain : the dmomain name as String with Http or Https
  *************************************************************************************************/
 void QCrawler::addDomain(QString theDomain)
 {
          QSqlQuery query;
          query.prepare("INSERT INTO "+config.prefix+config.domainsTableName+" VALUES (NULL, :domain, 0, 0, 0 )");
          query.bindValue(":domain",theDomain);

          query.exec();
          if (!query.isActive())
              cout << "\n\tDBE - QCrawler::addDomain : " << query.lastError().text().toStdString() <<"\n";
 }

 /*************************************************************************************************
  *  Update URL Crawled States
  *  @theDomain : the dmomain name as string with Http or Https
  *************************************************************************************************/
 void QCrawler::setURLAsCrawled(QString theURL)
 {
          QSqlQuery query;
          query.prepare("UPDATE "+config.prefix+config.urlsTableName+" SET crawled=1  WHERE url= '"+theURL+"' ");
          query.exec();
          if (!query.isActive())
              cout << "\n\tDBE - QCrawler::setURLAsCrawled : " << query.lastError().text().toStdString() <<"\n";
 }

 /*************************************************************************************************
  *  Update URL Done States
  *  @theDomain : the dmomain name as string with Http or Https
  *************************************************************************************************/
 void QCrawler::setURLAsDone(QString theURL)
 {
          QSqlQuery query;
          query.prepare("UPDATE "+config.prefix+config.urlsTableName+" SET done=1  WHERE url= '"+theURL+"' ");
          query.exec();
          if (!query.isActive())
              cout << "\n\tDBE - QCrawler::setURLAsDone : " << query.lastError().text().toStdString() <<"\n";
 }

 /*************************************************************************************************
  *  Add new URL with empty HTML Data and Crawled false and Done is False
  *  @theURL : the Link of the URL as String.
  *************************************************************************************************/
 void QCrawler::addNewURL(QString theURL)
 {
      QSqlQuery query;
      query.prepare("INSERT INTO "+config.prefix+config.urlsTableName+" VALUES (NULL, :url, '',0 ,0 ,0 )");
      query.bindValue(":url",theURL);

      query.exec();
      if (!query.isActive())
          cout << "\n\tDBE - QCrawler::addNewURL : " << query.lastError().text().toStdString() <<"\n";
 }


 /*************************************************************************************************
  * This this URL set as Done
  * @theURL : the URL link as String.
  * Return True if the URL is in the database and False if not.
  *************************************************************************************************/
 bool QCrawler::isDoneURL(QString theURL)
 {
     QSqlQuery query;
     query.prepare("SELECT COUNT(id) FROM "+config.prefix+config.urlsTableName+" WHERE url= '"+theURL+"' AND done='1' ");
     query.exec();

     if (!query.isActive())
         cout << "\n\tDBE - QCrawler::isDoneURL : " << query.lastError().text().toStdString() <<"\n";

     query.next();
     if (query.value(0).toInt()<1)
         return false;
     else
         return true;
 }

 /*************************************************************************************************
  * This this URL set as Crawled
  * @theURL : the URL link as String.
  * Return True if the URL is in the database and False if not.
  *************************************************************************************************/
 bool QCrawler::isThisURLCrawled(QString theURL)
 {
     QSqlQuery query;
     query.prepare("SELECT COUNT(id) FROM "+config.prefix+config.urlsTableName+" WHERE url='"+theURL+"' AND crawled='1' ");
     query.exec();

     if (!query.isActive())
         cout << "\n\tDBE - QCrawler::isThisURLCrawled : " << query.lastError().text().toStdString() <<"\n";

     query.next();
     if (query.value(0).toInt()<1)
         return false;
     else
         return true;
 }


 /*************************************************************************************************
  * get First Not Crawled URL
  * Return URL if there are but empty if not.
  *************************************************************************************************/
 QString QCrawler::getFirstNotCrawledURL()
 {
     QSqlQuery query;
     query.prepare("SELECT url FROM "+config.prefix+config.urlsTableName+" WHERE crawled=0 LIMIT 0,1 ");
     query.exec();

     if (!query.isActive())
         cout << "\n\tDBE - QCrawler::getFirstNotCrawledURL : " << query.lastError().text().toStdString() <<"\n";

     query.next();
     return query.value(0).toString();
 }




 /*************************************************************************************************
  * get First Not Done URL
  * Return URL if there are but empty if not.
  *************************************************************************************************/
 QString QCrawler::getFirstNotDoneURL()
 {
     QSqlQuery query;
     query.prepare("SELECT url FROM "+config.prefix+config.urlsTableName+" WHERE done=0 LIMIT 0,1 ");
     query.exec();

     if (!query.isActive())
         cout << "\n\tDBE - QCrawler::getFirstNotDoneURL : " << query.lastError().text().toStdString() <<"\n";

     query.next();
     return query.value(0).toString();
 }


 /*************************************************************************************************
  * Get Number of  URLs.
  * Return Number
  *************************************************************************************************/
 long int QCrawler::getNumberOfURLs()
 {
     QSqlQuery query;
     query.prepare("SELECT COUNT(id) FROM "+config.prefix+config.urlsTableName);
     query.exec();

     if (!query.isActive())
         cout << "\n\tDBE - QCrawler::getNumberOfURLs : " << query.lastError().text().toStdString() <<"\n";

     query.next();
     return query.value(0).toInt();
 }


 /*************************************************************************************************
  * Get Number of Crawled URLs.
  * Return Number
  *************************************************************************************************/
 long int QCrawler::getNumberOfCrawledURLs()
 {
     QSqlQuery query;
     query.prepare("SELECT COUNT(id) FROM "+config.prefix+config.urlsTableName+" WHERE crawled=1 ");
     query.exec();

     if (!query.isActive())
         cout << "\n\tDBE - QCrawler::getNumberOfCrawledURLs : " << query.lastError().text().toStdString() <<"\n";

     query.next();
     return query.value(0).toInt();
 }

 /*************************************************************************************************
  * Get Number of Done URLs.
  * Return Number
  *************************************************************************************************/
 long int QCrawler::getNumberOfDoneURLs()
 {
     QSqlQuery query;
     query.prepare("SELECT COUNT(id) FROM "+config.prefix+config.urlsTableName+" WHERE done=1 ");
     query.exec();

     if (!query.isActive())
         cout << "\n\tDBE - QCrawler::getNumberOfDoneURLs : " << query.lastError().text().toStdString() <<"\n";

     query.next();
     return query.value(0).toInt();
 }

 /*************************************************************************************************
  * Get Number of Archived URLs.
  * Return Number
  *************************************************************************************************/
 long int QCrawler::getNumberOfArchivedURLs()
 {
     QSqlQuery query;
     query.prepare("SELECT COUNT(id) FROM "+config.prefix+config.urlsTableName+" WHERE archived=1 ");
     query.exec();

     if (!query.isActive())
         cout << "\n\tDBE - QCrawler::getNumberOfArchivedURLs : " << query.lastError().text().toStdString() <<"\n";

     query.next();
      return query.value(0).toInt();
 }

 /*************************************************************************************************
  * Get Number of Empty Data URLs.
  * Return Number
  *************************************************************************************************/
 long int QCrawler::getNumberOfEmptyDataURLs()
 {
     QSqlQuery query;
     query.prepare("SELECT COUNT(id) FROM "+config.prefix+config.urlsTableName+" WHERE data='' ");
     query.exec();

     if (!query.isActive())
         cout << "\n\tDBE - QCrawler::getNumberOfEmptyDataURLs : " << query.lastError().text().toStdString() <<"\n";

     query.next();
     return query.value(0).toInt();
 }

 void QCrawler::beforeContinue()
 {
     QSqlQuery query;
     query.prepare("UPDATE "+config.prefix+config.urlsTableName+" SET crawled=0 WHERE data='' ");

     query.exec();
     if (!query.isActive())
         cout << "\n\tDBE - QCrawler::beforeContinue() : " << query.lastError().text().toStdString() <<"\n";
 }


 // Create Connection to the database.
 bool QCrawler::createConnection()
 {
         db = QSqlDatabase::addDatabase(config.databaseType);
         db.setHostName(config.hostName);
         db.setDatabaseName(config.databaseName);
         db.setUserName(config.userName);
         db.setPassword(config.password);
         db.setPort(config.port.toInt());

         if( !db.open() )
         {
             cout << "\n\tDatabase Connection Error :  " << db.lastError().text().toStdString() <<"\n";
                 return false;
         }
         return true;
 }

 // print Help menu
 void QCrawler::printHelp()
 {
     cout<<"\n+---------------------------------------------------------------+\n"
         <<"|    List of all Crawler commands:                              |\n"
         <<"+---------------------------------------------------------------+\n"
         <<"|  ?                  : Synonym for 'help'.                     |\n"
         <<"|  clear              : Clear the current input statement.      |\n"
         <<"|  delete             : Delete data from the database .         |\n"
         <<"|  exit               : Exit Crawler. Same as quit.             |\n"
         <<"|  help               : Display this help.                      |\n"
         <<"|  load               : Load Crawler Settings.                  |\n"
         <<"|  filters            : Show List of Filterd URLS.              |\n"
         <<"|  extensions         : Show List of File extensions.           |\n"
         <<"|  configurations      : Show Database configurations.            |\n"
         <<"|  info,total         : Display Total Informaitons              |\n"
         <<"+---------------------------------------------------------------+\n\n";
 }

 void QCrawler::printConfig()
 {
     cout <<"+-------------------------------------------------------------+\n"
          <<"|        Configraitons                                        |\n"
          <<"+-------------------------------------------------------------+\n"
          <<"|      Database Type : "<< config.databaseType.toStdString() <<"\n"
          <<"|          Host Name : "<< config.hostName.toStdString() <<"\n"
          <<"|      Database Name : "<< config.databaseName.toStdString() <<"\n"
          <<"|    Database Prefix : "<< config.prefix.toStdString() <<"\n"
          <<"|    URLs Table Name : "<< config.urlsTableName.toStdString() <<"\n"
          <<"+-------------------------------------------------------------+\n\n";
 }

 void QCrawler::doDelete()
 {
     cout<<" Are you sure that you want to Delete Data for:\n"
         <<" Type [Yes/y] to delete or anything else to cancel delete > ";

     string deleteConfirm;
     cin>>deleteConfirm;
     if(deleteConfirm=="y" || deleteConfirm=="Y"  || deleteConfirm=="Yes"  || deleteConfirm=="yes")
     {
       deleteAllURLs();
       deleteAllDomains();
       deleteAllEmails();

       cout <<"\n  Data have been deleted\n\n";
     }
     else
     {
       cout <<"\n  Delete was canceled\n\n";
     }
 }


 void QCrawler::doDeleteBeforeStartCrawling()
 {
     cout<<" You are About to Start new Crawling task\n"
         <<" Old Data will be deleted\n"
         <<" Type [Yes/y] to delete or anything else to cancel delete > ";

     string deleteConfirm;
     cin>>deleteConfirm;
     if(deleteConfirm=="y" || deleteConfirm=="Y"  || deleteConfirm=="Yes"  || deleteConfirm=="yes")
     {
       deleteAllURLs();
       deleteAllDomains();
      deleteAllEmails();

       cout <<"\n  Old Data have been deleted\n\n";
     }
     else
     {
       cout <<"\n  Old Data is saved\n\n";
     }
 }

 void QCrawler::printTotlaInfo()
 {
     cout <<"\n+-------------------------------------------------------------+\n"
          <<"|        Total Iinformations                                  |\n"
          <<"+-------------------------------------------------------------+\n"
          <<"|              Total Number of URLs = "<<getNumberOfURLs()<<"\n";
     cout <<"|      Total Number of Crawled URLs = "<<getNumberOfCrawledURLs()<<"\n";
     cout <<"|         Total Number of Done URLs = "<<getNumberOfDoneURLs()<<"\n";
     cout <<"|     Total Number of Archived URLs = "<<getNumberOfArchivedURLs()<<"\n";
     cout <<"|   Total Number of Empty Data URLs = "<<getNumberOfEmptyDataURLs()<<"\n"
          <<"+-------------------------------------------------------------+\n\n";
 }

 void QCrawler::printFilters()
 {
     this->filters.print("Filters");
 }

 void QCrawler::printExtensions()
 {
     this->extensions.print("Extintions");
 }
