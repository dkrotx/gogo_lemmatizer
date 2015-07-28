// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Syntax Analysis (www.aot.ru)
// ==========  Copyright by Alexey Sokirko

#ifndef utilit_h
 #define utilit_h

#include  <stdio.h>




#ifdef WIN32

	// it is used to find memory leaks
	#ifdef DETECT_MEMORY_LEAK
		extern void* operator new(size_t nSize, const char* lpszFileName, int nLine);
		extern void  operator delete(void* p, const char* lpszFileName, int nLine);
		#define DEBUG_NEW new(THIS_FILE, __LINE__)
	#endif
	

	#include "windows.h"
	#include "winuser.h"
#endif

#include  <assert.h>
#include  <stdio.h>
#include  <stdarg.h>

#include  <ctype.h>
#include  <string>
#include  <cstring>
#include  <algorithm>
#include  <vector>
#include  "set"
#include  "stack"
#include  "map"
#include "time.h"


// #pragma warning (disable : 4018)
// #pragma warning (disable : 4244)
// #pragma warning (disable : 4103)
// #pragma warning  (disable : 4530)
// #pragma warning  (disable : 4251)

#ifdef STLPORT
	using namespace stlport;
#else
	using namespace std;
#endif





//#if !defined (STLPORT) || defined(__STL_USE_NAMESPACES)
//using namespace std;
//#endif



typedef unsigned char BYTE;

#ifdef WIN32
	#include  <io.h>
	#include <fcntl.h>
	typedef __int64 QWORD;
	
#else
	#include  <unistd.h>
	typedef unsigned int	DWORD;	
	typedef unsigned short	WORD;
	typedef long long		QWORD;
	typedef unsigned int	UINT;
	const   unsigned int _MAX_PATH = 512;
#endif

typedef vector<string> StringVector;
typedef set<string> StringSet;
typedef vector<DWORD> DwordVector;

inline QWORD GetMaxQWORD ()
{
	#ifdef WIN32
		return  0xffffffffffffffff;
	#else
		return  0xffffffffffffffffULL;
	#endif
};

const BYTE   cHyphenChar   = (BYTE) '-';
const BYTE   cPeriodChar   = (BYTE) '.';
const BYTE   cEofChar      = (BYTE) 0x1a;
const BYTE   cPtrChar      = (BYTE) 0x13;
const BYTE   cSpaceChar    = (BYTE) 32;
const BYTE   cBackChar     = (BYTE) 'Ђ';
const BYTE   cFullChar    = (BYTE) '«';
const BYTE   cNumChar      = (BYTE) 'N';
const BYTE   cAmpChar      = (BYTE) '&';
const BYTE   cParagraph    = (BYTE) 21;
const BYTE   cCompanyChar  = (BYTE) 176;
const BYTE   cIonChar      = (BYTE) 183;
const BYTE   cNumberChar   = (BYTE) 0xB9;
const BYTE   cPiChar       = (BYTE) 182;
const BYTE   cPseudoColon  = (BYTE) 133;

// morphology&syntax
typedef enum {	morphUnknown = 0, morphRussian = 1, morphEnglish = 2,morphGerman = 3} MorphLanguageEnum;


class CExpc
{
 public:
	string	m_strCause;
	int		m_ErrorCode;

	CExpc(const string& Cause, int ErrorCode = -1);
	CExpc(const CExpc& from);
	CExpc& operator= (const CExpc& from);
};


// working with files
extern bool		FileExists (const char *FName);
extern size_t	FileSize (const char *filename);
extern void		ErrorMessage (const string& Titul, const string& Message);
extern void		ErrorMessage (const string& Message);
extern string	MakeFName ( const string& InpitFileName,  const string& Ext);
extern bool		MakePath (char* RossPath, char* FileName, char* FullPath);
extern string	GetPathByFile (string FileName);
extern bool		IsBinFile (const char* FileName);
extern bool		IsEmptyLine ( const char *t);
extern bool		IsHtmlFile (const string& FileName);
extern void		AddFile(const char* MainFile, const char* ToAdd);
extern string	CreateTempFileName();


// working with registry 
extern string	GetRegistryString (string RegistryPath);
extern string	GetRegistryStringFromLocalIniFile (string RegistryPath);
extern bool		CanGetRegistryString (string RegistryPath);
extern void		SetRegistryString (string RegistryPath, string Value);
extern bool		IsRmlRegistered(string& Error);
extern bool		CheckEvaluationTime();
extern struct tm  RmlGetCurrentTime ();
extern bool		ReadTimeOutFromRegistry(bool bReadFromLocalFile, int& TimeOut);
extern string	GetIniFilePath();

