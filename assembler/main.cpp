#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include "assembler.h"
#include <QDebug>
#include <QString>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    QFile file("input.txt");
//    file.open(QFile::ReadOnly | QFile::Text);
//    QTextStream text(&file);

//    QString input = text.readAll();

//    Assembler ass;

//    QString output = ass.ass2bin(input);

//    QFile file2("output.txt");
//    file2.open(QFile::WriteOnly | QFile::Text);

//    QTextStream outputtext(&file2);

//    outputtext << output;
//    file2.close();
    qDebug() << QString("    \t\t   ").replace(" ", "");

    return a.exec();
}
