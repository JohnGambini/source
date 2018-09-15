#ifndef GNAVARIANT_H
#define GNAVARIANT_H

#include "gnaGlobal.h"
#include "gnaFile.h"
#include "gnaString.h"

class GNALIBSHARED_EXPORT gnaVariant
{
public:
    gnaVariant();
    gnaVariant( const gnaVariant & right);
    gnaVariant( const GNACHAR * value);
    gnaVariant( const GNASTRING & value);
    gnaVariant( const gnaString & value);
    gnaVariant( gnaint32 value );
    const GNACHAR * toString();
    gnaint32 toInt();
    gnaVariant & operator = (const GNACHAR * right);
    gnaVariant & operator = (const gnaString & right);
    gnaVariant & operator = (const int & right);

    virtual void Serialize(gnaFile & fl);

private:
    gnaString m_string;
    gnaint32 m_long;
    gnaint32 m_type;
};

#endif // GNAVARIANT_H