// working with strings
extern char*	rtrim (char* s);
extern bool		IsSuperEqualChar (BYTE ch1, BYTE ch2, MorphLanguageEnum langua);
extern bool		strscmp ( const char *s1, const char *s2, size_t l, MorphLanguageEnum langua);
extern int		CompareWithoutRegister ( const char *s1, const char *s2, size_t l, MorphLanguageEnum langua);
extern BYTE		force_rus_char (BYTE ch);
extern bool		force_to_rus (char* dest, const char* sour, size_t len);
extern char*	IntToStr (int Value, char* Buffer);
extern string&	IntToStr (int Value, string& oBuffer);
extern string	Format( const char* format, ... );
extern string&  TrimLeft (string& str);
extern string&  TrimRight (string& str);
extern string&	Trim (string& str);
extern void		rml_TRACE( const char* format, ... );
extern bool		LoadFileToString(string FileName, string& Result);
extern void		SqueezeSpacesAndDeleteEoln(string& s);
extern void		KOI8ToWin (string& s);
extern void		WinToKOI8 (string& s);
extern DWORD	StringCrc32(const char* szString);



extern void QPEncodeString(string& s);
extern void QPDecodeString(string& s);
#ifndef WIN32
	extern void strrev(char* s);
#endif

const BYTE	UnknownPartOfSpeech		= 0xff;
const int	UnknownSyntaxElement = 0xffff;
typedef enum {DontKillHomonyms = 0, CoverageKillHomonyms = 1} KillHomonymsEnum;

typedef enum { LocThes = 0, FinThes, CompThes, OmniThes, NoneThes } EThesType;
typedef enum { EClause = 0, EWord, EGroup, ENoneType } EUnitType;


extern int GetPredictionPartOfSpeech(const string& PartOfSpeech, MorphLanguageEnum langua);
extern bool GetLanguageByString (string s, MorphLanguageEnum& Result);
extern string GetStringByLanguage (MorphLanguageEnum Langua);



// ==============   Saving and restoring vectors  ======================

// ============== DWORD =====================
inline size_t get_size_in_bytes (const DWORD& /*t*/)
{
	return 4;
};
inline size_t save_to_bytes(const DWORD& i, BYTE* buf)
{
	*((DWORD*)buf) = i;
	return get_size_in_bytes(i);
}

inline size_t restore_from_bytes(DWORD& i, const BYTE* buf)
{
	i = *((DWORD*)buf);
	return get_size_in_bytes(i);
};



// ============== int =====================
inline size_t get_size_in_bytes (const int& /*t*/)
{
	return 4;
};
inline size_t save_to_bytes(const int& i, BYTE* buf)
{
	*((int*)buf) = i;
	return get_size_in_bytes(i);
}

inline size_t restore_from_bytes(int& i, const BYTE* buf)
{
	i = *((int*)buf);
	return get_size_in_bytes(i);
};


// ============== CHAR =====================
inline size_t get_size_in_bytes (const char& /*t*/)
{
	return 1;
};

inline size_t save_to_bytes(const char& i, BYTE* buf)
{
	*((char*)buf) = i;
	return get_size_in_bytes(i);
};

inline size_t restore_from_bytes(char& i, const BYTE* buf)
{
	i = *((char*)buf);
	return get_size_in_bytes(i);
};


// ============== BYTE =====================
inline size_t get_size_in_bytes (const BYTE& /*t*/)
{
	return 1;
};

inline size_t save_to_bytes(const BYTE& i, BYTE* buf)
{
	*((BYTE*)buf) = i;
	return get_size_in_bytes(i);
};

inline size_t restore_from_bytes(BYTE& i, const BYTE* buf)
{
	i = *((BYTE*)buf);
	return get_size_in_bytes(i);
};



// ============== WORD =====================
inline size_t get_size_in_bytes (const WORD& /*t*/)
{
	return 2;
};

inline size_t save_to_bytes(const WORD& i, BYTE* buf)
{
	*((WORD*)buf) = i;
	return get_size_in_bytes(i);
};

inline size_t restore_from_bytes(WORD& i, const BYTE* buf)
{
	i = *((WORD*)buf);
	return get_size_in_bytes(i);
};


// ============== QWORD =====================
inline size_t get_size_in_bytes (const QWORD& /*t*/)
{
	return 8;
};

inline size_t save_to_bytes(const QWORD& i, BYTE* buf)
{
	*((QWORD*)buf) = i;
	return get_size_in_bytes(i);
};

inline size_t restore_from_bytes(QWORD& i, const BYTE* buf)
{
	i = *((QWORD*)buf);
	return get_size_in_bytes(i);
};

// ============== PAIR =====================
template <class T>
inline size_t get_size_in_bytes (const pair<T, T>& t)
{
	return get_size_in_bytes(t.first) * 2;
};

template <class T>
inline size_t save_to_bytes(const pair<T, T>& t, BYTE* buf)


