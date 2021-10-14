#include <array>
#include <vector>
#include <cmath>    //for std::sqrt(), std::pow()
#include "file.hpp"

namespace temp
{
    constexpr int num{20};
    //to be used in .reserve() for vector\
    to prevent repeted reallocations

    int house_visited{1};
    //1 because the starting is also counted\
    as 1

    int house_visited2{1};
    //part 2 of the question
}

struct data
{
    std::vector<int> m_pos_x{};
    std::vector<int> m_pos_y{};
    //to store position of houses visited

    data()  noexcept
    {
        m_pos_x.reserve(temp::num);
        m_pos_y.reserve(temp::num);

        m_pos_x.push_back(0);
        m_pos_y.push_back(0);
        //to store origin as its counted as 1st house
    }

    std::array<int, 2> m_pos{};
    //m_pos[x,y]\
    to store the current position of visit

    bool m_hasVisited{};
    //a flag for revisit

    int m_length{};
    //storing the dynamic array length
};

//part 2
struct santa
{
    std::array<int, 2> pos_santa{};
    //[x,y]

    std::vector<int> santa_x{};
    std::vector<int> santa_y{};
};
struct robosanta
{
    std::array<int, 2> pos_robosanta{};
    //[x,y]

    std::vector<int> robo_x{};
    std::vector<int> robo_y{};
};

struct turns: public santa, public robosanta
{
    turns()
    {
        santa_x.reserve(temp::num);
        santa_y.reserve(temp::num);

        santa_x.push_back(0);
        santa_y.push_back(0);
        //as santa always starts the game

        robo_x.reserve(temp::num);
        robo_y.reserve(temp::num);
    }

    bool isSanta{};

    std::array<bool, 2> hasvisited{};
    //bool[0] = santa | bool[1] = robosanta

    int robolength{};
    int santalength{};

    size_t turn{};
    //even means santa [0,2,4,6,8,....]\
    odd means robo santa [1,3,5,7,9,...]

    //check for revistis by other santa
    bool santa_check()
    {
        //if santa already visited then its a revisit
        for(int x{}; x < santalength; ++x)
        {
            if(pos_robosanta[0] == santa_x[x] && pos_robosanta[1] == santa_y[x])
                return true;
        }
        
        return false;
    }
    bool robo_santa_check()
    {
        //if robosanta already visited then its a revisit
        for(int x{}; x < robolength; ++x)
        {
            if(pos_santa[0] == 0 && pos_santa[1] == 0)
                continue;
            if(pos_santa[0] == robo_x[x] && pos_santa[1] == robo_y[x])
                return true;
        }
        
        return false;
    }
};

void data_fill(data& , char , turns& );

int main(int argc, char* argv[])
{
    File read{argv[1]};
    std::ifstream fptr{read.fname()};
    
    if(!fptr)
    {
        Error{errors::f_not_found};
        return -1;
    }

    data store;
    turns robo_santa;

    while(read.readc(fptr))
    {
        switch(read.ch())
        {
            //north
            case '^':
            data_fill(store, '^', robo_santa);
            break;

            //south
            case 'v':
            data_fill(store, 'v', robo_santa);
            break;

            //east
            case '>':
            data_fill(store, '>', robo_santa);
            break;

            //west
            case '<':
            data_fill(store, '<', robo_santa);
            break;

            default:
            break;
        }
    }

    //part 1
    std::cout << temp::house_visited << " houses got atleast 1 present\n";
    //part2
    std::cout << temp::house_visited2 << " houses visited while taking turns\n";
}

