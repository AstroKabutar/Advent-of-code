#include "file.hpp"

bool File::readl(std::ifstream& fptr)
{
	std::getline(fptr, m_line);

	if(fptr.eof())
		return false;
	
	return true;
}

bool File::readc(std::ifstream& fptr)
{
	fptr.get(m_ch);

	if(fptr.eof())
		return false;

	return true;
}