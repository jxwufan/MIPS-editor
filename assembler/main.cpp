#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include "assembler.h"
#include <QDebug>
#include <QString>
#include <QDataStream>
#include <QByteArray>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("input.txt");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream text(&file);

    QString input = text.readAll();

    Assembler ass;

    QString output = ass.ass2bin(input);

    QFile file2("output.txt");
    file2.open(QFile::WriteOnly | QFile::Text);

    QDataStream outputData(&file2);

    QByteArray outputByteArray = output.toLatin1();

    outputData.writeRawData(outputByteArray.data(), outputByteArray.size());

    file2.close();

    return a.exec();
}
