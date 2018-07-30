#ifndef UTILS_H
#define UTILS_H
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "type.h"

typedef struct JSONObject
{
	/*
	* Define a JSON Object
	*/
	char* _key;
	char* _value;
}JSONObj;

typedef struct JSONObjects
{
	/*
	*	Define a array of JSONObject
	*/
	int _size;
	JSONObj* _element;
}JSONObjs;

int countStrStr(const char* str, const char* subStr);
int compareStrStr(const char* str1, const char* str2);
int strcha(const char* str, char cha);
char** stringSplit(const char* str, char character, int times);
int fileExists(const char* filePath);
char** readFromFile(const char* filePath);
char* replaceStrStr(const char* string, const char* substr, const char* replacement);
JSONObj createJSONObj(const char* key, const char* value);
char* composeJSONObj(JSONObjs objs);
JSONObj getJSONObj(JSONObjs objs, const char* key);

JSONObj createJSONObj(const char* key, const char* value)
{
	/*
	*	Create a JSON Object from string key and value
	*/
	char* _key = replaceStrStr(key, "%20", " ");
	_key = replaceStrStr(_key, "%21", "!");
	_key = replaceStrStr(_key, "%22", "\"");
	_key = replaceStrStr(_key, "%23", "#");
	_key = replaceStrStr(_key, "%24", "$");
	_key = replaceStrStr(_key, "%25", "%");
	_key = replaceStrStr(_key, "%26", "&");
	_key = replaceStrStr(_key, "%27", "'");
	_key = replaceStrStr(_key, "%28", "(");
	_key = replaceStrStr(_key, "%29", ")");
	_key = replaceStrStr(_key, "%2A", "*");
	_key = replaceStrStr(_key, "%2B", "+");
	_key = replaceStrStr(_key, "%2C", ",");
	_key = replaceStrStr(_key, "%2D", "-");
	_key = replaceStrStr(_key, "%2E", ".");
	_key = replaceStrStr(_key, "%2F", "/");
	_key = replaceStrStr(_key, "%40", "@");
	_key = replaceStrStr(_key, "%5F", "_");

	char* _value = replaceStrStr(value, "%20", " ");
	_value = replaceStrStr(_value, "%21", "!");
	_value = replaceStrStr(_value, "%22", "\"");
	_value = replaceStrStr(_value, "%23", "#");
	_value = replaceStrStr(_value, "%24", "$");
	_value = replaceStrStr(_value, "%25", "%");
	_value = replaceStrStr(_value, "%26", "&");
	_value = replaceStrStr(_value, "%27", "'");
	_value = replaceStrStr(_value, "%28", "(");
	_value = replaceStrStr(_value, "%29", ")");
	_value = replaceStrStr(_value, "%2A", "*");
	_value = replaceStrStr(_value, "%2B", "+");
	_value = replaceStrStr(_value, "%2C", ",");
	_value = replaceStrStr(_value, "%2D", "-");
	_value = replaceStrStr(_value, "%2E", ".");
	_value = replaceStrStr(_value, "%2F", "/");
	_value = replaceStrStr(_value, "%40", "@");
	_value = replaceStrStr(_value, "%5F", "_");
	JSONObj obj;
	obj._key = (char*)malloc(1024);
	obj._value = (char*)malloc(1024);
	strcpy(obj._key, _key);
	strcpy(obj._value, _value);
	return obj;
}

char* composeJSONObj(JSONObjs objs)
{
	/*
	*	Compose a JSONObj pointer tp JSON string valid
	*/
	char* content = (char*)malloc(4096);
	strcpy(content, "{");
	int i = 0;	/* for loop */
	while(i<objs._size)
	{
		char* pair = (char*)malloc(512);
		if (i > 0)
			sprintf(pair, ",\"%s\":\"%s\"", objs._element[i]._key, objs._element[i]._value);
		else
			sprintf(pair, "\"%s\":\"%s\"", objs._element[i]._key, objs._element[i]._value);
		strcat(content, pair);
		free(pair);
		i++;
	}
	strcat(content, "}");
	return content;
}

