#include <vector>
#include <stdexcept>

#include "CMorphology.hpp"
#include "LemInterface.hpp"
#include "utf_cp.hpp"

LemInterface::LemInterface(bool utf8) : utf8_(utf8){
    morph_.reset(new CMorphology());

    if (!morph_->Agramtab.LoadFromRegistry() || !morph_->RussianDict.LoadDictionariesRegistry()) {
        throw std::runtime_error("Failed to load russian dictionary");
    }
    
    if ( !(plem_rus_ =  morph_->GetLemmatizer(morphRussian)) ) {
        throw std::runtime_error("Failed to get lemmatizer");
    }
}


short LemInterface::partOfSpeech(const std::string &word1251) {
    std::vector<CFormInfo> paradigms;
    std::string w = word1251;

    plem_rus_->CreateParadigmCollection(false, w, false /* isUppercased */, paradigms);
    if (paradigms.empty())
        return -1;

    std::string GramCodes = paradigms[0].GetSrcAncode();
    return morph_->GetGramTab(morphRussian)->GetPartOfSpeech(GramCodes.c_str());
}

/**
 * Input and output is always UTF-8
 * but lemmatizer itself works only with CP1251.
 * So, we have to convert given `word'
 */
bool LemInterface::Lemmatize(const std::string &word, result_t *res, int flags /* = 0 */)
{
    std::string word_cp1251;

    if (utf8_) {
        if (!gogo::conv_str(gogo::CONV_UTF8, gogo::CONV_CP1251, word, &word_cp1251)) {
            // lemmatizer is limited tp CP1251, so
            // there is no reason to continue
            return false;
        }
    } else {
        word_cp1251 = word;
    }

    std::vector<std::string> fforms = morph_->GetFirstForm(morphRussian, word_cp1251);
    if (fforms.empty()) {
        return false;
    }

    if (utf8_) {
        gogo::conv_str(gogo::CONV_CP1251, gogo::CONV_UTF8, fforms[0], &res->first_form);
    } else {
        res->first_form = fforms[0];
    }

    if (flags & FILL_PART_OF_SPEECH) {
        short c = partOfSpeech(word_cp1251);

        if (utf8_) {
            std::string tmp = morph_->Agramtab.GetPartOfSpeechStr(c);
            gogo::conv_str(gogo::CONV_CP1251, gogo::CONV_UTF8, tmp, &res->part_of_speech);
        } else {
            res->part_of_speech = morph_->Agramtab.GetPartOfSpeechStr(c);
        }
    }

    return true;
}
