#include "fgen.h"
#include <QDebug>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>

class FGenData : public QSharedData
{
public:

};

FGen::FGen() : data(new FGenData)
{

}

FGen::FGen(const FGen &rhs) : data(rhs.data)
{

}

FGen &FGen::operator=(const FGen &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

FGen::~FGen()
{

}

void FGen::sendVoltage(float voltage)
{
    //necessary commands to connect to the funtion generator
}
