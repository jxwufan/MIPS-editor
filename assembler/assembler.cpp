#include "assembler.h"

#include <QList>
#include <QString>
#include <QStringList>
#include <QDebug>

Assembler::Assembler()
{
    registerTable.clear();
    registerTable["$ZERO"]  = 0;
    registerTable["$AT"]    = 1;
    registerTable["$V0"]    = 2;
    registerTable["$V1"]    = 3;
    registerTable["$A0"]    = 4;
    registerTable["$A1"]    = 5;
    registerTable["$A2"]    = 6;
    registerTable["$A3"]    = 7;
    registerTable["$T0"]    = 8;
    registerTable["$T1"]    = 9;
    registerTable["$T2"]    = 10;
    registerTable["$T3"]    = 11;
    registerTable["$T4"]    = 12;
    registerTable["$T5"]    = 13;
    registerTable["$T6"]    = 14;
    registerTable["$T7"]    = 15;
    registerTable["$S0"]    = 16;
    registerTable["$S1"]    = 17;
    registerTable["$S2"]    = 18;
    registerTable["$S3"]    = 19;
    registerTable["$S4"]    = 20;
    registerTable["$S5"]    = 21;
    registerTable["$S6"]    = 22;
    registerTable["$S7"]    = 23;
    registerTable["$T8"]    = 24;
    registerTable["$T9"]    = 25;
    registerTable["$K0"]    = 26;
    registerTable["$K1"]    = 27;
    registerTable["$GP"]    = 28;
    registerTable["$SP"]    = 29;
    registerTable["$FP"]    = 30;
    registerTable["$RA"]    = 31;

    functCode.clear();
    functCode["SLL"]  = 0;
    functCode["SRL"]  = 2;
    functCode["SRA"]  = 3;
    functCode["SLLV"] = 4;
    functCode["SRLV"] = 6;
    functCode["SRAV"] = 7;

    functCode["MUL"]  = 24;
    functCode["MULU"] = 25;
    functCode["DIV"]  = 26;
    functCode["DIVU"] = 27;
    functCode["ADD"]  = 32;
    functCode["ADDU"] = 33;
    functCode["SUB"]  = 34;
    functCode["SUBU"] = 35;
    functCode["AND"]  = 36;
    functCode["OR"]   = 37;
    functCode["XOR"]  = 38;
    functCode["NOR"]  = 39;
    functCode["SLT"]  = 42;
    functCode["SLTU"] = 43;

    type1.clear();
    type1["ADD"]	 = 0x20;
    type1["ADDU"]	 = 0x21;
    type1["AND"]	 = 0x24;
    type1["NOR"]	 = 0x27;
    type1["OR"]		 = 0x25;
    type1["SUB"]	 = 0x22;
    type1["SUBU"]	 = 0x23;
    type1["XOR"]	 = 0x26;
    type1["SLT"]	 = 0x2a;
    type1["SLTU"]	 = 0x2b;

    type2.clear();
    type2["LW"]		= 0x23;
    type2["LB"]		= 0x20;
    type2["LBU"]	= 0x24;
    type2["LH"]		= 0x21;
    type2["LHU"]	= 0x25;
    type2["SW"]		= 0x2b;
    type2["SB"]		= 0x28;
    type2["SH"]		= 0x29;

    type3.clear();
    type3["ADDI"]	=	8;
    type3["ADDIU"]	=	9;
    type3["ANDI"]	=	0xc;
    type3["ORI"]	=	0xd;
    type3["XORI"]	=	0xe;
    type3["SLTI"]	=	0xa;
    type3["SLTIU"]	=	0xb;

    type4.clear();
    type4["SLL"]	=	0;
    type4["SRL"]	=	2;
    type4["SRA"]	=	3;

    type5.clear();
    type5["MULT"]	=	0x18;
    type5["MULTU"]	=	0x19;
    type5["DIV"]	=	0x1a;
    type5["DIVU"]	=	0x1b;
}

Assembler::~Assembler()
{

}

