#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H
#include <QMap>

class QString;

class Disassembler
{
public:
    Disassembler();
    ~Disassembler();

    QString bin2ass(QString input);

private:
    QMap<int, QString> opTable;
    QMap<int, QString> functTable;
    QMap<int, QString> registerTable;
    QMap<int, QString> rtTable;
    QMap<QString, int> type1;
    QMap<QString, int> type2;
    QMap<QString, int> type3;
    QMap<QString, int> type4;
    QMap<QString, int> type5;
};

#endif // DISASSEMBLER_H
