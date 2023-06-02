#pragma once

#include <cstdarg>

class Log
{
public:
	enum class LogLevel
	{
		LL_ERROR = 0,
		LL_INFO,
		LL_DEBUG,
		LL_MAX
	};

private:
	LogLevel level;

	void print(LogLevel lvl, const char *lvl_prompt, const char *fmt, va_list ap);

public:
	Log():
		level(LogLevel::LL_ERROR)
			{};
	Log(LogLevel lvl):
		level(lvl)
			{};

	void error(const char *fmt, ...);
	void debug(const char *fmt, ...);
	void info(const char *fmt, ...);
};
