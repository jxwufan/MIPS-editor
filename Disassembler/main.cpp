#include <QCoreApplication>
#include <QDataStream>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QChar>
#include <QByteArray>
#include "disassembler.h"
#include "binarystringtranslator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("input.txt");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray buffer(file.readAll());

    BinaryStringTranslator translator;

    Disassembler ass;

    QString output = ass.bin2ass(translator.toBinstr(buffer));

    QFile file2("output.txt");
    file2.open(QFile::WriteOnly | QFile::Text);

    QTextStream outputText(&file2);

    outputText << output;

    file2.close();

    return a.exec();
}
