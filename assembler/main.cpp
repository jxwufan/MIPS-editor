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
    QMap<QString, int> labelTable;
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
            }
            if (mode == Instruction) {
                for (int i = 0; i < 4; i++)
                    if (ram.contains(address + i)) {
                        //QMessageBox conflict
                        return "";
                    }
                ram[address] = str;
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
                        ram[address + i * 2] = list.at(i).trimmed();
                    }
                    address += list.size() * 2;
                }
            }
        }
    }

    for (int i = 0; i < 200; i++)
        if (ram.contains(i)) qDebug() << "line\t" << i << "\t" << ram[i];

    foreach (QString label, labelTable.keys())
        qDebug() << label;

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
