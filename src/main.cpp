#include <QtCore/QCoreApplication>
#include <QApplication>
#include <QtSql>
#include <iostream>
#include <string>

using namespace std;

#include "qcrawler.h"
#include "configurations.h"
#include <process.h>

// Create Connection to the database.


// Crate the Crawler Object
QCrawler crawler ;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    string buffer;
    QString cinBuffer;

    cout <<"\n\n"
         <<"  Natshah Crawler [ Version 1.10.12 ]\n"
         <<"  Copyright (c) 2015 Natshah.com .  All rights reserved.\n\n";


    // Load All Crawler Settings.
    crawler.load();

    // Print the configurations Menu at the start.
    crawler.printConfig();

    // Create the Connection to the database.
    if( !crawler.createConnection() )
    {
        cout <<"\n\n"
             <<"     Error Connecting to the Database\n"
             <<"     Press Enter to exit ..";
        cin.get();
        return 1;
    }

    while (buffer !="exit" || buffer !="Exit" ||buffer !="EXIT" )
    {
            buffer.clear();
            cinBuffer.clear();

            // Print Crawler Promt.
            cout<<"Natshah Crawler> ";
            cin>>buffer;

            cinBuffer = QString::fromStdString(buffer);

            if ( cinBuffer.startsWith("start") || cinBuffer.startsWith("Start") || cinBuffer.startsWith("START"))
            {

                string urlbuffer;
                cout<<"  Start Crawling\n"
                    <<"  Enter Domain name : ";
                cin>>urlbuffer;
                QString url  = QString::fromStdString(urlbuffer);

               if (crawler.isURL(url))
               {

                    crawler.doDeleteBeforeStartCrawling();

                   // Load All Crawler Settings.
                   crawler.load();

                   // put the buffer to the root URL.
                   crawler.rootURL = url;

                    cout <<"\n+-------------------------------------------------------------+\n"
                         <<"|  Add Domain : "<< crawler.rootURL.toStdString()<<"\n"
                         <<"+-------------------------------------------------------------+\n\n";
                    crawler.addDomain(crawler.rootURL);


                        cout <<"\n+-------------------------------------------------------------+\n"
                             <<"|    Start Crawleing "<< crawler.rootURL.toStdString()<<"\n"
                             <<"+-------------------------------------------------------------+\n\n";

                                    cout << "\r\tCrawleing Start\t     ";
                                    crawler.download_the_HTML_code_for_the_URL_in_a_file(crawler.rootURL);
                                    cout <<"\r\tCrawleing\t\t\t\t\t[  Done  ]\n";


                                    cout << "\tRTHTMLFTF";
                                    crawler.read_the_HTML_data_from_the_file();
                                    cout << "\r\tRTHTMLFTF\t\t\t\t\t[  Done  ]\n";

                                    cout << "\tSTHTMLDITD";
                                    crawler.addNewURL(crawler.rootURL);
                                    crawler.save_the_HTML_data_in_the_database(crawler.rootURL);
                                    cout << "\r\tSTHTMLDITD\t\t\t\t\t[  Done  ]\n";



                                     crawler.setURLAsCrawled(crawler.rootURL);
                                     crawler.setURLAsDone(crawler.rootURL);

                                     crawler.addNewURL(crawler.rootURL+"/");
                                     crawler.save_the_HTML_data_in_the_database(crawler.rootURL+"/");
                                     crawler.setURLAsCrawled(crawler.rootURL+"/");
                                     crawler.setURLAsDone(crawler.rootURL+"/");

                                    // Crawel All Sub URLs Links in This site.
                                    crawler.addSubURLs(crawler.rootURL);

                        cout <<"\n+-------------------------------------------------------------+\n"
                             <<"|   Finish Crawleing "<< crawler.rootURL.toStdString()<<"\n"
                             <<"+-------------------------------------------------------------+\n\n";
                } else
                    cout <<"\n\n"
                         <<"  Error in the URL\n"
                         <<"  URL must be http:// or https://\n"
                         <<"  for example http://natshah.com \n\n";
            }
            else if (cinBuffer.startsWith("continue") || cinBuffer.startsWith("cont") || cinBuffer.startsWith("Continue") || cinBuffer.startsWith("Cont") || cinBuffer.startsWith("CONTINUE") || cinBuffer.startsWith("CONT"))
            {
                string urlbuffer;
                cout<<"  Continue Crawling\n"
                    <<"  Enter Domain name : ";
                cin>>urlbuffer;
                QString url  = QString::fromStdString(urlbuffer);

               if (crawler.isURL(url))
               {

                   // Load All Crawler Settings.
                   crawler.load();

                   // put the buffer to the root URL.
                   crawler.rootURL = url;

                   // Fix some Data before Continue.
                   crawler.beforeContinue();

                        cout <<"\n+-------------------------------------------------------------+\n"
                             <<"|    Continue Crawleing "<< crawler.rootURL.toStdString()<<"\n"
                             <<"+-------------------------------------------------------------+\n\n";

                                    // Crawel All Sub URLs Links in This site.
                                    crawler.addSubURLs(crawler.rootURL);

                        cout <<"\n+-------------------------------------------------------------+\n"
                             <<"|    Finish Crawleing "<< crawler.rootURL.toStdString()<<"\n"
                             <<"+-------------------------------------------------------------+\n\n";
                } else
                    cout <<"\n\n"
                         <<"     Error in the URL\n"
                         <<"     URL must be http:// or https://\n"
                         <<"     for example http://natshah.com \n\n";
            }
           else if (cinBuffer=="load" || cinBuffer=="Load" || cinBuffer=="LOAD")
             crawler.load();
          else if (cinBuffer=="delete" || cinBuffer=="Delete" || cinBuffer=="DELETE" )
             crawler.doDelete();
           else if (cinBuffer=="filters" || cinBuffer=="filter" || cinBuffer=="Filters" || cinBuffer=="Filter" || cinBuffer=="FILTERS" || cinBuffer=="FILTER")
             crawler.printFilters();
           else if (cinBuffer=="extensions" || cinBuffer=="ext" || cinBuffer=="extensions" || cinBuffer=="Ext"|| cinBuffer=="extensions" || cinBuffer=="EXT")
             crawler.printExtensions();
           else if(cinBuffer=="total" || cinBuffer=="info" || cinBuffer=="Total" || cinBuffer=="Info" || cinBuffer=="TOTAL" || cinBuffer=="INFO")
             crawler.printTotlaInfo();
           else if (cinBuffer=="help" || cinBuffer=="Help" || cinBuffer=="HELP" || cinBuffer=="-h" || cinBuffer=="-help" || cinBuffer=="-?" || cinBuffer=="?")
             crawler.printHelp();
           else if (cinBuffer=="configurations" || cinBuffer=="config" || cinBuffer=="configurations" || cinBuffer=="Config" || cinBuffer=="configurations" || cinBuffer=="CONFIG")
             crawler.printConfig();
           else if (cinBuffer=="clear" || cinBuffer=="cls"|| cinBuffer=="Clear" || cinBuffer=="Cls" || cinBuffer=="CLEAR" || cinBuffer=="CLS" )
            {
                   system("cls");
                   a.flush();
                   cout<<"\n";
            }
            else if (cinBuffer=="exit" || cinBuffer=="quite" || cinBuffer=="Exit" || cinBuffer=="Quite" || cinBuffer=="EXIT" || cinBuffer=="QUITE" )
            {
                    cout<<"Bye\n";
                    a.exit();
                    return 0;
            }
            else
            {
                cout <<"'"<<cinBuffer.toStdString()<<"' is not recognized as an internal command\n"
                     <<" for help type 'help' or -h then Press Enter to see list of Commands\n";
                QApplication::beep();
            }

     }
    a.exit();
    return 0;
}
