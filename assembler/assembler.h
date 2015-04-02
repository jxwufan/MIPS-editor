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
};

#endif // ASSEMBLER_H