QString Assembler::ass2bin(QString input)
{
    input = input.toUpper();
    input = input.replace("\t", " ");
    QStringList inputCodes = input.split(QChar('\n'), QString::SkipEmptyParts);
    QString output = "";

    labelTable.clear();
    ram.clear();

    for (int i = 0; i < inputCodes.size(); ++i) {
        inputCodes[i] = inputCodes[i].trimmed();
        if (inputCodes[i].indexOf('#') >= 0)
            inputCodes[i].truncate(inputCodes[i].indexOf('#'));
        if (inputCodes[i].indexOf("//") >= 0)
            inputCodes[i].truncate(inputCodes[i].indexOf("//"));
    }
    input = inputCodes.join("\n");
    input.replace(";", "\n");
    inputCodes = input.split('\n', QString::SkipEmptyParts);

    enum Mode {
        Instruction, Data
    };

    Mode mode = Instruction;
    int address = 0;

    foreach (QString str, inputCodes) {
        if (str.startsWith("BASEADDRE")) {
            mode = Instruction;
            bool ok;
            address = str.split(':').at(1).trimmed().toInt(&ok, 16);
        } else if (str.startsWith("DATAADDRE")) {
            mode = Data;
            bool ok;
            address = str.split(':').at(1).trimmed().toInt(&ok, 16);
        } else if (str.contains("EQU")) {
            str.replace("EQU", " ");
            QStringList list = str.split(" ");
            labelTable[list.at(0).trimmed()] = list.at(1).trimmed().toInt();
        } else {
            if (str.contains(':')) {
                QStringList list= str.split(':');
                labelTable[list.at(0).trimmed()] = address;
                str = list.at(1).trimmed();
                if (str.isEmpty()) continue;
            }
            if (mode == Instruction) {
                //pesudo instruction
                for (int i = 0; i < 4; i++)
                    if (ram.contains(address + i)) {
                        //QMessageBox conflict
                        return "";
                    }
                ram[address] = str.trimmed();
                address += 4;
            } else {
                if (str.startsWith("DB")) {
                    str = str.remove(0, 2).trimmed();
                    QStringList list = str.split(',');
                    for (int i = 0; i < list.size(); i++) {
                        if (ram.contains(address + i)) {
                            //QMessageBox conflict
                            return "";
                        }
                        ram[address + i] = list.at(i).trimmed();
                    }
                    address += list.size();
                } else {
                    str = str.remove(0, 2).trimmed();
                    QStringList list = str.split(',');
                    for (int i = 0; i < list.size(); i++) {
                        if (ram.contains(address + i * 2)) {
                            //QMessageBox conflict
                            return "";
                        }
                        QString temp = list.at(i).trimmed();
                        if (temp.size() == 3) {
                            temp.chop(1);
                            temp += QChar('\000');
                            temp += QChar('\'');
                        }
                        ram[address + i * 2] = temp;
                    }
                    address += list.size() * 2;
                }
            }
        }
    }

    for (int address = 0; address < 200; address++)
    {
        if (ram.contains(address)) {
            QString str = ram[address];
            qDebug() << "Line\t" << address << "\t" << ram[address];
            if (str.contains('\'')) {
                str.remove(0, 1);
                str.chop(1);
                output += str;
                if (str.size() == 2) address++;
            }
            else if (str.startsWith("0X")){
                str.remove(0, 2);
                if (str.size() == 4) address++;
                while (!str.isEmpty()) {
                    output += QChar(str.mid(0, 2).toInt(NULL, 16));
                    str.remove(0, 2);
                }
            } else {
                output += translate2bin(str);
                address += 3;
            }
        } else
            output += QChar(0);
    }

    return output;
}

QString Assembler::translate2bin(QString instruction)
{
    QString bin = "";
    QString binstr = "";
    QString operation;
    QStringList operands;
    operation = instruction.mid(0, instruction.indexOf(" ")).trimmed();
    operands = instruction.remove(0, instruction.indexOf(" ")).replace(" ", "").split(",");

    if (type1.contains(operation)) {
        if (operands.size() != 3) {
            //QMessageBox invalid
            return "";
        }
        binstr += "000000";
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(1)], 2), 5);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(2)], 2), 5);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += "00000";
        binstr += fillZeroOrChop(QString::number(type1[operation], 2), 6);
    } else if (type2.contains(operation)) {
        if (operands.size() != 2) {
            //QMessageBox invalid
            return "";
        }
        QStringList offsetWithRegister = operands.at(1).split("(");
        offsetWithRegister[1].replace(")", "");
        binstr += fillZeroOrChop(QString::number(type2[operation], 2), 6);
        binstr += fillZeroOrChop(QString::number(registerTable[offsetWithRegister.at(1)], 2), 5);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop(QString::number(offsetWithRegister.at(0).toInt(NULL, 10), 2), 16);
    }

    binstr = "10000101010000000000000000010000";

    while (!binstr.isEmpty()) {
        bin += QChar(binstr.mid(0, 8).toInt(NULL, 2));
        binstr.remove(0, 8);
    }

    qDebug() << bin.size();

    if (bin.contains(QChar(194))) qDebug() << "shit";

    return bin;
}

QString Assembler::fillZeroOrChop(QString str, int len)
{
    QString out = "";
    if (str.size() < len) {
        for (int i = 0; i < len - str.size(); i++)
            out += "0";
        out += str;
    } else {
        out += str.mid(str.size() - len, len);
    }
    return out;
}

