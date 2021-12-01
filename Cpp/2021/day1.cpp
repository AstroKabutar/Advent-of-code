#include <iostream>
#include <sstream>
#include <array>
#include "file.hpp"
#include "challenge_file.hpp"

struct data
{
    std::stringstream m_conv;
    int m_previous{};
    int m_present{};

    int n_increase{};

    int depth_chk(const data& );

    void sstream_reset()
    {
        //reset the stringstream error flags and all
        m_conv.str("");
        m_conv.clear();
    }
};

struct data2
{
    std::array<int, 3> m_window{};
    int m_prev_win{};
    int m_pres_win{};

    int n_increase{};

    //will be used to change file pointer back 2 noise
    std::array<int, 2> fptr_changer{};

    int depth_chk(const data2& );
    void rm_noise(data2& );
};

int main()
{
    File day1{challenge_files::file1};

    std::ifstream rd1{day1.fname()};

    if(!rd1)
    {
        Error{errors::f_not_found};
        return -1;
    }

    //---------part1------
    data p1;

    //reading from input file
    while(day1.readl(rd1))
    {
        //taking in the string in stringstream
        p1.m_conv << day1.line();

        //converting string into int
        p1.m_conv >> p1.m_present;

        //for the 1st read of depth
        if(p1.m_previous == 0)
        {
            p1.m_previous = p1.m_present;
            p1.sstream_reset();
            continue;
        }

        //number of depth increase
        p1.n_increase += p1.depth_chk(p1);
        p1.m_previous = p1.m_present;

        //reset the stringstream error flags and all
        p1.sstream_reset();
    }
    
    rd1.close();
    std::cout << p1.n_increase << " is the number of total measurements larger then previous\n";
    
    p1.sstream_reset();

    //-------part2-------
    
    //reopen the file
    rd1.open(day1.fname());
    data2 p2;

    for(int i{}; ;++i)
    {
        if(i == 1)
            p2.fptr_changer[0] = rd1.tellg();

        //window formed
        if(i == 3)
        {
            p2.rm_noise(p2);
 //           std::cout << p2.m_pres_win << " -----\n";

            //for 1st window formation
            if(p2.m_prev_win == 0)
            {
                p2.m_prev_win = p2.m_pres_win;
                i = -1;

                //backing up 2 data
                rd1.seekg(p2.fptr_changer[0] - p2.fptr_changer[1], std::ios::cur);
                continue;
            }

            p2.n_increase += p2.depth_chk(p2);
            p2.m_prev_win = p2.m_pres_win;

            //backing up 2 data
            rd1.seekg(p2.fptr_changer[0] - p2.fptr_changer[1], std::ios::cur);

            //ready for next windows formation
            i = -1;
            continue;
        }

        if(!day1.readl(rd1))
            break;

        p1.m_conv << day1.line();
        p1.m_conv >> p2.m_window[i];

        p1.sstream_reset();

        if(i == 2)
            p2.fptr_changer[1] = rd1.tellg();
    }

    rd1.close();
    std::cout << p2.n_increase << " is the total number of increase in removed noise mode\n";
}

int data2::depth_chk(const data2& d)
{
    if(d.m_pres_win > d.m_prev_win)
        return 1;

    return 0;
}

void data2::rm_noise(data2& d)
{
    for(int x{},temp{}; x < 3; ++x)
    {
        temp += d.m_window[x];

        if(x == 2)
            d.m_pres_win = temp;
    }
}

int data::depth_chk(const data& d)
{
    if(d.m_present > d.m_previous)
        return 1;
    
    return 0;
}
