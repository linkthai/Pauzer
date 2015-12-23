#ifndef XML_COMMON_H
#define XML_COMMON_H
#include "stable.h"

class Xml_Common
{
public:
    Xml_Common();
    QDomElement creatRoot(QString name);
    QDomElement creatNode(QString name, QDomElement &dad);
    QDomElement creatNode(QDomElement child, QDomElement &dad, bool allChild = true);
};

#endif // XML_COMMON_H
