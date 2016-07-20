#ifndef NEMESYS_H
#define NEMESYS_H


class Nemesys
{
public:
    Nemesys();

    void openConnection();

    void closeConnection();

    void setActiveFlowUnit(unsigned char, unsigned char);

    void setActiveVolumeUnit(unsigned char, unsigned char);

    void DoseVolume(unsigned char, double, double);

    void EmptySyringe(unsigned char, double);

    void RefillSyringe(unsigned char, double);

    long CheckDosingStatus(unsigned char);

    long CheckCalibrateStatus(unsigned char);

    void CalibrateUnit(unsigned char);

    void StopUnit(unsigned char);

    double getActualFlowRate(unsigned char);

    double getActualSyringeLevel(unsigned char);

    void getSyringeLevels(unsigned char, double, double);


};

#endif // NEMESYS_H
