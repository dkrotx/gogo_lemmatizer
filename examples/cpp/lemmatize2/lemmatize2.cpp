/**
 * Upgraded version of <lemmatize.cpp>.
 *
 * Example of using [C++] LemInterface to Lemmatizer
 * Usage is straightforward: ./lemmatize [options] word ...
 *
 * July 15, 2015: Updated to process words from a file, or standard input.
 *                                                          [--Pav]
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE /* for getopt_long(3) */
#endif

#include <string.h>
#include <sysexits.h>
#include <getopt.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>

#include <LemInterface.hpp> 

namespace {
    int process_arguments(int argc, char **argv);

    void usage(const char *progname);

    void process_option_input_file(bool is_utf8, bool output_json,
            int argc, char **argv);

    void process_word(LemInterface &lemmatizer, bool output_json,
            const char *word);

    void process_file(bool is_utf8, bool output_json, std::istream &input_file);

    void process_files(bool is_utf8, bool output_json, int argc, char** argv,
            int optind);

    std::string escapeJsonString(const std::string& value);
}

int
main(int argc, char *argv[])
{
    // CygWin sometimes dumps core when an exception is thrown, rather than
    // reporting it, so we do it ourselves:
    try
    {
        process_arguments(argc, argv);
    }
    catch (std::exception &exc)
    {
        std::cerr << "Exception: " << exc.what() << std::endl;
    }
}



namespace
{

int process_arguments(int argc, char *argv[])
{
    bool is_utf8 = true;
    bool output_json = false;

    while(1) {
        static struct option long_options[] = {
            {"utf8",        no_argument,       NULL, 'u'},
            {"cp1251",      no_argument,       NULL, 'c'},
            {"output-json", no_argument,       NULL, 'j'},
            {"input-file",  required_argument, NULL, 'i'},
            {"help",        no_argument,       NULL, 'h'},
        };

        int c = getopt_long(argc, argv, "ucjih", long_options, NULL);

        if (c == -1) {
            argc -= optind;
            argv += optind;
            break;
        }

        switch(c) {
            case 'u': is_utf8 = true;  break;
            case 'c': is_utf8 = false; break;
            case 'j': output_json = true; break;
            case 'i':
                process_option_input_file(is_utf8, output_json, argc, argv);
                break;
            default : usage(argv[0]); /* UNDESTINABLE */
        }
    }


    LemInterface lem(is_utf8);

    while(argc--) {
        const char *word = *argv++;
        process_word(lem, output_json, word);
    }

    return 0;
}

void usage(const char *progname)
{
    std::cout << "Usage: " << progname << " [-u] [-c] word ...\n"; 
    std::cout << "Options are:\n"; 
    std::cout << "\t-u|--utf:    treat input and output as UTF-8\n" 
                 "\t-c|--cp1251: treat input and output as CP1251 (Windows encoding)\n"
                 "\t-j|--output-json: output results in JSON format\n"
                 "\t-i|--input-file=<path>: read words from <path> rather than the command-line.\n"
                 "\t\tIf <path> is '-', read from standard input.\n"
                 "\t\tIf <path> is '--', process all remaining command-line arguments as input file paths.\n";
    std::cout << "\n";

    exit(EX_USAGE);
}

void process_option_input_file
(bool is_utf8, bool output_json, int argc, char **argv)
{
    char *input_path;

    if (NULL == optarg)
    {
        if (optind < argc)
        {
            input_path = argv[optind];
            ++optind;
        }
        else
        {
            // Usage exits:
            usage(argv[0]);
            return;
        }
    }
    else
    {
        input_path = optarg;
    }
    if (strcmp("--", input_path) == 0)
    {
        process_files(is_utf8, output_json, argc, argv, optind);
    }
    else
    {
        if (strcmp("-", input_path) == 0)
        {
            process_file(is_utf8, output_json, std::cin);
        }
        else
        {
            std::filebuf file_buffer;

            file_buffer.open(input_path, std::ios::in);

            std::istream input_file(&file_buffer);

            process_file(is_utf8, output_json, input_file);
        }
    }
}

void process_word(LemInterface &lemmatizer, bool output_json, const char *word)
{
    LemInterface::result_t res;

    if (lemmatizer.Lemmatize(word, &res, LemInterface::FILL_PART_OF_SPEECH))
    {
        if (output_json)
        {
            std::cout << "{word:\"" << word
                      << "\",first_form:\"" << res.first_form
                      << "\",part_of_speech:\"" << res.part_of_speech
                      << "\"}\n";
        }
        else
        {
            std::cout << word 
                      << " => { first_form = " << res.first_form 
                      << "; part_of_speech = '" << res.part_of_speech 
                      << "' }\n";
        }
    }
    else
    {
        if (output_json)
        {
            std::cout << "{word:\"" << word
                      << "\",status:\"error\",message:\"Failed to parse word.\"}";
        }
        else
        {
            std::cerr << "failed to parse '" << word << "'\n";
        }
    }
}


void process_file(bool is_utf8, bool output_json, std::istream &input_file)
{
    LemInterface lemmatizer(is_utf8);
    LemInterface::result_t res;
    std::string word;

    while (input_file >> word)
    {
        process_word(lemmatizer, output_json, word.c_str());
    }
}

void process_files
(bool is_utf8, bool output_json, int argc, char** argv, int optind)
{
    for (int i = optind; i < argc; ++i)
    {
        std::filebuf file_buffer;

        file_buffer.open(argv[i], std::ios::in);

        std::istream file(&file_buffer);

        process_file(is_utf8, output_json, file);
    }
}

// From <stackoverflow.com/questions/7724448/simple-json-string-escape-for-c>:
std::string escapeJsonString(const std::string& value)
{
    std::ostringstream result;

    for (std::string::const_iterator iterator = value.begin();
    iterator != value.end(); ++iterator)
    {
        switch (*iterator)
        {
            case '\\': result << "\\\\"; break;
            case '"':  result << "\\\""; break;
            case '/':  result << "\\/"; break;
            case '\b': result << "\\b"; break;
            case '\f': result << "\\f"; break;
            case '\n': result << "\\n"; break;
            case '\r': result << "\\r"; break;
            case '\t': result << "\\t"; break;
            default: result << (*iterator); break;
        }
    }

    return result.str();
}

} /* anon namespace */
