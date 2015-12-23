#ifndef XML_DATA_H
#define XML_DATA_H
#include "xml_common.h"

class Xml_Data : public Xml_Common
{
public:
    Xml_Data();
    QDomElement readXml(const QString &filename);
    QString writeXml(QDomElement data, const QString &filename);
};

#endif // XML_DATA_H
