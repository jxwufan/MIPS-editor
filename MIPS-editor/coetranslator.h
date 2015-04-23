#ifndef COETRANSLATOR_H
#define COETRANSLATOR_H

class QString;

class COETranslator
{
public:
    COETranslator();
    ~COETranslator();
    QString bin2coe(QString bin);
    QString coe2bin(QString coe);
private:
    QString addZeroOrchop(QString str, int len);
};

#endif // COETRANSLATOR_H
