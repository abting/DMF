#ifndef NEMESYS_H
#define NEMESYS_H


class Nemesys
{
public:
    Nemesys();

    void testing();

    void openConnection();

    void closeConnection();

    void setActiveFlowUnit(unsigned char);

    void setActiveVolumeUnit(unsigned char);

    void DoseVolume(double, double);

    void EmptySyringe(double);
};

#endif // NEMESYS_H
