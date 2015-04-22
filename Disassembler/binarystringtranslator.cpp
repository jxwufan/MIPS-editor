#include "binarystringtranslator.h"
#include <QString>
#include <QByteArray>

BinaryStringTranslator::BinaryStringTranslator()
{

}

BinaryStringTranslator::~BinaryStringTranslator()
{

}

QString BinaryStringTranslator::toBinstr(QByteArray bin)
{
    QString binstr = "";
    for (int i = 0; i < bin.size(); i++) {
        char chr = bin.at(i);
        if (chr & 1 << 7) binstr += "1"; else binstr += "0";
        if (chr & 1 << 6) binstr += "1"; else binstr += "0";
        if (chr & 1 << 5) binstr += "1"; else binstr += "0";
        if (chr & 1 << 4) binstr += "1"; else binstr += "0";
        if (chr & 1 << 3) binstr += "1"; else binstr += "0";
        if (chr & 1 << 2) binstr += "1"; else binstr += "0";
        if (chr & 1 << 1) binstr += "1"; else binstr += "0";
        if (chr & 1 << 0) binstr += "1"; else binstr += "0";
    }
    return binstr;
}

QString BinaryStringTranslator::toBin(QString binstr)
{
    QString bin = "";

    while (!binstr.isEmpty()) {
        bin += char(binstr.mid(0, 8).toInt(NULL, 2));
        binstr.remove(0, 8);
    }

    return bin;
}

