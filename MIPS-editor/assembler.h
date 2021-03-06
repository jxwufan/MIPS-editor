#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <QMap>

class QString;
class QStringList;

class Assembler
{
public:
    Assembler();
    ~Assembler();

    QString ass2bin(QString input);

private:
    QMap<qlonglong, QString> ram;
    QMap<QString, qlonglong> labelTable;
    QMap<QString, int> registerTable;
    QMap<QString, int> functCode;
    QMap<QString, int> type1;
    QMap<QString, int> type2;
    QMap<QString, int> type3;
    QMap<QString, int> type4;
    QMap<QString, int> type5;

    QString translate2bin(QString instruction, int address);
    QString fillZeroOrChop(QString str, int len);
    QString imm2bin(QString str, int len);
    QStringList imm2two(QString str);
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
