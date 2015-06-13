#include "assembler.h"
#include "binarystringtranslator.h"

#include <QList>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>

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

    registerTable["$R0"]    =   0;
    registerTable["$R1"]    =   1;
    registerTable["$R2"]    =   2;
    registerTable["$R3"]    =   3;
    registerTable["$R4"]    =   4;
    registerTable["$R5"]    =   5;
    registerTable["$R6"]    =   6;
    registerTable["$R7"]    =   7;
    registerTable["$R8"]    =   8;
    registerTable["$R9"]    =   9;
    registerTable["$R10"]    =   10;
    registerTable["$R11"]    =   11;
    registerTable["$R12"]    =   12;
    registerTable["$R13"]    =   13;
    registerTable["$R14"]    =   14;
    registerTable["$R15"]    =   15;
    registerTable["$R16"]    =   16;
    registerTable["$R17"]    =   17;
    registerTable["$R18"]    =   18;
    registerTable["$R19"]    =   19;
    registerTable["$R20"]    =   20;
    registerTable["$R21"]    =   21;
    registerTable["$R22"]    =   22;
    registerTable["$R23"]    =   23;
    registerTable["$R24"]    =   24;
    registerTable["$R25"]    =   25;
    registerTable["$R26"]    =   26;
    registerTable["$R27"]    =   27;
    registerTable["$R28"]    =   28;
    registerTable["$R29"]    =   29;
    registerTable["$R30"]    =   30;
    registerTable["$R31"]    =   31;

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
    qlonglong maxAddress = 0;
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
        if (inputCodes[i].indexOf(";") >= 0)
            inputCodes[i].truncate(inputCodes[i].indexOf(";") + 1);
    }
    input = inputCodes.join("\n");
    input.replace(";", "\n");
    inputCodes = input.split('\n', QString::SkipEmptyParts);

    enum Mode {
        Instruction, Data
    };

    Mode mode = Instruction;
    qlonglong address = 0;

    foreach (QString str, inputCodes) {
        str = str.trimmed();
        if (str.startsWith("BASEADDRE")) {
            mode = Instruction;
            bool ok;
            address = str.split(':').at(1).trimmed().toLongLong(&ok, 16);
        } else if (str.startsWith("DATAADDRE")) {
            mode = Data;
            bool ok;
            address = str.split(':').at(1).trimmed().toLongLong(&ok, 16);
        } else if (str.contains("EQU")) {
            str.replace("EQU", " ");
            QStringList list = str.split(" ", QString::SkipEmptyParts);
            labelTable[list.at(0).trimmed()] = list.at(1).trimmed().toInt();
        } else if (str.contains("MOVE")) {
            str.replace("MOVE", "ADDI");
            str.append(",0");
            ram[address] = str.trimmed();
            address += 4;
        } else if (str.contains("LA")) {
            str.replace("LA", "");
            str = str.trimmed();
            QStringList operands = str.split(",", QString::SkipEmptyParts);
            QStringList list = imm2two(QString::number(labelTable[operands.at(1).trimmed()], 10));
            str = "LUI " + operands.at(0).trimmed() + ", " + list.at(1);
            ram[address] = str;
            address += 4;
            str = "SRL " + operands.at(0) + ", " + operands.at(0) + ", 16";
            ram[address] = str;
            address += 4;
            str = "LUI " + operands.at(0).trimmed() + ", " + list.at(0);
            ram[address] = str;
            address += 4;
        } else if (str.contains("LI")) {
            str.replace("LI", "");
            str = str.trimmed();
            QStringList operands = str.split(",", QString::SkipEmptyParts);
            QStringList list = imm2two(operands.at(1));
            str = "LUI " + operands.at(0).trimmed() + ", " + list.at(1);
            ram[address] = str;
            address += 4;
            str = "SRL " + operands.at(0) + ", " + operands.at(0) + ", 16";
            ram[address] = str;
            address += 4;
            str = "LUI " + operands.at(0).trimmed() + ", " + list.at(0);
            ram[address] = str;
            address += 4;
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
                        QMessageBox::critical(NULL , "Conflict", "Memory conflict!", QMessageBox::Ok);
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
                            QMessageBox::critical(NULL , "Conflict", "Memory conflict!", QMessageBox::Ok);
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
                            QMessageBox::critical(NULL, "Conflict", "Memory conflict!", QMessageBox::Ok);
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
        if (address > maxAddress) maxAddress = address;
    }

    for (int address = 0; address < maxAddress; address++)
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
                output += translate2bin(str, address);
                address += 3;
            }
        } else
            output += QChar(0);
    }

    return output;
}

