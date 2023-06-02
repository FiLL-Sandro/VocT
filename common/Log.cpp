#include <cstdio>

#include "Log.h"

#define ERROR_PROMPT "ERROR > "
#define DEBUG_PROMPT "DEBUG > "
#define INFO_PROMPT  "INFO  > "

void Log::print(LogLevel lvl, const char *lvl_prompt, const char *fmt, va_list ap)
{
	if (lvl <= level)
	{
		printf("%s", lvl_prompt);
		vprintf(fmt, ap);
	}
}

void Log::error(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	print(LogLevel::LL_ERROR, ERROR_PROMPT, fmt, ap);
	va_end(ap);
}

void Log::debug(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	print(LogLevel::LL_DEBUG, DEBUG_PROMPT, fmt, ap);
	va_end(ap);
}

void Log::info(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	print(LogLevel::LL_INFO, INFO_PROMPT, fmt, ap);
	va_end(ap);
}
