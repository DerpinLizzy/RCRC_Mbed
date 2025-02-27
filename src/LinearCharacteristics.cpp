#include "LinearCharacteristics.h"

using namespace std;

LinearCharacteristics::LinearCharacteristics(float gain,float offset){    // standard lin characteristics
    this->gain = gain;
    this->offset = offset;
    this->ulim = 999999.0;          // a large number
    this->llim = -999999.0;         // a large neg. number
}

LinearCharacteristics::LinearCharacteristics(float xmax,float ymax,float xmin, float ymin){    // standard lin characteristics
    this->gain = (ymax - ymin) /(xmax - xmin);
    this->offset = xmax - ymax / gain;
    this->ulim = 999999.0;          // a large number
    this->llim = -999999.0;         // a large neg. number
}

LinearCharacteristics::~LinearCharacteristics() {}


float LinearCharacteristics::evaluate(float x)
{   
    return gain * (x-offset);
}
