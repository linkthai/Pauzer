#include "xml_common.h"

Xml_Common::Xml_Common()
{
}

QDomElement Xml_Common::creatRoot(QString name)
{
    QDomElement result;
    QDomDocument doc;
    result = doc.createElement(name);
    doc.appendChild(result);
    return result;
}
QDomElement Xml_Common::creatNode(QString name, QDomElement dad)
{
    QDomElement result;
    QDomDocument doc = dad.ownerDocument();
    result = doc.createElement(name);
    dad.appendChild(result);
    return result;
}
QDomNode Xml_Common::creatNode(QDomElement node, QDomElement dad, bool allChild)
{
    QDomNode result;
    QDomDocument doc;
    result = doc.importNode(node, allChild);
    dad.appendChild(result);
    return result;
}

