#include <QCoreApplication>
#include "coetranslator.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString bin = "00000000000000000000000000100000"
                  "10001101001000001111111111111100"
                  "10000101010000000000000000010000"
                  "00100001001000001111111111111110"
                  "00110100011000101111111111111110"
                  "00111100000010010000000001111011"
                  "00000000000010100100101010000000";


    COETranslator trans;

    qDebug() << trans.bin2coe(bin);

    qDebug() << trans.coe2bin(trans.bin2coe(bin));

    QString test = "memory_initialization_radix = 16;\
    memory_initialization_vector = 0000,0020,8d20,fffc,8540,0010,2120,fffe,\
    3462,fffe,3c09,007b,000a,4a80,012c,001a,\
    152a,fffb,1002,fff8,0421,fff9,1c20,fff8,\
    0420,fff7,1820,fff6,0800,0003,0c00,0003,\
    0120,0008,0040,f809,0000,4810,0140,0011,\
    0000,5812,0180,0013,4089,1000,400b,1000,\
    4200,0018,0000,000c,0000,030c;";

    qDebug() << trans.coe2bin(test);
    qDebug() << trans.coe2bin(test).size();


    return a.exec();
}
