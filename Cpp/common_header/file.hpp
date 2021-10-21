#ifndef FILE_HPP
#define FILE_HPP

#include <fstream>
#include <string>
#include <string_view>
#include <iostream>

namespace errors
{
	constexpr std::string_view f_not_found{"Failed to open file"};
}

class File
{
private:
	std::string m_fname{};
	std::string m_line{};
	char m_ch{};
	
public:
	File(const std::string& fname)	noexcept
	:m_fname{fname}
	{}
	
	//getter functions+++++++++++++++++++++++++++++++++++
	const std::string& fname() const { return m_fname; }
	const std::string& line() const { return m_line; }
	char ch() const { return m_ch; }
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	//read an entire line from file at a time
	bool readl(std::ifstream& fptr);
	
	//read a character one at a time
	bool readc(std::ifstream& fptr);
};

class Error
{
public:
	Error(const std::string_view& error)
	{
		std::cerr << error << '\n';
	}
};
#endif
