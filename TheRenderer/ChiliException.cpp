#include "ChiliException.h"
#include <sstream>

ChiliException::ChiliException(int line, const char* file) noexcept:line(line),file(file){}

const char* ChiliException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}
const char* ChiliException::GetType() const noexcept
{
	return "Text Exception";
}
int ChiliException::GetLine() const noexcept
{
	return line;
}
std::string ChiliException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File]" << file << std::endl
		<< "[Line]" << line;
	return oss.str();
}