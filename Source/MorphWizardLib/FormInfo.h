#ifndef _FORM_INFO_H
#define _FORM_INFO_H


struct CMorphForm 
{
	string m_Gramcode;
	string m_FlexiaStr;
	string m_PrefixStr;

	CMorphForm (string Gramcode, string FlexiaStr, string PrefixStr)
	{
		m_Gramcode = Gramcode;
		m_FlexiaStr = FlexiaStr;
		m_PrefixStr = PrefixStr;
		assert (!m_Gramcode.empty());
	};
	bool		operator == (const CMorphForm& comparand) const 
	{
		return		m_Gramcode == comparand.m_Gramcode
				&&	m_FlexiaStr == comparand.m_FlexiaStr
				&&	m_PrefixStr == comparand.m_PrefixStr;
	};

};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
struct  CFlexiaModel
{
	vector<CMorphForm>	m_Flexia;

	bool		operator == (const CFlexiaModel& comparand) const 
	{
		return	m_Flexia == comparand.m_Flexia;
	};
	bool		ReadFromString(const string& s);
	string		ToString() const;

	string		get_first_flex() const;
	string		get_first_code() const;
	bool		has_ancode(const string& search_ancode) const;
	
};



//----------------------------------------------------------------------------
struct CAccentModel 
{
	vector<BYTE> m_Accents;

	bool operator == (const CAccentModel& comparand) const
	{
		return m_Accents == comparand.m_Accents;

	};
	bool		ReadFromString(const string& s);
	string		ToString() const;
};


//----------------------------------------------------------------------------

const size_t CommonAncodeSize = 2;
const WORD UnknownParadigmNo = 0xffff-1;
const WORD UnknownAccentModelNo = 0xffff-1;

struct CLemmaInfo
{
	WORD	m_FlexiaModelNo;
	WORD	m_AccentModelNo;
	char	m_CommonAncode[CommonAncodeSize];

	CLemmaInfo()
	{
		m_FlexiaModelNo = UnknownParadigmNo;
		m_AccentModelNo = UnknownAccentModelNo;
		m_CommonAncode[0] = 0; 
	};

	bool operator ==(const CLemmaInfo& obj) const
	{	
		return (	(m_FlexiaModelNo == obj.m_FlexiaModelNo)
				&&	(m_AccentModelNo == obj.m_AccentModelNo)
				&&	!strncmp(m_CommonAncode,obj.m_CommonAncode,CommonAncodeSize)
				);	
	}

	bool operator <(const CLemmaInfo& obj) const
	{	
		if (m_FlexiaModelNo != obj.m_FlexiaModelNo)
			return m_FlexiaModelNo < obj.m_FlexiaModelNo;

		int res = strncmp(m_CommonAncode,obj.m_CommonAncode,CommonAncodeSize);
		if (res != 0)
			return res < 0;

		return m_AccentModelNo < obj.m_AccentModelNo;
	}

	string GetCommonAncode()  const 
	{
		if (m_CommonAncode[0] == 0) return "";
		return string(m_CommonAncode,2);	
	}

};

inline size_t get_size_in_bytes (const CLemmaInfo& t)
{
	return		
				get_size_in_bytes(t.m_FlexiaModelNo)
			+   get_size_in_bytes(t.m_AccentModelNo)
			+   2; //== CommonAncodeSize
			;
};



inline size_t save_to_bytes(const CLemmaInfo& i, BYTE* buf)
{
	buf += save_to_bytes(i.m_FlexiaModelNo, buf);
	buf += save_to_bytes(i.m_AccentModelNo, buf);
	buf += save_to_bytes((BYTE)i.m_CommonAncode[0], buf);
	buf += save_to_bytes((BYTE)i.m_CommonAncode[1], buf);
	return get_size_in_bytes(i);
}


inline size_t restore_from_bytes(CLemmaInfo& i, const BYTE* buf)
{
	buf += restore_from_bytes(i.m_FlexiaModelNo, buf);
	buf += restore_from_bytes(i.m_AccentModelNo, buf);
	buf += restore_from_bytes(i.m_CommonAncode[0], buf);
	buf += restore_from_bytes(i.m_CommonAncode[1], buf);
	return get_size_in_bytes(i);
}


extern void ReadFlexiaModels(FILE* fp, vector<CFlexiaModel>& FlexiaModels );
extern void WriteFlexiaModels(FILE* out_fp, const vector<CFlexiaModel>& FlexiaModels );
extern void ReadAccentModels (FILE* fp, vector<CAccentModel>&	AccentModels );
extern void WriteAccentModels(FILE* out_fp, const vector<CAccentModel>& AccentModels );

#endif
