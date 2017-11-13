#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "RegMatch.h"

#if 1
#define REG_DEBUG(...)
#else
    //printf("[%s:%s] ---> ", __FILE__,  __FUNCTION__);    
#define REG_DEBUG(...) do{   \
    printf("[%s] ---> ",  __FUNCTION__);    \
    printf(__VA_ARGS__);          \
}while(0)
#endif

RegMatch::RegMatch(char *pattern)
{
    char errbuf[1024];
    int err;

    if(regcomp(&reg, pattern, REG_EXTENDED) < 0){
        regerror(err,&reg,errbuf,sizeof(errbuf));
        REG_DEBUG("regcomp err\n");
    }

   //RegMatch_compile(pattern);
}

RegMatch::~RegMatch()
{
   RegMatch_free(); 
}

int RegMatch::RegMatch_compile(char *pattern)
{
#if 0
    int cflags = REG_EXTENDED;
    int len;

    if(pattern == NULL)return -1;
    len = strlen(pattern);
    REG_DEBUG("malloc: %d\n", len);
    this->pattern = (char *)malloc(len);
    if(this->pattern == NULL){
        REG_DEBUG("malloc fail\n");
        return -1;
    }else{
        strcpy(this->pattern, pattern);
    }

    if(regcomp(&reg, pattern1, cflags) < 0){
        REG_DEBUG("regcomp err\n");
        return -1;
    }
    return 0;
#endif
}

int RegMatch::RegMatch_free()
{
    regfree(&reg);
#if 0
    if(this->pattern){
        free(this->pattern);
    }
#endif
}

int RegMatch::RegMatch_match(char *data, char *pattern)
{
#if 0
    if(pattern != NULL){
        if(RegMatch_compile(pattern) < 0){
            return -1;
        }
    }

#endif
    if(data){
        this->data = data;
    }else{
        return -1;
    }

    int err = regexec(&reg, this->data, sizeof(pmatch)/sizeof(regmatch_t), pmatch, REG_NOTBOL);
    if(err == REG_NOMATCH){
        REG_DEBUG("no match\n");
        return -1;
    }else if(err){
        REG_DEBUG("err %d\n", err);
        return -1;
    }
    return 0;
}

int RegMatch::RegMatch_search(char *data, char *pattern)
{

}

int RegMatch::RegMatch_findall(char *data, char *pattern)
{

}

int RegMatch::RegMatch_next_data(int index, char *buf, char *data)
{
#if 1
    int len;
    if(data == NULL){
        data = this->data;
    }

    len = pmatch[index].rm_eo - pmatch[index].rm_so;
    REG_DEBUG("index:%d len:%d\n", index, len);
    if(len > 0){
        strncpy(buf, data+pmatch[index].rm_so, len);
        return len;
    }else{
        return 0;
    }
#endif
    return 0;
}

