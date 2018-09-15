#ifndef GNACLASSES_GLOBAL_H
#define GNACLASSES_GLOBAL_H

#if ! defined(STATIC)
#if defined(GNALIB_LIBRARY)
#  define GNALIBSHARED_EXPORT __declspec(dllexport)
#else
#  define GNALIBSHARED_EXPORT __declspec(dllimport)
#endif
#else
#   define GNALIBSHARED_EXPORT
#endif

#if defined(GNALIB_WIDECHAR) || defined(_UNICODE)
#undef GNALIB_WIDECHAR
#define GNALIB_WIDECHAR
#define WCHARLIB 1
#define GNAFSTREAM wfstream
# define GNACHAR wchar_t
# define GNASTRING wstring
# define GNAL(x) L ## x
# define GNASTRCMP wcscmp
# define GNASTRLEN wcslen
#else
# define WCHARLIB 0
# define GNAFSTREAM fstream
# define GNACHAR char
# define GNASTRING string
# define GNAL(x) (x)
# define GNASTRCMP strcmp
# define GNASTRLEN strlen
#endif

namespace gna
{
    enum FileMode
    {
        FileRead,
        FileWrite
    };

    enum DataType
    {
        Version,
        String,
        Long
    };

	enum EditFlag
	{
		CanEdit = 0x0001,
		CanDelete = 0x0002,
		ShowChildren = 0x0004,
		AddChildren = 0x0008,
		Animate = 0x0010,
		AddFiles = 0x0020
	};
}

typedef unsigned short gnaint16;
typedef unsigned long gnaint32;
typedef unsigned char gnaint8;

#include <string>
using namespace std;

#if defined(_WINDOWS)
#pragma warning (disable : 4251 )
#pragma warning (disable : 4800 )
#endif

#endif // GNACLASSES_GLOBAL_H
