/***************************************************************************
***************************************************************************/

#include "gnastring.h"



gnaFile::gnaFile()
{
    m_majorVersion = 0;
    m_minorVersion = 1;
	m_fileMajorVersion = m_majorVersion;
	m_fileMinorVersion = m_minorVersion;
    m_isWideChar = WCHARLIB;
}

gnaFile::~gnaFile()
{
    if( is_open())
        close();
}

bool gnaFile::Open(GNASTRING filename, gna::FileMode mode)
{
    gnaString flnm = filename;

    if( mode == gna::FileRead)
    {
        open(flnm.toMBS(), ios::in | ios::binary );
        if( is_open())
        {
            m_isReading = true;
            seekg (0, ios::beg);
            read((GNACHAR *)&m_isWideChar, sizeof(bool));
            read((GNACHAR *)&m_fileMajorVersion, sizeof(gnaint8));
            read((GNACHAR *)&m_fileMinorVersion, sizeof(gnaint8));
            return true;
        }
        else
            return false;
    }

    if( mode == gna::FileWrite)
    {
        open( flnm.toMBS(), ios::out | ios::binary);
        if( is_open())
        {
            m_isReading = false;
            seekp(0,ios::beg);
            write((GNACHAR *)&m_isWideChar, sizeof(bool));
            write((GNACHAR *)&m_majorVersion, sizeof(gnaint8));
            write((GNACHAR *)&m_minorVersion, sizeof(gnaint8));
            return true;
        }
        else
            return false;

    }

    return false;

}


bool gnaFile::writeString(const GNACHAR * strng)
{
    streambuf * outBuf = rdbuf();
    outBuf->sputn (strng, GNASTRLEN(strng));
    flush();
    return true;
}
