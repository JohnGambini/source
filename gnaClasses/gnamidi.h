#ifndef GNAMIDI_H
#define GNAMIDI_H

#include "gnaGlobal.h"
#include "gnaTreeItem.h"

class GNALIBSHARED_EXPORT gnaMidi : public gnaTreeItem
{
public:
    gnaMidi();
    gnaMidi(vector<gnaVariant> & data);
    int parse(char const * midiData, unsigned long size );
};

#endif // GNAMIDI_H
