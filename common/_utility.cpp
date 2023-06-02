#include <cstdio>
#include <cstring>

#include <string>
#include <regex>
#include <iostream>
#include <fstream>

#include "common.h"
#include "log.h"

bool VocT_utility::is_file_exist(const std::string &filepath)
{
	if (filepath.empty())
	{
		log_err("filepath is empty!!!\n");
		return false;
	}
	
	std::wfstream f(filepath.c_str());
	if (f.good())
	{
		log_dbg("%s is exist\n", filepath.c_str());
		f.close();
		return true;
	}
	
	log_dbg("%s isn't exist\n", filepath.c_str());
	return false;
}

std::vector<std::string> split_string(const std::string &line, const std::string &delim)
{
	std::vector<std::string> result;
	wchar_t *tok = NULL, *save_ptr = NULL;
	wchar_t *line2 = wcsdup(line);
	
	if (line2)
	{
		for (tok = wcstok(line2, delim, &save_ptr);
		     tok;
		     tok = wcstok(NULL, delim, &save_ptr))
		{
			std::wstring string(tok);
			
			log_dbg("string == %ls\n", string.c_str());
			result.push_back(string);
		}
		free(line2);
	}
	
	return result;
}

std::vector<std::wstring> VocT_utility::split_string(const wchar_t *line, const wchar_t *delim)
{
	std::vector<std::wstring> result;
	wchar_t *tok = NULL, *save_ptr = NULL;
	wchar_t *line2 = wcsdup(line);
	
	if (line2)
	{
		for (tok = wcstok(line2, delim, &save_ptr);
		     tok;
		     tok = wcstok(NULL, delim, &save_ptr))
		{
			std::wstring string(tok);
			
			log_dbg("string == %ls\n", string.c_str());
			result.push_back(string);
		}
		free(line2);
	}
	
	return result;
}

int VocT_utility::create_file(const std::string &filepath)
{
	if (is_file_exist(filepath.c_str()))
		return 1;

	std::wfstream f(filepath);
	if (f.good())
	{
		f.close();
		return 0;
	}
	return -1;
}

bool VocT_utility::verify_entry(std::wstring &word, std::wstring &trans)
{
#if 0
	return true;
#else
	std::wregex expr_rus(L"[А-Яа-я -]+");
	std::wregex expr_en(L"[A-Za-z -]+");
	bool res = false;
	
	res = std::regex_match(word, expr_en);
	if (res)
	{
		log_dbg("expr_en SUCCESS (%ls)\n", word.c_str());
		res = std::regex_match(trans, expr_rus);
	}

	return res;
#endif
}
