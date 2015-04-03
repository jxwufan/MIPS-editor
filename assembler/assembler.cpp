#include "assembler.h"

#include <QList>
#include <QString>
#include <QStringList>
#include <QDebug>

Assembler::Assembler()
{
    registerTable.clear();
    registerTable["$ZERO"] = 0;
    registerTable["$AT"] = 1;
    registerTable["$V0"] = 2;
    registerTable["$V1"] = 3;
    registerTable["$A0"] = 4;
    registerTable["$A1"] = 5;
    registerTable["$A2"] = 6;
    registerTable["$A3"] = 7;
    registerTable["$T0"] = 8;
    registerTable["$T1"] = 9;
    registerTable["$T2"] = 10;
    registerTable["$T3"] = 11;
    registerTable["$T4"] = 12;
    registerTable["$T5"] = 13;
    registerTable["$T6"] = 14;
    registerTable["$T7"] = 15;
    registerTable["$S0"] = 16;
    registerTable["$S1"] = 17;
    registerTable["$S2"] = 18;
    registerTable["$S3"] = 19;
    registerTable["$S4"] = 20;
    registerTable["$S5"] = 21;
    registerTable["$S6"] = 22;
    registerTable["$S7"] = 23;
    registerTable["$T8"] = 24;
    registerTable["$T9"] = 25;
    registerTable["$K0"] = 26;
    registerTable["$K1"] = 27;
    registerTable["$GP"] = 28;
    registerTable["$SP"] = 29;
    registerTable["$FP"] = 30;
    registerTable["$RA"] = 31;
}

Assembler::~Assembler()
{

}

QString Assembler::ass2bin(QString input)
{
    input = input.toUpper();
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
                    output += QChar(str.mid(str.size() - 2, 2).toInt(NULL, 16));
                    str.chop(2);
                }
            } else {
                output += QChar(0);
                output += QChar(0);
                output += QChar(0);
                output += QChar(0);
                address += 3;
            }
        } else
            output += QChar(0);
    }

    return output;
}

