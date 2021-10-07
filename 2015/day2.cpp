#include <array>
#include <sstream>
#include <algorithm>
#include "file.hpp"

struct data
{
	int l{}, w{}, h{};
	int sum{},ribbon{};
	int peri{},vol{};
	int a1{}, a2{}, a3{};
	int la{};
	int count{};
	std::string line{};
	
	void replace();
	void fill();
};

int main(int argc, char* argv[])
{
	File pkg{argv[1]};
	std::ifstream fptr{pkg.fname()};
	if(!fptr)
	{
		Error{errors::f_not_found};
		return 1;
	}
	
	std::stringstream os;
	data param;
	while(pkg.readl(fptr))
	{	++param.count;
		param.line = pkg.line();
		//assign the line in struct
		
		param.replace();
		//function to check and replace x with ' '
		
		os << param.line;
		//pushing the string of "###  ###  ###" in buffer
		
		os >> param.l >> param.w >> param.h;
		//converting string to numbers

		param.fill();
		//function which fills all the datafield in data struct
		
		os.str("");
		os.clear();
		//a reset of stringstream of error flags and all
	}
	
	std::cout << param.sum << " sq feet\t" << param.ribbon << " ft ribbon\n";
}

void data::replace()
{
	int len = line.length();
	
	for(int x{}; x < len; ++x)
	{
		if(line[x] == 'x' || line[x] == 'X')
			line[x] = ' ';
	}
}

void data::fill()
{
	//fill the areas
	a1 = 2 * l * w;
	a2 = 2 * l * h;
	a3 = 2 * w * h;
	vol = l * w * h;
	std::array sides{l,w,h};
	
	//filling the least area
	//note: Bug data same dimentions causes a problem in if-else
	//note 2 updated: if we give the <= sign it will also work
	if(a1 <= a2 && a1 <= a3)
	{
		la = a1/2;
		std::sort(sides.begin(), sides.end());
		peri = 2*(sides[0] + sides[1]);
	}
	if(a2 <= a1 && a2 <= a3)
	{
		la = a2/2;
		std::sort(sides.begin(), sides.end());
		peri = 2*(sides[0] + sides[1]);
	}
	if(a3 <= a1 && a3 <= a2)
	{
		la = a3/2;
		std::sort(sides.begin(), sides.end());
		peri = 2*(sides[0] + sides[1]);
	}
	
	//filling total area required
	sum += a1 + a2 + a3 + la;
	ribbon += vol + peri;
}