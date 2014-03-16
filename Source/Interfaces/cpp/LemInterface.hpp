#ifndef GOGO_LEMINTERFACE_HPP__
#define GOGO_LEMINTERFACE_HPP__

#include <string>
#include <memory>

class CMorphology;
class CLemmatizer;

class LemInterface
{
public:
    struct result_t {
        std::string first_form;
        std::string part_of_speech;
    };

    enum { FILL_PART_OF_SPEECH = 1 };

    LemInterface(bool utf8);

public:
    bool Lemmatize(const std::string &word, result_t *res, int flags = 0);

private:
    short partOfSpeech(const std::string &word1251);

private:
    std::auto_ptr<CMorphology> morph_;
    const CLemmatizer *plem_rus_;
    bool utf8_;    
};

#endif // GOGO_LEMINTERFACE_HPP__
