#ifndef GNAFILE_H
#define GNAFILE_H

#include "gnaGlobal.h"
#include <fstream>
#include <strings.h>

class GNALIBSHARED_EXPORT gnaFile : public GNAFSTREAM
{
public:
    gnaFile();
    virtual ~gnaFile();

    bool Open(GNASTRING filename, gna::FileMode mode);
    bool isStoring() { return ! m_isReading; }
    bool writeString(const GNACHAR * strng);

	gnaint8 m_fileMajorVersion;
	gnaint8 m_fileMinorVersion;


private:
    bool m_isReading;
    gnaint8 m_majorVersion;
    gnaint8 m_minorVersion;
    bool    m_isWideChar;
};

#endif // GNAFILE_H
