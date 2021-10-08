#include "file.hpp"

int main(int agrc, char* argv[])
{
	File read{argv[1]};
	//read the filename
	
	std::ifstream file{read.fname()};
	if(!file)
	{
		Error{errors::f_not_found};
		return 1;
	}

	int floor{};
	
	//part 1
	while(read.readc(file))
	{
		if(read.ch() == '(')
			++floor;
		if(read.ch() == ')')
			--floor;
	}
	
	std::cout << "Floor number: " << floor << '\n';	
//	file.close();
	
	//part2
//	file.open(read.fname());
	
	file.clear();
	//to reset the eof of file so that seek works
	file.seekg(0, std::ios::beg);
	//seek to the beginning to file 
	floor = 0;
	int pos{};
	while(read.readc(file))
	{
		if(read.ch() == '(')
		{
			++floor, ++pos;
			if(floor == -1)
				break;
		}
		
		if(read.ch() == ')')
		{
			--floor, ++pos;
			if(floor == -1)
				break;
		}
		
	}
	
	std::cout << "Basement entry position: " << pos << '\n';
}
