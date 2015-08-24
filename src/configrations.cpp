#include "configurations.h"

#include <iostream>
using namespace std;

configurations::configurations(QObject *parent) :
    QObject(parent)
{
}

void configurations::load(QString fileName)
{


    // Open the xml file and load it to the memory
    // -----------------------------------------------
            QDomDocument doc("xml");
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly))
            {
                cout << " configurations::load File Error : Open File : " << file.errorString().toStdString() <<"\n";
                return;
            }
            if (!doc.setContent(&file)) {
                file.close();
                cout << " configurations::load File Error : Get Content :" << file.errorString().toStdString() <<"\n";
                return;
            }
            file.close();

            QDomElement configurationsElement = doc.documentElement();
            QDomNode databaseChiledNode  = configurationsElement.firstChild();

            while (!databaseChiledNode.isNull()) { // For all Database Chiled Nodes
                   QDomElement databaseChiledElement = databaseChiledNode.toElement();

                            if(databaseChiledElement.tagName() == "type")  // Get Databse Type.
                            {
                                    QDomNode typeNode = databaseChiledNode.firstChild();
                                    this->databaseType  = typeNode.toText().data();
                            }

                            if(databaseChiledElement.tagName() == "host")  // Get Databse host name.
                            {
                                    QDomNode hostNameNode = databaseChiledNode.firstChild();
                                    this->hostName  = hostNameNode.toText().data();
                            }

                            if(databaseChiledElement.tagName() == "name")  // Get Databse name.
                            {
                                    QDomNode nameNode = databaseChiledNode.firstChild();
                                    this->databaseName  = nameNode.toText().data();
                            }

                            if(databaseChiledElement.tagName() == "user")  // Get User name.
                            {
                                    QDomNode userNode = databaseChiledNode.firstChild();
                                    this->userName  = userNode.toText().data();
                            }

                            if(databaseChiledElement.tagName() == "password")  // Get  password.
                            {
                                    QDomNode passwordNode = databaseChiledNode.firstChild();
                                    this->password  = passwordNode.toText().data();
                            }

                            if(databaseChiledElement.tagName() == "port")  // Get Databse port.
                            {
                                    QDomNode portNode = databaseChiledNode.firstChild();
                                    this->port  = portNode.toText().data();
                            }

                            if(databaseChiledElement.tagName() == "prefix")  // Get Databse prefix.
                            {
                                    QDomNode prefixNode = databaseChiledNode.firstChild();
                                    this->prefix  = prefixNode.toText().data();
                            }

                            if(databaseChiledElement.tagName() == "urlsTableName")  // Get URLs Table Name.
                            {
                                    QDomNode urlsTableNameNode = databaseChiledNode.firstChild();
                                    this->urlsTableName  = urlsTableNameNode.toText().data();
                            }

                            if(databaseChiledElement.tagName() == "domainsTableName")  // Get Domains Table Name.
                            {
                                    QDomNode domainsTableNameNode = databaseChiledNode.firstChild();
                                    this->domainsTableName  = domainsTableNameNode.toText().data();
                            }

                            if(databaseChiledElement.tagName() == "emailsTableName")  // Get Emails Table Name.
                            {
                                    QDomNode emailsTableNameNode = databaseChiledNode.firstChild();
                                    this->emailsTableName  = emailsTableNameNode.toText().data();
                            }

                            if(databaseChiledElement.tagName() == "WAITE_TIME")  // Get WAITE_TIME.
                            {
                                    QDomNode WAITE_TIMENode = databaseChiledNode.firstChild();
                                    this->WAITE_TIME  = WAITE_TIMENode.toText().data().toInt();
                            }

                            if(databaseChiledElement.tagName() == "PROGRESS_PAR_SHOW_TIME")  // Get PROGRESS_PAR_SHOW_TIME.
                            {
                                    QDomNode PROGRESS_PAR_SHOW_TIMENode = databaseChiledNode.firstChild();
                                    this->PROGRESS_PAR_SHOW_TIME  = PROGRESS_PAR_SHOW_TIMENode.toText().data().toInt();
                            }

               databaseChiledNode = databaseChiledNode.nextSibling();
            }
}
