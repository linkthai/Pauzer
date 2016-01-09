#include "setting.h"

QMap<QString, float> Setting::_default = Setting::InitDefault();

Setting::Setting()
{
    _list = InitDefault();
}

QMap<QString, float> Setting::InitDefault()
{
    QMap<QString, float> temp;

    temp.insert("Volume", 1);
    temp.insert("Detector", 1);
    temp.insert("Lock", 0);
    temp.insert("Shuffle", 0);
    temp.insert("Repeat", 0);
    temp.insert("State_Full", 1);
    temp.insert("Maximized", 0);
    return temp;
}

void Setting::ResetDefault()
{
    _list = _default;
}
void Setting::WriteSetting()
{
    Xml_Bus BUS;

    if (!QDir("Data").exists())
        QDir().mkdir("Data");

    QString XmlPath = "Data\\Setting.xml";
    QFile file(XmlPath);

    QDomDocument doc;
    QDomElement data = BUS.creatRoot("SETTINGS");

    QDomProcessingInstruction header = doc.createProcessingInstruction("xml", "version='1.0' encoding='UTF-16'");

    doc.appendChild(header);
    doc.appendChild(data);

    foreach(QString key, _list.keys())
    {
        QDomElement temp = BUS.creatNode("UNIT", data);
        temp.setAttribute("Key", key);
        temp.setAttribute("Value", _list.value(key));
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // Error while loading file
        std::cerr << "Error while loading file" << std::endl;
    }

    QTextStream stream(&file);
    doc.save(stream, 0);
    file.close();
}

void Setting::ReadSetting()
{

    QDomElement data;
    QString XmlPath = "Data\\Setting.xml";

    QFileInfo checkFile(XmlPath);
    if (!(checkFile.exists() && checkFile.isFile()))
        return;

    QFile file(XmlPath);
    QDomDocument doc;

    if (!file.open(QIODevice::ReadOnly ))
    {
        // Error while loading file
        std::cerr << "Error while loading file" << std::endl;
    }
    else
    {
        doc.setContent(&file);
        file.close();
        data = doc.documentElement();
    }

    for (int i = 0; i < data.childNodes().length(); i++)
    {
        _list.insert(data.childNodes().at(i).toElement().attribute("Key"),
                      data.childNodes().at(i).toElement().attribute("Value").toFloat());
    }
}

QMap<QString, float> Setting::list()
{
    return _list;
}

void Setting::setList(const QMap<QString, float> &list)
{
    _list = list;
}