JSONObj getJSONObj(JSONObjs objs, const char* key)
{
	JSONObj obj;
	for(int i=0;i<objs._size;i++)
	{
		if (countStrStr(objs._element[i]._key, key)==1)
		{
			obj = createJSONObj(key, objs._element[i]._value);
			return obj;
		}
	}
	obj = createJSONObj("", "");
	return obj;
}

int countStrStr(const char* str, const char* subStr)
{
	/*
	*	Count how many time a subStr appear in str
	*/
	int count = 0;
	for(int i=0;str[i]!='\0';i++)
	{
		if (str[i] == subStr[0])
		{
			int j = 0;
			for(j=0;subStr[j]!='\0';j++)
			{
				if (str[i+j] != subStr[j])
					break;
			}
			if (subStr[j] == '\0')
				count++;
		}
	}
	return count;
}

int compareStrStr(const char* str1, const char* str2)
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

int strcha(const char* str, char cha)
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

char** stringSplit(const char* str, char character, int times)
{
	/*
	*	Split a string (char*) into chunk of string (char**), break at character
	*	Example: 
	*		stringSplit("Hello Hello Hello Hello", ' ', 3)
	*		return
	*			"Hello"
	*			"Hello"
	*			"Hello"
	*			"Hello"
	*/
	char** str_return;
	int i = 0;	/* for loop */
	str_return = (char**)malloc(times*sizeof(char*));
	for(i=0;i<=times;i++)
		str_return[i] = (char*)malloc(KBYTE_T);
	int str_return_index = 0;
	char* buffer;
	int str_index = 0;
	while(times >= 0)
	{
		buffer = (char*)malloc(KBYTE_T);
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
	}

	return str_return;
}

int fileExists(const char* filePath)
{
	/*
	*	Check if a file exists or not
	*	filePath is a path to file. For example: /home/pmtoan/text.txt
	*	if (file exists) return 1, else return 0
	*/
	if(!access(filePath, F_OK ))
		return 1;
	else
		return 0;
}

char** readFromFile(const char* filePath)
{
	/*
	*	Open a file and read, return char** content of file
	*/
	if (fileExists(filePath) != 1)
		printf("[ERROR]    Error file not found\n");
	char** content;
	content = (char**)malloc(4*KBYTE_T);	/* Max row of file is 4096 lines */
	FILE* f_file = fopen(filePath, "r");
	if (f_file == NULL)
		printf("[ERROR]   Error while reading file\n");
	else
	{
		int i = 0;
		char* buffer = (char*)malloc(KBYTE_T);
		while(!feof(f_file))
		{
			fgets(buffer, KBYTE_T, f_file);
			content[i] = (char*)malloc(KBYTE_T);
			strcpy(content[i], buffer);
			i++;
		}
		free(buffer);
	}
	return content;
}

char* replaceStrStr(const char* string, const char* substr, const char* replacement)
{
	/*
	*	Replace a sub-string in a string
	*	Example:
	*		replaceStrStr("hello world", "hello", "hi") -> hi world
	*/
	char* tok = NULL;
	char* newstr = NULL;
	char* oldstr = NULL;
	int   oldstr_len = 0;
	int   substr_len = 0;
	int   replacement_len = 0;
	newstr = strdup(string);
	substr_len = strlen(substr);
	replacement_len = strlen(replacement);

	if (substr == NULL || replacement == NULL) {
		return newstr;
	}
	while ((tok = strstr(newstr, substr))) {
		oldstr = newstr;
		oldstr_len = strlen(oldstr);
		newstr = (char*)malloc(sizeof(char) * (oldstr_len - substr_len + replacement_len + 1));
		if (newstr == NULL) {
			free(oldstr);
			return NULL;
		}
		memcpy(newstr, oldstr, tok - oldstr);
		memcpy(newstr + (tok - oldstr), replacement, replacement_len);
		memcpy(newstr + (tok - oldstr) + replacement_len, tok + substr_len, oldstr_len - substr_len - (tok - oldstr));
		memset(newstr + oldstr_len - substr_len + replacement_len, 0, 1);
		free(oldstr);
	}
	return newstr;
}

#endif