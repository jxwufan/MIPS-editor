#include "coetranslator.h"
#include <QString>
#include <QStringList>
#include <cmath>
#include <QDebug>

COETranslator::COETranslator()
{

}

COETranslator::~COETranslator()
{

}

QString COETranslator::bin2coe(QString bin)
{
    QString coe = "memory_initialization_radix = 16;\nmemory_initialization_vector = ";
    bool hasColon = false;
    while (!bin.isEmpty()) {
        for (int i = 0; i < 32; i++) {
            if (!bin.isEmpty()) {
                coe += QString::number(bin.mid(0, 4).toInt(NULL, 2), 16);
                bin.remove(0, 4);
                if (i % 4 == 3) {
                    coe += ",";
                    hasColon = true;
                } else hasColon = false;
            }
        }
        if (!bin.isEmpty()) coe += "\n";
    }
    if (hasColon) coe.chop(1);
    coe += ";";
    return coe;
}

QString COETranslator::coe2bin(QString coe)
{
    QString bin = "";
    QStringList lines = coe.split("\n", QString::SkipEmptyParts);
    while ((!lines.isEmpty()) && lines.at(0).startsWith(";"))
        lines.removeFirst();
    coe = lines.join("");
    coe.replace(" ", "");
    coe.replace(",", "");
    lines = coe.split(";", QString::SkipEmptyParts);
    if (lines.size() != 2) {
        //QMessageBox invalid
        return "";
    } else {
        int radix = lines.at(0).split("=", QString::SkipEmptyParts).at(1).toInt(NULL, 10);
        QString data = lines.at(1).split("=", QString::SkipEmptyParts).at(1);
        qDebug() << data;
        qDebug() << data.size();
        qDebug() << (int)log2((double)radix);
        if (radix == 2) bin = data;
        else {
            while (!data.isEmpty()) {
                bin += addZeroOrchop(QString::number(data.mid(0, 1).toInt(NULL, radix), 2), (int)log2((double)radix));
                data.remove(0, 1);
            }
        }
    }
    return bin;
}

QString COETranslator::addZeroOrchop(QString str, int len)
{
    while (str.size() < len)
        str = "0" + str;
    if (str.size() > len) {
        str = str.mid(str.size() - len, len);
    }
    return str;
}