void data_fill(data& d, char ch, turns& santa)
{
    switch(ch)
    {
        case '^':
        d.m_pos[1] += 1;

        if(santa.turn % 2 == 0)  //if turns are even
        {
            santa.isSanta = true;
            ++santa.pos_santa[1]; 
        }

        if(santa.turn % 2 != 0) //if turns are odd
        {
            santa.isSanta = false;
            ++santa.pos_robosanta[1];
        }

        santa.robolength    = static_cast<int>(santa.robo_x.size());
        santa.santalength   = static_cast<int>(santa.santa_x.size());
        d.m_length          = static_cast<int>(d.m_pos_x.size());
        //get present length of D-array

        //part 1
        for(int x{}; x < d.m_length; ++x)
        {
            if(d.m_pos[0] == d.m_pos_x[x] && d.m_pos[1] == d.m_pos_y[x])
            {
                d.m_hasVisited = true;
                break;
            }
        }

        //part 2
        //robosanta visites co-ordinates
        for(int x{}; x < santa.robolength; ++x)
        {
            if(santa.isSanta)
                break;

            if(santa.pos_robosanta[0] == santa.robo_x[x] && santa.pos_robosanta[1] == santa.robo_y[x])
            {
                santa.hasvisited[1] = true;
                break;
            }
        }

        //santa visites co-ordinates
        for(int x{}; x < santa.santalength; ++x)
        {
            if(!santa.isSanta)
                break;
                
            if(santa.pos_santa[0] == santa.santa_x[x] && santa.pos_santa[1] == santa.santa_y[x])
            {
                santa.hasvisited[0] = true;
                break;
            }
        }

        if(!d.m_hasVisited)
        {
            d.m_pos_x.push_back(d.m_pos[0]);
            d.m_pos_y.push_back(d.m_pos[1]);

            temp::house_visited += 1;
        }

        if(!santa.hasvisited[0] && santa.isSanta)
        {
            santa.santa_x.push_back(santa.pos_santa[0]);
            santa.santa_y.push_back(santa.pos_santa[1]);

            if(!santa.robo_santa_check())
                temp::house_visited2 += 1;
        }

        if(!santa.hasvisited[1] && !santa.isSanta)
        {
            santa.robo_x.push_back(santa.pos_robosanta[0]);
            santa.robo_y.push_back(santa.pos_robosanta[1]);

            if(!santa.santa_check())
                temp::house_visited2 += 1;
        }

        if(d.m_hasVisited || santa.hasvisited[0] || santa.hasvisited[1])
        {
            d.m_hasVisited = false;
            santa.hasvisited[0] = false;
            santa.hasvisited[1] = false;
            //reset flags
        }

        ++santa.turn;
        break;

        case 'v':
        d.m_pos[1] -= 1;

        if(santa.turn % 2 == 0)  //if turns are even
        {
            santa.isSanta = true;
            --santa.pos_santa[1]; 
        }

        if(santa.turn % 2 != 0) //if turns are odd
        {
            santa.isSanta = false;
            --santa.pos_robosanta[1];
        }

        santa.robolength    = static_cast<int>(santa.robo_x.size());
        santa.santalength   = static_cast<int>(santa.santa_x.size());
        d.m_length          = static_cast<int>(d.m_pos_x.size());
        //get present length of D-array

        //part 1
        for(int x{}; x < d.m_length; ++x)
        {
            if(d.m_pos[0] == d.m_pos_x[x] && d.m_pos[1] == d.m_pos_y[x])
            {
                d.m_hasVisited = true;
                break;
            }
        }

        //part 2
        //robosanta visites co-ordinates
        for(int x{}; x < santa.robolength; ++x)
        {
            if(santa.isSanta)
                break;

            if(santa.pos_robosanta[0] == santa.robo_x[x] && santa.pos_robosanta[1] == santa.robo_y[x])
            {
                santa.hasvisited[1] = true;
                break;
            }
        }

        //santa visites co-ordinates
        for(int x{}; x < santa.santalength; ++x)
        {
            if(!santa.isSanta)
                break;
               
            if(santa.pos_santa[0] == santa.santa_x[x] && santa.pos_santa[1] == santa.santa_y[x])
            {
                santa.hasvisited[0] = true;
                break;
            }
        }

        if(!d.m_hasVisited)
        {
            d.m_pos_x.push_back(d.m_pos[0]);
            d.m_pos_y.push_back(d.m_pos[1]);

            temp::house_visited += 1;
        }

        if(!santa.hasvisited[0] && santa.isSanta)
        {
            santa.santa_x.push_back(santa.pos_santa[0]);
            santa.santa_y.push_back(santa.pos_santa[1]);

            if(!santa.robo_santa_check())
                temp::house_visited2 += 1;
        }

        if(!santa.hasvisited[1] && !santa.isSanta)
        {
            santa.robo_x.push_back(santa.pos_robosanta[0]);
            santa.robo_y.push_back(santa.pos_robosanta[1]);

            if(!santa.santa_check())
                temp::house_visited2 += 1;
        }

        if(d.m_hasVisited || santa.hasvisited[0] || santa.hasvisited[1])
        {
            d.m_hasVisited = false;
            santa.hasvisited[0] = false;
            santa.hasvisited[1] = false;
            //reset flags
        }

        ++santa.turn;
        break;

        case '>':
        d.m_pos[0] += 1;

        if(santa.turn % 2 == 0)  //if turns are even
        {
            santa.isSanta = true;
            ++santa.pos_santa[0]; 
        }

        if(santa.turn % 2 != 0) //if turns are odd
        {
            santa.isSanta = false;
            ++santa.pos_robosanta[0];
        }

        santa.robolength    = static_cast<int>(santa.robo_x.size());
        santa.santalength   = static_cast<int>(santa.santa_x.size());
        d.m_length          = static_cast<int>(d.m_pos_x.size());
        //get present length of D-array

        //part 1
        for(int x{}; x < d.m_length; ++x)
        {
            if(d.m_pos[0] == d.m_pos_x[x] && d.m_pos[1] == d.m_pos_y[x])
            {
                d.m_hasVisited = true;
                break;
            }
        }

        //part 2
        //robosanta visites co-ordinates
        for(int x{}; x < santa.robolength; ++x)
        {
            if(santa.isSanta)
                break;

            if(santa.pos_robosanta[0] == santa.robo_x[x] && santa.pos_robosanta[1] == santa.robo_y[x])
            {
                santa.hasvisited[1] = true;
                break;
            }
        }

        //santa visites co-ordinates
        for(int x{}; x < santa.santalength; ++x)
        {
            if(!santa.isSanta)
                break;
                
            if(santa.pos_santa[0] == santa.santa_x[x] && santa.pos_santa[1] == santa.santa_y[x])
            {
                santa.hasvisited[0] = true;
                break;
            }
        }

        if(!d.m_hasVisited)
        {
            d.m_pos_x.push_back(d.m_pos[0]);
            d.m_pos_y.push_back(d.m_pos[1]);

            temp::house_visited += 1;
        }

        if(!santa.hasvisited[0] && santa.isSanta)
        {
            santa.santa_x.push_back(santa.pos_santa[0]);
            santa.santa_y.push_back(santa.pos_santa[1]);

            if(!santa.robo_santa_check())
                temp::house_visited2 += 1;
        }

        if(!santa.hasvisited[1] && !santa.isSanta)
        {
            santa.robo_x.push_back(santa.pos_robosanta[0]);
            santa.robo_y.push_back(santa.pos_robosanta[1]);

            if(!santa.santa_check())
                temp::house_visited2 += 1;
        }

        if(d.m_hasVisited || santa.hasvisited[0] || santa.hasvisited[1])
        {
            d.m_hasVisited = false;
            santa.hasvisited[0] = false;
            santa.hasvisited[1] = false;
            //reset flags
        }

        ++santa.turn;
        break;

        case '<':
        d.m_pos[0] -= 1;

        if(santa.turn % 2 == 0)  //if turns are even
        {
            santa.isSanta = true;
            --santa.pos_santa[0]; 
        }

        if(santa.turn % 2 != 0) //if turns are odd
        {
            santa.isSanta = false;
            --santa.pos_robosanta[0];
        }

        santa.robolength    = static_cast<int>(santa.robo_x.size());
        santa.santalength   = static_cast<int>(santa.santa_x.size());
        d.m_length          = static_cast<int>(d.m_pos_x.size());
        //get present length of D-array

        //part 1
        for(int x{}; x < d.m_length; ++x)
        {
            if(d.m_pos[0] == d.m_pos_x[x] && d.m_pos[1] == d.m_pos_y[x])
            {
                d.m_hasVisited = true;
                break;
            }
        }

        //part 2
        //robosanta visites co-ordinates
        for(int x{}; x < santa.robolength; ++x)
        {
            if(santa.isSanta)
                break;

            if(santa.pos_robosanta[0] == santa.robo_x[x] && santa.pos_robosanta[1] == santa.robo_y[x])
            {
                santa.hasvisited[1] = true;
                break;
            }
        }

        //santa visites co-ordinates
        for(int x{}; x < santa.santalength; ++x)
        {
            if(!santa.isSanta)
                break;
                
            if(santa.pos_santa[0] == santa.santa_x[x] && santa.pos_santa[1] == santa.santa_y[x])
            {
                santa.hasvisited[0] = true;
                break;
            }
        }

        if(!d.m_hasVisited)
        {
            d.m_pos_x.push_back(d.m_pos[0]);
            d.m_pos_y.push_back(d.m_pos[1]);

            temp::house_visited += 1;
        }

        if(!santa.hasvisited[0] && santa.isSanta)
        {
            santa.santa_x.push_back(santa.pos_santa[0]);
            santa.santa_y.push_back(santa.pos_santa[1]);

            if(!santa.robo_santa_check())
                temp::house_visited2 += 1;
        }

        if(!santa.hasvisited[1] && !santa.isSanta)
        {
            santa.robo_x.push_back(santa.pos_robosanta[0]);
            santa.robo_y.push_back(santa.pos_robosanta[1]);

            if(!santa.santa_check())
                temp::house_visited2 += 1;
        }

        if(d.m_hasVisited || santa.hasvisited[0] || santa.hasvisited[1])
        {
            d.m_hasVisited = false;
            santa.hasvisited[0] = false;
            santa.hasvisited[1] = false;
            //reset flags
        }

        ++santa.turn;
        break;

        default:
        break;
    }
}