{
	buf += save_to_bytes(t.first, buf);
	buf += save_to_bytes(t.second, buf);
	return get_size_in_bytes(t);
};

template <class T>
inline size_t restore_from_bytes(pair<T, T>& t, const BYTE* buf)
{
	buf += restore_from_bytes(t.first, buf);
	buf += restore_from_bytes(t.second, buf);
	return get_size_in_bytes(t);
};


// ============== TRIPLE =====================

template <class T1, class T2, class T3>
struct	troika : public pair<T1, T2>
{
	T3 third;
};

template <class T>
inline size_t get_size_in_bytes (const troika<T, T, T>& t)
{
	return get_size_in_bytes(t.first) * 3;
};

template <class T>
inline size_t save_to_bytes(const troika<T, T, T>& t, BYTE* buf)
{
	buf += save_to_bytes(t.first, buf);
	buf += save_to_bytes(t.second, buf);
	buf += save_to_bytes(t.third, buf);
	return get_size_in_bytes(t);
};

template <class T>
inline size_t restore_from_bytes(troika<T, T, T>& t, const BYTE* buf)
{
	buf += restore_from_bytes(t.first, buf);
	buf += restore_from_bytes(t.second, buf);
	buf += restore_from_bytes(t.third, buf);
	return get_size_in_bytes(t);
};


const int VectorMaxStructSize = 200;

template <class T>
void ReadVectorInner (FILE* fp, vector<T>& V, size_t Count)
{
	#ifdef  WIN32
		if (_setmode( _fileno( fp ), _O_BINARY ) != _O_BINARY) 
		{
			ErrorMessage("ReadVectorInner", "File is not opened in binary mode!");
			return;
		};
	#endif


	T dummy;
	size_t size_of_t = get_size_in_bytes(dummy);

	BYTE buffer[VectorMaxStructSize];
	assert (size_of_t < VectorMaxStructSize);

	V.clear();
	V.reserve(Count);

	for (int i = 0; i < Count; i++)
	{
		fread ((void*)buffer, size_of_t, 1, fp);
		restore_from_bytes (dummy, buffer);
		V.push_back(dummy);
	};
};

template <class T>
inline void ReadVector (const string& FileName, vector<T>& V)
{
	V.clear();
	size_t sz = FileSize(FileName.c_str());

	FILE* fp = fopen (FileName.c_str(),"rb");
	if (!fp) return;
 	
	T dummy;
	size_t size_of_t = get_size_in_bytes(dummy);
	size_t Count = sz/size_of_t;
	ReadVectorInner(fp, V, Count);
	fclose (fp);
};


template <class T>
bool WriteVectorInner (FILE* fp, const vector<T>& V)
{
	#ifdef  WIN32
		if (_setmode( _fileno( fp ), _O_BINARY ) != _O_BINARY) 
		{
			ErrorMessage("WriteVectorInner", "File is not opened in binary mode!");
			return false;
		};
	#endif

	T dummy;
	size_t size_of_t = get_size_in_bytes(dummy);

	BYTE buffer[VectorMaxStructSize];
	assert (size_of_t < VectorMaxStructSize);

	size_t  count = V.size();

	for (size_t i =0; i < count; i++)
	{
		save_to_bytes(V[i], buffer);
		if (!fwrite((void*)buffer, size_of_t, 1, fp)) return false;
	};
	return true;
};


template <class T>
inline bool WriteVector (const string& FileName, const vector<T>& V)
{
	FILE* fp = fopen (FileName.c_str(),"wb");
	if (!fp)
	{
		ErrorMessage ("Cannot write vector to "+FileName);
		return false;
	};
	bool b = WriteVectorInner(fp, V);
	fclose (fp);
	return b;
};



/*
clears also capacity
*/
template <class T> 
void ClearVector(vector<T>& V)
{
	V.clear();
	vector<T> dummy (V);
	V.swap (dummy);
};





// ============     Brackets =========================


const char brackets[2][6] = {"{[(<\0","}])>\0"};
extern int isbracket  (BYTE x);
extern int dual_bracket (BYTE x);



//==================    Digits   ===========================
const char UpRomDigits[] = "IVXLШ\0";
const char LwRomDigits[] = "ixlv\0";

extern bool is_upper_roman_digit (BYTE ch);
extern bool is_lower_roman_digit (BYTE ch);
extern bool is_roman_number ( const char *s, size_t len);
extern bool is_pseudo_graph(BYTE x);
extern bool is_word_delim (BYTE x);
//  =============  Punctuation Letters ======================

extern	bool isEOS (BYTE ch);
extern	bool isEOP (BYTE ch);
extern	bool is_spc_fill (BYTE x);




