#include "xml_bus.h"

Xml_Bus::Xml_Bus()
{

}
QDomElement Xml_Bus::GetAllSong(QDomElement node)
{
    QDomElement result = creatRoot("SONG_LIST");
    GetAllSong(node, result);
    return result;
}
void Xml_Bus::GetAllSong(QDomElement node, QDomElement &result)
{
    if (!node.hasChildNodes())
    {
        QDomElement temp = creatNode("SONG", result);
        temp.setAttribute("ID", node.attribute("ID"));
        temp.setAttribute("Title", node.attribute("Title"));
        temp.setAttribute("Path", node.attribute("Path"));
    }

    for (int i = 0; i < node.childNodes().length(); i++)
    {
        GetAllSong(node.childNodes().at(i).toElement(), result);
    }
}

