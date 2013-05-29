#include "SimpleLemmatiser.hpp"

int main(int argc, char **argv)
{
	SimpleLemmatiser SL(0);
	if (SL.LoadDictionary())
	{
		printf("%s\n", SL.Lemmatise((char *)"�������"));
		printf("%s\n", SL.LemmaAndCodes((char *)"�������"));
		char ** pszFirstForms = SL.FirstForm((char *)"�������");
		int i = 0;
		while (pszFirstForms[i] != NULL)
		{
			char * szForm = pszFirstForms[i];
			printf("%s\n", szForm);
			i++;
		}
	}
	else { printf("Error Loading Dictionary"); }
	return 0;
}
