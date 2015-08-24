#include "qextensions.h"
#include <iostream>
using namespace std;

QExtensions::QExtensions(QObject *parent) :
    QObject(parent)
{
}
void QExtensions::load(QString fileName)
{
    // Open the xml file and load it to the memory
    // -----------------------------------------------
            QDomDocument doc("xml");
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly))
            {
                cout << " QExtensions::load File Error : Open File : " << file.errorString().toStdString() <<"\n";
                return;
            }
            if (!doc.setContent(&file)) {
                file.close();
                cout << " QExtensions::load File Error : Get Content :" << file.errorString().toStdString() <<"\n";
                return;
            }
            file.close();
            list.clear();
            QDomElement extensionsElement = doc.documentElement();
            QDomNode extensionsChiledNode  = extensionsElement.firstChild();

            while (!extensionsChiledNode.isNull()) { // For all extension Chiled Nodes
                   QDomElement extensionChiledElement = extensionsChiledNode.toElement();
                            if(extensionChiledElement.tagName() == "extension")  // Get extension.
                            {
                                    QDomNode extensionNode = extensionChiledElement.firstChild();
                                    list << extensionNode.toText().data();
                            }
                  extensionsChiledNode = extensionsChiledNode.nextSibling();
            }
}
void QExtensions::print(QString theTitle)
{
    cout <<"\n+-------------------------------------------------------------+\n"
         <<"|        "<< theTitle.toStdString()<<"\n"
         <<"+-------------------------------------------------------------+\n";
    for(int i=0;i<list.size();i++)
         cout<<"| "<<list.at(i).toStdString()<<"\n";
    cout <<"+-------------------------------------------------------------+\n\n";
}
