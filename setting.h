#ifndef SETTING_H
#define SETTING_H
#include "stable.h"
#include "xml_parser.h"

class Setting
{
private:
    QMap<QString, float> _list;

    static QMap<QString, float> _default;
    static QMap<QString, float> InitDefault();
public:
    Setting();
    void ResetDefault();
    void WriteSetting();
    void ReadSetting();
    QMap<QString, float> list();
    void setList(const QMap<QString, float> &list);
};

#endif // SETTING_H
