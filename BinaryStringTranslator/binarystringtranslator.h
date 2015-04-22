#ifndef BINARYSTRINGTRANSLATOR_H
#define BINARYSTRINGTRANSLATOR_H

class QString;
class QByteArray;

class BinaryStringTranslator
{
public:
    BinaryStringTranslator();
    ~BinaryStringTranslator();

    QString toBinstr(QByteArray bin);
    QString toBin(QString binstr);
};

#endif // BINARYSTRINGTRANSLATOR_H
