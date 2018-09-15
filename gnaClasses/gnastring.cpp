#include "gnastring.h"
#include <stdlib.h>
#include <string.h>

gnaString::gnaString()
{
    m_buffMBS = NULL;
}

gnaString::gnaString(const gnaString & right)
{
    m_buffMBS = NULL;
    assign(right.c_str());
}

gnaString::gnaString(const GNACHAR * right)
{
    m_buffMBS = NULL;
    assign(right);
}

gnaString::gnaString(const GNASTRING & right)
{
    m_buffMBS = NULL;
    assign(right);
}

gnaString::~gnaString()
{
    if( m_buffMBS != NULL )
        delete [] m_buffMBS;
}

const char * gnaString::toMBS() const
{
#ifdef GNALIB_WIDECHAR
    if( m_buffMBS )
        delete [] m_buffMBS;
    m_buffMBS = new char[length()+1];
    wcstombs(m_buffMBS, c_str(), length());
    return m_buffMBS;
#else
    return c_str();
#endif
}

gnaString & gnaString::operator = (const GNACHAR * value)
{
    assign(value);
    return *this;
}

gnaString & gnaString::operator = (const GNASTRING & value)
{
    assign(value);
    return *this;
}

void gnaString::Serialize(gnaFile & fl)
{
	std::streamsize stringSize = size();

    if(!fl.is_open())
        return;

    if( fl.isStoring())
    {
        fl.write((GNACHAR *)&stringSize, sizeof(size_t));
        fl.write(c_str(), stringSize);
    }
    else
    {
        fl.read((GNACHAR *)&stringSize, sizeof(size_t));
        GNACHAR * buffer = new GNACHAR[stringSize+1];
        fl.read(buffer, stringSize);
        buffer[stringSize] = '\0';
        assign(buffer);
        delete [] buffer;
    }
}
