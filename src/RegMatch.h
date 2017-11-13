#ifndef REGMATCH_H
#define REGMATCH_H
#include <stdio.h>
#include<regex.h>

class RegMatch{
public:
    RegMatch(char *pattern);
    ~RegMatch();

    int RegMatch_compile(char *pattern);
    int RegMatch_free();
    int RegMatch_search(char *data, char *pattern=NULL);
    int RegMatch_match(char *data, char *pattern=NULL);
    int RegMatch_findall(char *data, char *pattern=NULL);
    int RegMatch_next_data(int index, char *buf, char *data=NULL);

    regmatch_t pmatch[100];
private:
    char *pattern;
    regex_t reg;
    char *data;
};

#endif
