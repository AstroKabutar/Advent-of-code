#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include "file.hpp"
#include "challenge_file.hpp"

namespace mv
{
    enum class cmd
    {
        up = 1,
        down,
        forward,
        total
    };

    enum mv::cmd command(const std::string& );
}

struct data
{
    //in part 2 the y part will become the AIM changes
    
    //0 == x, 1 == y
    std::array<int, 2> m_coordinate{};

    //temp_cord is the new read coordinates from file
    std::array<int, 2> m_temp_cord{};


    std::stringstream m_conv;

    void sstream_reset()
    {
        m_conv.str("");
        m_conv.clear();
    }

    //for part 2
    std::vector<int> m_aim{};
};

int main(void)
{
    File day2{challenge_files::file2};

    std::ifstream rd2{day2.fname()};
    if(!rd2)
    {
        Error{errors::f_not_found};
        return -1;
    }

    //------part 1--------
    data d1;

    while(day2.readw(rd2))
    {
        //checking the movement command
        switch(mv::command(day2.word()))
        {
            case mv::cmd::up:                                         //UP
            //read the steps
            day2.readw(rd2);

            //exract the steps in present temp coordinate
            d1.m_conv << day2.word();
            d1.m_conv >> d1.m_temp_cord[1];

            //a string stream reset
            d1.sstream_reset();

            //putting it in final coordinates
            d1.m_coordinate[1] += d1.m_temp_cord[1];
            continue;

            case mv::cmd::down:                                       //DOWN
            //read the steps
            day2.readw(rd2);

            //exract the steps in present temp coordinate
            d1.m_conv << day2.word();
            d1.m_conv >> d1.m_temp_cord[1];

            //a string stream reset
            d1.sstream_reset();

            //putting it in final coordinates
            d1.m_coordinate[1] -= d1.m_temp_cord[1];
            continue;

            case mv::cmd::forward:                                      //FORWARD

            //read the steps
            day2.readw(rd2);

            //exract the steps in present temp coordinate
            d1.m_conv << day2.word();
            d1.m_conv >> d1.m_temp_cord[0];

            //a string stream reset
            d1.sstream_reset();

            //putting it in final coordinates
            d1.m_coordinate[0] += d1.m_temp_cord[0];
            continue;
        }
    }

    std::cout << "Final Horizontal: " << d1.m_coordinate[0] << '\t';
    std::cout << "Final Vertical:   " << d1.m_coordinate[1] << '\n';
    std::cout << "Multiplied:       " << (d1.m_coordinate[0] * d1.m_coordinate[1]) << "\n\n";
    rd2.close();

    //--------part 2 ------------
    data d2;
    d2.m_aim.reserve(15);

    rd2.open(day2.fname());

    while(day2.readw(rd2))
    {
        switch(mv::command(day2.word()))
        {
            case mv::cmd::forward:                                          //FORWARD
            
            //read the value
            day2.readw(rd2);

            d2.m_conv << day2.word();
            d2.m_conv >> d2.m_temp_cord[0];

            //a string stream reset
            d2.sstream_reset();

            //new horizontal position
            d2.m_coordinate[0] += d2.m_temp_cord[0];

            //depth change of sub
            d2.m_aim.push_back(d2.m_coordinate[1] * d2.m_temp_cord[0]);
            continue;

            case mv::cmd::up:                                                  //UP

            //read the value
            day2.readw(rd2);

            d2.m_conv << day2.word();
            d2.m_conv >> d2.m_temp_cord[1];

            //a string stream reset
            d2.sstream_reset();

            //new depth after decrease
            d2.m_coordinate[1] = d2.m_coordinate[1] - d2.m_temp_cord[1];
            continue;

            case mv::cmd::down:                                                  //DOWN

            //read the value
            day2.readw(rd2);

            d2.m_conv << day2.word();
            d2.m_conv >> d2.m_temp_cord[1];

            //a string stream reset
            d2.sstream_reset();

            //new depth after increase
            d2.m_coordinate[1] += d2.m_temp_cord[1];
            continue;
        }
    }

    rd2.close();

    int total_depth{};

    //getting total depth of submarine
    for(int x: d2.m_aim)
    {
        //turn it into positive number
        if(x < 0)
            x = (-1) * x;

        total_depth += x;
    }

    std::cout << "Final Horizontal: " << d2.m_coordinate[0] << '\t';
    std::cout << "Total depth:      " << total_depth << '\n';
    std::cout << "Multiplied:       " << (d2.m_coordinate[0] * total_depth) << '\n';
}

enum mv::cmd mv::command(const std::string& str)
{
    if(str == "forward")
        return mv::cmd::forward;
    else if(str == "up")
        return mv::cmd::up;
    else if(str == "down")
        return mv::cmd::down;
    else
        return mv::cmd::total;
}
