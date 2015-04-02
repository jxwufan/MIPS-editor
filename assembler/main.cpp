#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include "assembler.h"
#include <QDebug>
#include <QString>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("input.txt");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream text(&file);

    QString input = text.readAll();

    //qDebug() <<
    Assembler ass;

    qDebug() << ass.ass2bin(input);

    return a.exec();
}
