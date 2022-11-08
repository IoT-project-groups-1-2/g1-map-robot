//
// Created by tylen on 8.11.2022.
//
#include "json_str.h"

static bool compare_str(const char *str1, const char *str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) return false;
        str1++;
        str2++;
    }
    return (*str2 == 0);
}

static const char* strstr(const char* haystack, const char* needle)
{
    while (*haystack != '\0')
    {
        if ((*haystack == *needle) && compare_str(haystack, needle)) {
            return haystack;
        }
        haystack++;
    }

    return NULL;
}

static unsigned int strlen(const char * str){
    unsigned int cc = 0;
    for (; *str != 0; str++){
        cc++;
    }
    return cc;
}

static void memorycpy(void * from,void * to, unsigned int n){
    if(!from) return;
    char * source = (char*)from;
    char * dest = (char*)to;
    while(n){
        (*dest++)=(*source++);
        n--;
    }
}

static void memoryset(void * data, char c, int n){
    if(!data) return;
    char* c_data = (char*)data;
    for(int i = 0; i < n; i++){
        c_data[i] = c;
    }
}

static bool isspace(char s){
    switch (s) {
        case ' ':
            break;
        case '\n':
            break;
        case '\r':
            break;
        case '\t':
            break;
        case '\v':
            break;
        case '\f':
            break;
        default:
            return false;
    }
    return true;
}

/*
* Clears received json string from whitespaces and newlines. Doesn't affect variable scopes.
* ( '{ "make" : "My Car" }' -> '{"make":"My Car"}' )
*/
static void clean_json_string(char **string) {
    if (*string == JSON_STR_NULL) return; //Received null pointer.
    bool var_scope = false; //Flag to check whether whitespace is located inside the variable scope. (We don't want to ruin the names)
    int i = 0;
    int w = strlen(*string);
    char prev_char = ' '; //Not the best way to check '\', but there shouldn't be '\' in the end of the make or model of the car. (it should be enough)

    for (i = 0; i < w; i++) {
        //Clear all whitespaces outside the variable scopes.
        if (isspace(string[0][i]) && !var_scope) {
            for (int q = i + 1; q <= w; q++) {
                string[0][q - 1] = string[0][q];
            }
            w--;
            i--;
        }
            //It is '"' -> flip variable scope.
        else if (prev_char != '\\' && string[0][i] == '\"') {
            if (var_scope) {
                var_scope = false;
            }
            else {
                var_scope = true;
            }
        }
        else {
            prev_char = string[0][i];
        }
    }
}

bool json_str_int_from_context(char * json_str, const char * context, int * dest)
{
    char *value = json_str;
    clean_json_string(&value);
    char *temp = strstr(value, context);
    if(!temp){
        return false;
    }
    int sign = 1;
    while (*temp++ != ':');
    value = temp;
    int byte_counter = 0;
    while (*temp != ',' && *temp != '}') {
        if(*temp == '-'){
            sign = -1;
            *value++;
            byte_counter -= 1;
        }
        temp++;
        byte_counter++;
    }
    int extracted_value = 0;
    for (int i = 0; i < byte_counter; ++i) {
        extracted_value = extracted_value * 10 + *value - '0';
        value++;
    }
    *dest = extracted_value * sign;
    return true;
}

bool json_str_str_from_context(char * json_str, const char * context, char* dest)
{
    char *value = json_str;
    clean_json_string(&value);
    char *temp = strstr(value, context);
    if(!temp){
        return false;
    }
    while (*temp != ':') temp++;
    temp += 2;
    value = temp;

    int byte_counter = 0;
    while (*temp != '"') {
        temp++;
        byte_counter++;
    }
    if(dest) memoryset(dest, 0, strlen(dest));
    memorycpy(value, dest, byte_counter);
    return true;
}