QString Assembler::translate2bin(QString instruction, int address)
{
    QString bin = "";
    QString binstr = "";
    QString operation;
    QStringList operands;
    if (instruction.contains(" ")) {
        operation = instruction.mid(0, instruction.indexOf(" ")).trimmed();
        operands = instruction.remove(0, instruction.indexOf(" ")).replace(" ", "").split(",");
    } else {
        operation = instruction;
        operands.clear();
    }
    qDebug() << operation;
    qDebug() << operands;

    if (type1.contains(operation)) {
        if (operands.size() != 3) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
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
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        QStringList offsetWithRegister = operands.at(1).split("(");
        offsetWithRegister[1].replace(")", "");
        binstr += fillZeroOrChop(QString::number(type2[operation], 2), 6);
        binstr += fillZeroOrChop(QString::number(registerTable[offsetWithRegister.at(1)], 2), 5);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop(QString::number(offsetWithRegister.at(0).toInt(NULL, 10), 2), 16);
    } else if (type3.contains(operation)) {
        if (operands.size() != 3) {
           //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop(QString::number(type3[operation], 2), 6);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(1)], 2), 5);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += imm2bin(operands.at(2), 16);
    } else if (operation == "LUI") {
        if (operands.size() != 2) {
           //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop(QString::number(0xf, 2), 6);
        binstr += fillZeroOrChop(QString::number(0, 2), 5);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += imm2bin(operands.at(1), 16);
    } else if (type4.contains(operation)) {
        if (operands.size() != 3) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += "00000000000";
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(1)], 2), 5);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += imm2bin(operands.at(2), 5);
        binstr += fillZeroOrChop(QString::number(type4[operation], 2), 6);
    } else if (type5.contains(operation)) {
        if (operands.size() != 2) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop("0", 6);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(1)], 2), 5);
        binstr += fillZeroOrChop("0", 10);
        binstr += fillZeroOrChop(QString::number(type5[operation], 2), 6);
    } else if (operation == "BNE") {
        if (operands.size() != 3) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop(QString::number(5, 2), 6);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(1)], 2), 5);
        binstr += fillZeroOrChop(QString::number((labelTable[operands.at(2)] - (address + 4)) / 4, 2), 16);
    } else if (operation == "BEQ") {
        if (operands.size() != 3) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop(QString::number(4, 2), 6);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(1)], 2), 5);
        binstr += fillZeroOrChop(QString::number((labelTable[operands.at(2)] - (address + 4)) / 4, 2), 16);
    } else if (operation == "BGEZ") {
        if (operands.size() != 2) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop(QString::number(1, 2), 6);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop(QString::number(1, 2), 5);
        binstr += fillZeroOrChop(QString::number((labelTable[operands.at(1)] - (address + 4)) / 4, 2), 16);
    } else if (operation == "BGTZ") {
        if (operands.size() != 2) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop(QString::number(7, 2), 6);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop(QString::number(0, 2), 5);
        binstr += fillZeroOrChop(QString::number((labelTable[operands.at(1)] - (address + 4)) / 4, 2), 16);
    } else if (operation == "BLTZ") {
        if (operands.size() != 2) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop(QString::number(1, 2), 6);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop(QString::number(0, 2), 5);
        binstr += fillZeroOrChop(QString::number((labelTable[operands.at(1)] - (address + 4)) / 4, 2), 16);
    } else if (operation == "BLEZ") {
        if (operands.size() != 2) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop(QString::number(6, 2), 6);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop(QString::number(0, 2), 5);
        binstr += fillZeroOrChop(QString::number((labelTable[operands.at(1)] - (address + 4)) / 4, 2), 16);
    } else if (operation == "J") {
        if (((address + 4) & 0xf0000000) != (labelTable[operands.at(0)] & 0xf0000000)) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        } else {
            if (operands.size() != 1) {
                //QMessageBox invalid
                QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
                return "";
            }
            binstr += fillZeroOrChop(QString::number(2, 2), 6);
            binstr += fillZeroOrChop(QString::number((labelTable[operands.at(0)] & 0x0fffffff) / 4, 2), 26);
        }
    } else if (operation == "JAL") {
        if (((address + 4) & 0xf0000000) != (labelTable[operands.at(0)] & 0xf0000000)) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        } else {
            if (operands.size() != 1) { //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
                return "";
            }
            binstr += fillZeroOrChop(QString::number(3, 2), 6);
            binstr += fillZeroOrChop(QString::number((labelTable[operands.at(0)] & 0x0fffffff) / 4, 2), 26);
        }
    } else if (operation == "JALR") {
        if (operands.size() != 1) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += "000000";
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += "00000";
        binstr += "11111";
        binstr += "00000";
        binstr += fillZeroOrChop(QString::number(9, 2), 6);
    } else if (operation == "JR") {
        if (operands.size() != 1) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop("0", 6);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop("0", 15);
        binstr += fillZeroOrChop(QString::number(8, 2), 6);
    } else if (operation == "MFHI") {
        if (operands.size() != 1) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop("0", 16);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop("0", 5);
        binstr += fillZeroOrChop(QString::number(0x10, 2), 6);
    } else if (operation == "MFLO") {
        if (operands.size() != 1) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop("0", 16);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop("0", 5);
        binstr += fillZeroOrChop(QString::number(0x12, 2), 6);
    } else if (operation == "MTHI") {
        if (operands.size() != 1) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop("0", 6);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop("0", 15);
        binstr += fillZeroOrChop(QString::number(0x11, 2), 6);
    } else if (operation == "MTLO") {
        if (operands.size() != 1) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop("0", 6);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop("0", 15);
        binstr += fillZeroOrChop(QString::number(0x13, 2), 6);
    } else if (operation == "ERET") {
        if (operands.size() != 0) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += "01000010000000000000000000011000";
    } else if (operation == "BREAK") {
        if (operands.size() != 1) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += "000000";
        binstr += fillZeroOrChop(QString::number(operands.at(0).toInt(NULL, 10), 2), 20);
        binstr += "001100";
    } else if (operation == "SYSCALL") {
        if (operands.size() != 0) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += "00000000000000000000000000001100";
    } else if (operation == "MFC0") {
        if (operands.size() != 2) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop(QString::number(0x10, 2), 6);
        binstr += "00000";
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(1)], 2), 5);
        binstr += "00000000000";
    } else if (operation == "MTC0") {
        if (operands.size() != 2) {
            //QMessageBox invalid
            QMessageBox::critical(NULL, "Invalid", "Invalid instruction!", QMessageBox::Ok);
            return "";
        }
        binstr += fillZeroOrChop(QString::number(0x10, 2), 6);
        binstr += "00100";
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(0)], 2), 5);
        binstr += fillZeroOrChop(QString::number(registerTable[operands.at(1)], 2), 5);
        binstr += "00000000000";
    }

    BinaryStringTranslator translator;

    qDebug() << binstr;
    qDebug() << binstr.size();

    return translator.toBin(binstr);
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

QString Assembler::imm2bin(QString str, int len)
{
    if (str.startsWith("0X"))
        return fillZeroOrChop(QString::number(str.toInt(NULL, 16), 2), len);
    else
        return fillZeroOrChop(QString::number(str.toInt(NULL, 10), 2), len);
}

QStringList Assembler::imm2two(QString str)
{
    qlonglong imm;
    if (str.startsWith("0X"))
        imm = str.toLongLong(NULL, 16);
    else
        imm = str.toLongLong(NULL, 10);
    QStringList list;

    list.clear();
    list.append(QString::number(imm >> 16, 10));
    list.append(QString::number(imm - ((imm >> 16) << 16), 10));
    qDebug() << list << "two lists";
    return list;
}

