#ifndef FGEN_H
#define FGEN_H

#include <QMainWindow>
#include <QObject>
#include <QSharedDataPointer>
#include <QWidget>

class FGenData;

class FGen
{
public:
    FGen();
    FGen(const FGen &);
    FGen &operator=(const FGen &);
    ~FGen();

    void sendVoltage(float);

private:
    QSharedDataPointer<FGenData> data;
};

#endif // FGEN_H
