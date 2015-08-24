#include "qfilters.h"
#include <iostream>
using namespace std;

QFilters::QFilters(QObject *parent) :
    QObject(parent)
{
}


void QFilters::load(QString fileName)
{
    // Open the xml file and load it to the memory
    // -----------------------------------------------
            QDomDocument doc("xml");
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly))
            {
                cout << " QFilters::load File Error : Open File : " << file.errorString().toStdString() <<"\n";
                return;
            }
            if (!doc.setContent(&file)) {
                file.close();
                cout << " QFilters::load File Error : Get Content :" << file.errorString().toStdString() <<"\n";
                return;
            }
            file.close();
            list.clear();
            QDomElement filtersElement = doc.documentElement();
            QDomNode filterChiledNode  = filtersElement.firstChild();

            while (!filterChiledNode.isNull()) { // For all filter Chiled Nodes
                   QDomElement filterChiledElement = filterChiledNode.toElement();
                            if(filterChiledElement.tagName() == "filter")  // Get filter.
                            {
                                    QDomNode filterNode = filterChiledNode.firstChild();
                                    list << filterNode.toText().data();
                            }
                  filterChiledNode = filterChiledNode.nextSibling();
            }
}

void QFilters::print(QString theTitle)
{
    cout <<"\n+-------------------------------------------------------------+\n"
         <<"|        "<< theTitle.toStdString()<<"\n"
         <<"+-------------------------------------------------------------+\n";
    for(int i=0;i<list.size();i++)
         cout<<"| "<<list.at(i).toStdString()<<"\n";
    cout <<"+-------------------------------------------------------------+\n\n";
}
