// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Graphematical Module (www.aot.ru)
// ==========  Copyright by Alexey Sokirko (1996-2001)


#ifndef StdAfxGraphan
#define StdAfxGraphan

#ifdef WIN32

 #if _MSC_VER > 1000
 #pragma once
 #endif // _MSC_VER > 1000
 
 #define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#endif

#pragma warning (disable  : 4530)
#pragma warning (disable  : 4786)

#include "../common/utilit.h"
#include "../common/Graspace.h"
#include "Descriptors.h"


#include "../StructDictLib/Ross.h"


#define RD_MODE    (unsigned) 'r'
#define WR_MODE    (unsigned) 'w'

inline FILE* MOpen (const char *FName, int Mode)
{
	FILE *fp;
	while (isspace (*FName)) FName ++;
	size_t l = strlen (FName);
	if ((l == 0) || (l > 255))
	  throw CExpc ("Cannot read file");

	if (Mode == RD_MODE )
	 {
		fp = fopen (FName,"rb");
		if (!fp) throw CExpc ("Cannot read file");
		else return (fp);
	 }

	if (Mode == WR_MODE)
	 {
     fp = fopen(FName,"wb");
	 if (!fp) throw CExpc ("Cannot read file");
		else return (fp);
     return fp;
    }
  return NULL;
}



// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B1840C17_0F2A_4232_9A53_B53EBD2DA11C__INCLUDED_)
