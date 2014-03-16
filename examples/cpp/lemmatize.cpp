/**
 * Example of using [C++] LemInterface to Lemmatizer
 * Usage is straightforward: ./lemmatize [options] word ...
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE /* for getopt_long(3) */
#endif

#include <sysexits.h>
#include <getopt.h>

#include <iostream>
#include <cstdlib>
#include <LemInterface.hpp> 

namespace {
    void usage(const char *progname);
}

int
main(int argc, char *argv[])
{
    bool is_utf8 = true;

    while(1) {
        static struct option long_options[] = {
            {"utf8",   no_argument, 0, 'u' },
            {"cp1251", no_argument, 0, 'c' },
            {"help",   no_argument, 0, 'h' },
        };

        int c = getopt_long(argc, argv, "uch", long_options, NULL);
        if (c == -1) {
            argc -= optind;
            argv += optind;
            break;
        }

        switch(c) {
            case 'u': is_utf8 = true;  break;
            case 'c': is_utf8 = false; break;
            default : usage(argv[0]); /* UNDESTINABLE */
        }
    }


    LemInterface lem(is_utf8);

    while(argc--) {
        const char *word = *argv++;
        LemInterface::result_t res;

        if (lem.Lemmatize(word, &res, LemInterface::FILL_PART_OF_SPEECH)) {
            std::cout << word 
                      << " => { first_form = " << res.first_form 
                      << "; part_of_speech = '" << res.part_of_speech 
                      << "' }\n";
        }
        else {
            std::cerr << "failed to parse '" << word << "'\n";
        }
    }

    return 0;
}


namespace
{

void usage(const char *progname) {
    std::cout << "Usage: " << progname << " [-u] [-c] word ...\n"; 
    std::cout << "Options are:\n"; 
    std::cout << "\t-u|--utf:    treat input and output as UTF-8\n" 
                 "\t-c|--cp1251: treat input and output as CP1251 (Windows encoding)\n";
    std::cout << "\n";

    exit(EX_USAGE);
}

} /* anon namespace */
