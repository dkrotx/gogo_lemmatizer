#include <vector>
#include <stdexcept>

#include "../../common/utilit.h"
#include "CMorphology.hpp"
#include "LemInterface.hpp"
#include "utf_cp.hpp"


LemInterface::LemInterface(bool utf8) : utf8_(utf8) {
    morph_.reset(new CMorphology());

    if (!morph_->RussianDict.LoadDictionariesRegistry() || !morph_->Agramtab.LoadFromRegistry()) {
        throw std::runtime_error("Failed to load russian dictionary");
    }

    if (!morph_->EnglishDict.LoadDictionariesRegistry() || !morph_->Egramtab.LoadFromRegistry()) {
        throw std::runtime_error("Failed to load english dictionary");
    }
    
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

    MorphLanguageEnum lang = IsRussian(word_cp1251) ? morphRussian : morphEnglish;
    std::vector<std::string> fforms = morph_->GetFirstForm(lang, word_cp1251);
    if (fforms.empty()) {
        return false;
    }

    if (utf8_) {
        gogo::conv_str(gogo::CONV_CP1251, gogo::CONV_UTF8, fforms[0], &res->first_form);
    } else {
        res->first_form = fforms[0];
    }

    if (flags & FILL_PART_OF_SPEECH) {
        std::vector<CFormInfo> paradigms;
       
        // TODO: join it with first form
        const CLemmatizer *plem = morph_->GetLemmatizer(lang);
        plem->CreateParadigmCollection(false, word_cp1251, false /* isUppercased */, paradigms);

        if (paradigms.empty()) {
            res->part_of_speech.clear();
        }
        else {
            std::string GramCodes = paradigms[0].GetSrcAncode();
            const CAgramtab* gtab = morph_->GetGramTab(lang);
            short c = gtab->GetPartOfSpeech(GramCodes.c_str());

            if (lang == morphRussian && utf8_) {
                std::string tmp = gtab->GetPartOfSpeechStr(c);
                gogo::conv_str(gogo::CONV_CP1251, gogo::CONV_UTF8, tmp, &res->part_of_speech);
            } else {
                res->part_of_speech = gtab->GetPartOfSpeechStr(c);
            }
        }
    }

    return true;
}
