#ifndef CAMLER_UTILS_H
#define CAMLER_UTILS_H
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

/* Define size
*/
#define SIZE_TINY 8
#define SIZE_SMALL 32
#define SIZE_MEDIUM 128
#define SIZE_LARGE 1024

/* Define variables for server default infomation
*/
#define DEFAULT_PORT 30497
#define DEFAULT_STORAGE "storage"
#define DEFAULT_HCNET_PORT 8000

/* Define string value for  char* 
*/
#define NOTSET	"@#notset#@"	// using for a empty string
#define UNAUTHORIZED "{\"status\":\"unauthorized\"}"
#define FAILE "{\"status\":\"faile\"}"

/* API token key
*/
#define API_TOKEN "GXwmHg7kuYw0UQv5wRwzsUBKmtXe9TbrvuGYnoLKzDjCqdApPwI4APEI77HiWqklHpRtofrakiECY4541XBgagY3zJ96SMM1slGF10uc3v5a374yjN1v1ycQz6iULwV7"

typedef struct json_object
{
	/* Define a JSON Object
	*/
	char* _key;
	char* _value;
}json_obj;

typedef struct json_objects
{
	/*
	*	Define a array of json_object
	*/
	int _size;
	json_obj* _element;
}json_objs;

char* timestamp(); /* Get current time stamp */

/* Count number of sub-string in a string */
int count_str_str(const char* str, const char* sub_str);

/* Compare two string, if they are exactly same, return 1, otherwise return 0*/
int compare_str(const char* str1, const char* str2);

/* Count number of character in a string */
int count_cha_str(const char* str, char cha);

/* Split a string to n of sub-string at a character
*  Example: string_split("hello hello world", " ", 2)
*		return ["hello", "hello", "world"]
*/
char** string_split(const char* str, char character, int times);

/* Check a file exists or not */
int file_exists(const char* file_path);

/* Read and return content of a file */
char** read_from_file(const char* file_path);

/* Replace all sub-string in a string with new sub-string */
char* replace_str_str(const char* string, const char* sub_str, const char* replacement);

/* Create a json object structure from key and value */
json_obj create_json(const char* key, const char* value);

/* Compose list of json object to string format */
char* compose_json(json_objs objs);

/* Return json structure from list of json with key = key */
json_obj get_json(json_objs objs, const char* key);

char* timestamp()
{
    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    char* ts = (char*)malloc(64);
    sprintf(ts, "%s", asctime( localtime(&ltime) ));
    ts[strlen(ts)-1] = '\0';
    return ts;
}

json_obj create_json(const char* key, const char* value)
{
	/* Replace special character in x-www-form-urlencoded format
	*/
	char* _key = replace_str_str(key, "%20", " ");
	_key = replace_str_str(_key, "%21", "!");
	_key = replace_str_str(_key, "%22", "\"");
	_key = replace_str_str(_key, "%23", "#");
	_key = replace_str_str(_key, "%24", "$");
	_key = replace_str_str(_key, "%25", "%");
	_key = replace_str_str(_key, "%26", "&");
	_key = replace_str_str(_key, "%27", "'");
	_key = replace_str_str(_key, "%28", "(");
	_key = replace_str_str(_key, "%29", ")");
	_key = replace_str_str(_key, "%2A", "*");
	_key = replace_str_str(_key, "%2B", "+");
	_key = replace_str_str(_key, "%2C", ",");
	_key = replace_str_str(_key, "%2D", "-");
	_key = replace_str_str(_key, "%2E", ".");
	_key = replace_str_str(_key, "%2F", "/");
	_key = replace_str_str(_key, "%40", "@");
	_key = replace_str_str(_key, "%5F", "_");

	char* _value = replace_str_str(value, "%20", " ");
	_value = replace_str_str(_value, "%21", "!");
	_value = replace_str_str(_value, "%22", "\"");
	_value = replace_str_str(_value, "%23", "#");
	_value = replace_str_str(_value, "%24", "$");
	_value = replace_str_str(_value, "%25", "%");
	_value = replace_str_str(_value, "%26", "&");
	_value = replace_str_str(_value, "%27", "'");
	_value = replace_str_str(_value, "%28", "(");
	_value = replace_str_str(_value, "%29", ")");
	_value = replace_str_str(_value, "%2A", "*");
	_value = replace_str_str(_value, "%2B", "+");
	_value = replace_str_str(_value, "%2C", ",");
	_value = replace_str_str(_value, "%2D", "-");
	_value = replace_str_str(_value, "%2E", ".");
	_value = replace_str_str(_value, "%2F", "/");
	_value = replace_str_str(_value, "%40", "@");
	_value = replace_str_str(_value, "%5F", "_");
	json_obj obj;
	obj._key = (char*)malloc(SIZE_MEDIUM);
	obj._value = (char*)malloc(SIZE_LARGE);
	sprintf(obj._key, "%s", _key);
	sprintf(obj._value, "%s", _value);
	return obj;
}

char* compose_json(json_objs objs)
{
	/* Compose a list of json_obj to JSON string valid
	*/
	char* content = (char*)malloc(SIZE_LARGE);
	sprintf(content, "{");
	int i = 0;	/* for loop */
	while(i<objs._size)
	{
		char* pair = (char*)malloc(SIZE_MEDIUM);
		if (i > 0)
			sprintf(pair, ",\"%s\":\"%s\"", objs._element[i]._key, objs._element[i]._value);
		else
			sprintf(pair, "\"%s\":\"%s\"", objs._element[i]._key, objs._element[i]._value);
		strcat(content, pair);
		free(pair);
		i++;
	}
	sprintf(content, "%s}", content);
	return content;
}

