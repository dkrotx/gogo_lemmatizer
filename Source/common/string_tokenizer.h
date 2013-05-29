#ifndef INCL_STRING_TOKENIZER_H
#define INCL_STRING_TOKENIZER_H

#include "utilit.h"
/**
	define TOKENIZER_USES_VECTOR_BOOL 
	if you want to use vector<bool> in tokenizer.
	It must work faster with it, but it does not.
*/



/**
	The Reenterable replacement of strtok().


	Usage:		

	char *text = "word, word2;word3"
	StringTokenizer token(text, " ,;");
	const char *word;
	while((word = token())){
		// handle token here
	}


*/
class StringTokenizer{
	char *text_ptr;
	char *text;
	int i;
	//-------------------------------------------------------
	string delims;
	bool is_delim(char ch){
		return delims.find_first_of(ch) != string::npos;
	}
	char *_val;
public:
	//-------------------------------------------------------
	StringTokenizer(const char *_text, const char *_delims)
		:delims(_delims)
		,i(-1)
	{
		size_t leng = strlen(_text);
		text_ptr = new char[leng+2];
		text = text_ptr;
		strcpy(text, _text);
		text[leng+1] = 0;
	}

	//-------------------------------------------------------
	~StringTokenizer(){
		delete []text_ptr;
	}

	//-------------------------------------------------------
	const char * operator ()(){
		++i;
		// skip delims
		while(*text && is_delim(*text))
			text++;

		if(*text == 0)	return 0;

		char *ret_val = text;

		while(*text && !is_delim(*text))
			text++;
		*text++ = 0;
		_val = ret_val;
		return ret_val;
	}
	string next_token ()
	{
		const char* s = operator()();
		if (s) 
			return s;
		else 
			return string();
	}

	const char * get_rest ()
	{
		return text;
	};

	char *val(){return _val;}
	int  count(){return i;}
	bool has_next(){return (*text) != 0;}

};


#endif // INCL_STRING_TOKENIZER_H
