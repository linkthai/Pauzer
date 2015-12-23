#ifndef XML_BUS_H
#define XML_BUS_H
#include "xml_common.h"
#include "song.h"

class Xml_Bus : public Xml_Common
{
public:
    Xml_Bus();
    QDomElement GetAllSong(QDomElement node);
    void GetAllSong(QDomElement node, QDomElement &result);
};

#endif // XML_BUS_H
