#ifndef GNASTRING_H
#define GNASTRING_H

#include "gnaFile.h"

class GNALIBSHARED_EXPORT gnaString : public GNASTRING
{
public:
    gnaString();
    virtual ~gnaString();
    gnaString(const gnaString & right);
    gnaString(const GNACHAR * right);
    gnaString(const GNASTRING & right);
    const char * toMBS() const;

    gnaString & operator = (const GNACHAR * value);
    gnaString & operator = (const GNASTRING & value);
    virtual void Serialize(gnaFile & fl );

private:
    mutable char * m_buffMBS;
};

#endif // GNASTRING_H