json_obj get_json(json_objs objs, const char* key)
{
	json_obj obj;
	for(int i=0;i<objs._size;i++)
	{
		if (compare_str(objs._element[i]._key, key)==1)
		{
			obj = create_json(key, objs._element[i]._value);
			return obj;
		}
	}
	obj = create_json("", "");
	return obj;
}

int count_str_str(const char* str, const char* sub_str)
{
	/* Count how many time a sub_str appear in str
	*/
	int count = 0;
	for(int i=0;str[i]!='\0';i++)
	{
		if (str[i] == sub_str[0])
		{
			int j = 0;
			for(j=0;sub_str[j]!='\0';j++)
			{
				if (str[i+j] != sub_str[j])
					break;
			}
			if (sub_str[j] == '\0')
				count++;
		}
	}
	return count;
}

int compare_str(const char* str1, const char* str2)
{
	int i = 0;
	while(1)
	{
		if (str1[i] == '\0' && str2[i] == '\0') return 1;
		else if ((str1[i] == '\0' && str2[i] != '\0')
			|| (str1[i] != '\0' && str2[i] == '\0'))
			return 0;
		else if (str1[i] != str2[i]) return 0;
		i++;
	}
	return 1;
}

int count_cha_str(const char* str, char cha)
{
	/*
	*	Count how many character cha in string str
	*	Return 0 if not found
	*/
	int count = 0;
	int i = 0; /* for loop */
	while(str[i] != '\0')
	{
		if (str[i] == cha)
			count++;
		i++;
	}
	return count;
}

char** string_split(const char* str, char character, int times)
{
	/*  Split a string (char*) into chunk of string (char**), break at character
	*	Example: 
	*		string_split("Hello Hello Hello Hello", ' ', 3)
	*		return
	*			"Hello"
	*			"Hello"
	*			"Hello"
	*			"Hello"
	*/
	char** str_return;
	int i = 0;	/* for loop */
	str_return = (char**)malloc((times+1)*sizeof(char*));
	for(i=0;i<=times;i++)
		str_return[i] = (char*)malloc(SIZE_MEDIUM);
	int str_return_index = 0;
	char* buffer;
	int str_index = 0;
	while(times >= 0)
	{
		buffer = (char*)malloc(SIZE_MEDIUM);
		int buf_index = 0;
		while(
			((times>0) && (str[str_index] != character) && (str[str_index] != '\0'))
			|| ((times==0) && (str[str_index] != '\0')))
		{
			buffer[buf_index] = str[str_index];
			buf_index++;
			str_index++;
		}
		buffer[buf_index] = '\0';
		strcpy(str_return[str_return_index], buffer);
		str_index++;
		str_return_index++;
		times--;
		free(buffer);
	}

	return str_return;
}

int file_exists(const char* file_path)
{
	/*
	*	Check if a file exists or not
	*	file_path is a path to file. For example: /home/pmtoan/text.txt
	*	if (file exists) return 1, else return 0
	*/
	if(!access(file_path, F_OK ))
		return 1;
	else
		return 0;
}

char** read_from_file(const char* file_path)
{
	/* Open a file and read, return char** content of file
	*/
	if (file_exists(file_path) != 1)
		printf("[ERROR]    %s not found\n", file_path);
	char** content;
	content = (char**)malloc(SIZE_LARGE);	/* Max row of file is 4096 lines */
	FILE* f_file = fopen(file_path, "r");
	if (f_file == NULL)
		printf("[ERROR]   Error while reading %s\n", file_path);
	else
	{
		int i = 0;
		char* buffer = (char*)malloc(SIZE_LARGE);
		while(!feof(f_file))
		{
			fgets(buffer, SIZE_LARGE, f_file);
			content[i] = (char*)malloc(SIZE_LARGE);
			strcpy(content[i], buffer);
			i++;
		}
		free(buffer);
	}
	return content;
}

char* replace_str_str(const char* string, const char* sub_str, const char* replacement)
{
	/*
	*	Replace a sub-string in a string
	*	Example:
	*		replace_str_str("hello world", "hello", "hi") -> hi world
	*/
	char* tok = NULL;
	char* newstr = NULL;
	char* oldstr = NULL;
	int   oldstr_len = 0;
	int   sub_str_len = 0;
	int   replacement_len = 0;
	newstr = strdup(string);
	sub_str_len = strlen(sub_str);
	replacement_len = strlen(replacement);
	if (sub_str == NULL || replacement == NULL) {
		return newstr;
	}
	while ((tok = strstr(newstr, sub_str))) {
		oldstr = newstr;
		oldstr_len = strlen(oldstr);
		newstr = (char*)malloc(sizeof(char) * (oldstr_len - sub_str_len + replacement_len + 1));
		if (newstr == NULL) {
			free(oldstr);
			return NULL;
		}
		memcpy(newstr, oldstr, tok - oldstr);
		memcpy(newstr + (tok - oldstr), replacement, replacement_len);
		memcpy(newstr + (tok - oldstr) + replacement_len, tok + sub_str_len, oldstr_len - sub_str_len - (tok - oldstr));
		memset(newstr + oldstr_len - sub_str_len + replacement_len, 0, 1);
		free(oldstr);
	}
	return newstr;
}

#endif