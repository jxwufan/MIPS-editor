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
    QMap<QString, int> functCode;
    QMap<QString, int> type1;
    QMap<QString, int> type2;
    QMap<QString, int> type3;
    QMap<QString, int> type4;
    QMap<QString, int> type5;

    QString translate2bin(QString instruction);
    QString fillZeroOrChop(QString str, int len);
};

#endif // ASSEMBLER_H

/*
 * type1 include
 * add
 * addu
 * and
 * nor
 * or
 * sub
 * subu
 * xor
 * slt
 * sltu
 * */
