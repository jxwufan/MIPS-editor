#include <QCoreApplication>
#include <QTextStream>
#include <QList>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QMap>

QString ass2bin(QString input)
{
    input = input.toUpper();
    QStringList inputCodes = input.split(QChar('\n'), QString::SkipEmptyParts);
    QString output = "";

    QMap<int, QString> ram;

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

//    Mode mode = Instruction;
//    int Address;


    qDebug() << inputCodes;

    return output;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("input.txt");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream text(&file);

    QString input = text.readAll();

    //qDebug() <<
    ass2bin(input);

    return a.exec();
}
