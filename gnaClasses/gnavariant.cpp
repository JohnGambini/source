#include "gnavariant.h"

gnaVariant::gnaVariant()
{
    m_long = 0;
    m_type = 0;
}

gnaVariant::gnaVariant(const GNACHAR * value)
{
    m_string = value;
    m_long = 0;
    m_type = 0;
}

gnaVariant::gnaVariant( const gnaVariant & right)
{
    m_string = right.m_string;
    m_long = right.m_long;
    m_type = right.m_type;
}

gnaVariant::gnaVariant( const GNASTRING & value)
{
    m_string.assign(value);
    m_long = 0;
    m_type = 0;
}

gnaVariant::gnaVariant( const gnaString & value)
{
    m_string = value;
    m_long = 0;
    m_type = 0;
}

gnaVariant::gnaVariant(gnaint32 value)
{
    m_string = "";
    m_long = value;
    m_type = 1;
}

const GNACHAR * gnaVariant::toString()
{
    return m_string.c_str();
}

gnaint32 gnaVariant::toInt()
{
    return m_long;
}

gnaVariant & gnaVariant::operator = (const GNACHAR * string)
{
    m_string = string;
    return *this;
}

gnaVariant & gnaVariant::operator = (const gnaString & astring)
{
    m_string = astring;
    return *this;
}

gnaVariant & gnaVariant::operator = (const int & right)
{
    m_long = right;
    m_string = "";
    return *this;
}

void gnaVariant::Serialize(gnaFile & fl)
{
    if( !fl.is_open())
        return;

    m_string.Serialize(fl);

    if(fl.isStoring())
    {
        fl.write((GNACHAR *)&m_long,sizeof(m_long));
        fl.write((GNACHAR *)&m_type,sizeof(m_type));
    }
    else
    {
        fl.read((GNACHAR *)&m_long,sizeof(m_long));
        fl.read((GNACHAR *)&m_type,sizeof(m_type));
    }
}