extern  bool is_english_upper(BYTE x);
extern  bool is_english_lower(BYTE x);

const BYTE Auml  = 196; // "Д"
const BYTE auml  = 228; // "д"
const BYTE Uuml  = 220; // "Ь"
const BYTE uuml  = 252; // "ь"
const BYTE Ouml  = 214; // "Ц"
const BYTE ouml  = 246; // "ц"
const BYTE szlig  = 223; //"Я"
const BYTE Nu  = 181;   // "ч"
const BYTE agrave  = 224; //"р"
const BYTE egrave  = 232; //"ш"
const BYTE eacute  = 233; //"щ"


const BYTE LowerJO  = (BYTE)'ё';
const BYTE UpperJO  = (BYTE)'Ё';
const BYTE Apostrophe  = 39;



 
extern  bool is_german_upper(BYTE x); 
extern  bool is_german_lower(BYTE x);
extern  bool is_russian_upper(BYTE x);     
extern  bool is_russian_lower(BYTE x);

extern  bool is_lower_consonant(BYTE x, MorphLanguageEnum Langua);
extern  bool is_upper_consonant(BYTE x, MorphLanguageEnum Langua);
extern  bool is_lower_vowel(BYTE x, MorphLanguageEnum Langua);
extern  bool is_upper_vowel(BYTE x, MorphLanguageEnum Langua);

extern  bool is_english_alpha(BYTE x);
extern  bool is_russian_alpha(BYTE x);
extern  bool is_german_alpha(BYTE x);

extern  bool is_alpha (BYTE x);
extern  bool is_alpha (BYTE x, MorphLanguageEnum langua);
extern  bool is_lower_alpha(BYTE x, MorphLanguageEnum langua);
extern  bool is_upper_alpha(BYTE x, MorphLanguageEnum langua);
extern  bool isnspace(BYTE x);


// ===============  Register ========================================
extern  BYTE etoupper (BYTE ch);
extern  BYTE etolower (BYTE ch);
extern  BYTE rtoupper (BYTE ch); 
extern  BYTE rtolower (BYTE ch); 
extern  BYTE gtoupper (BYTE ch); 
extern  BYTE gtolower (BYTE ch); 
extern  BYTE ReverseChar (BYTE ch, MorphLanguageEnum langua);
extern char* RusMakeUpper (char *word);
extern char* EngMakeUpper (char *word);
extern string& EngMakeUpper (string& word);
extern string& EngMakeLower (string& word);
extern char* GerMakeUpper (char *word);
extern string& GerMakeUpper (string& word);
extern char* RusMakeLower (char *word);
extern string& EngRusMakeLower (string& word);
extern char* EngRusMakeLower (char* word);
extern char* RmlMakeUpper (char *word, MorphLanguageEnum langua);
extern string& RmlMakeUpper (string& word, MorphLanguageEnum langua);
extern string& RmlMakeLower (string& word, MorphLanguageEnum langua);

// конвертирует из строчной кириллицы в прописные 
extern string&  EngRusMakeUpper (string& word);
// конвертирует из строчной кириллицы в прописные 
extern char*  EngRusMakeUpper (char* word);


// check languaage 
extern bool IsRussian (const char *word); 
extern bool IsRussian (const string& word);
extern bool IsEnglish (const char *word);
extern bool IsEnglish (const string& word);
extern bool IsGerman (const char *word);
extern bool IsGerman (const string& word);
extern bool CheckLanguage (const char *word, MorphLanguageEnum langua);
extern bool CheckLanguage (const string& word, MorphLanguageEnum langua);



extern void ConvertJO2Je(string& src);
extern void ConvertJO2Je(char* src);




template <class T, class Pred, class Conv>
T& RegisterConverter (T& word, size_t Len, Pred predicate,  Conv converter)
{
	for( size_t i = 0 ; i < Len; i++ )
        if ( predicate( (BYTE)word[i] )  )
			word[i] = converter ( (BYTE)word[i] );

    return word;
}


template <class T>
T& GerEngRusMakeUpperTemplate (T& word, MorphLanguageEnum Langua, size_t Len )
{
	if (Len == 0) return word;

	if (Langua == morphGerman)
		return RegisterConverter(word, Len, is_german_lower, gtoupper);
	else
		for( size_t i = 0 ; i < Len; i++ )
			if (is_russian_lower((BYTE)word[i]))
					word[i] = rtoupper ( (BYTE)word[i] );
			else
			if (is_english_lower((BYTE)word[i]))
					word[i] = etoupper ( (BYTE)word[i] );

	return word;
};	



const char *getRMLDirectory();


//  QWORD mask
#define _QM(X) (((QWORD)1)<<X)

enum RegisterEnum {AnyRegister=0, LowLow=1, UpLow=2, UpUp=3};


#endif // utilit_h


