#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <QMap>

class QString;

class Assembler
{
public:
    Assembler();
    ~Assembler();

    QString ass2bin(QString input);

private:
    QMap<int, QString> ram;
    QMap<QString, int> labelTable;
    QMap<QString, int> registerTable;
    QMap<QString, int> intFunctCode;
    QString translate2bin(QString instruction);
};

#endif // ASSEMBLER_H
