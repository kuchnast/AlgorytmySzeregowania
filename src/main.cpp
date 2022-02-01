#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>

#include "FCFSScheduler.hpp"
#include "SJFScheduler.hpp"
#include "SRTFScheduler.hpp"
#include "RRScheduler.hpp"
#include "PriorityFCFSScheduler.hpp"
#include "PrioritySRTFScheduler.hpp"
#include "NonPreemPriorityFCFSScheduler.hpp"


bool charToUint(uint &param, char* str)
{
    std::string arg = str;

    try
    {
        std::size_t pos;
        param = (uint)std::stoul(arg, &pos);
    } 
    catch (std::invalid_argument const &e) 
    {
        std::cerr << "Invalid number: " << arg << '\n';
        return false;
    } 
    catch (std::out_of_range const &e) 
    {
        std::cerr << "Number out of range: " << arg << '\n';
        return false;
    }

    return true;
}

 bool readStepLine(std::vector<Process> &new_processes, uint curr_time)
{
    std::string line;
    uint t, i, p, r;

    new_processes.clear();

    std::getline(std::cin, line);
    std::cerr << "INPUT: " << line << std::endl;
    if(line.empty())
        return false;
    
    std::istringstream iss(line);
    iss >> t;
    if(curr_time != t)
        return false; 
    while(iss.good())
    {
        iss >> i >> p >> r;
        if(iss.fail())
            break;
        new_processes.push_back(Process(i,p,r));
    }

    return true;
}

std::unique_ptr<Scheduler>makeScheduler(uint sh_algorithm = 0, uint num_of_procesors = 1, uint time_unit = 1)
{
    switch(sh_algorithm)
    {
        case 0:
            return std::make_unique<FCFSScheduler>(FCFSScheduler(num_of_procesors, time_unit));
        case 1:
            return std::make_unique<SJFScheduler>(SJFScheduler(num_of_procesors, time_unit));
        case 2:
            return std::make_unique<SRTFScheduler>(SRTFScheduler(num_of_procesors, time_unit));
        case 3:
            return std::make_unique<RRScheduler>(RRScheduler(num_of_procesors, time_unit));
        case 4:
            return std::make_unique<PriorityFCFSScheduler>(PriorityFCFSScheduler(num_of_procesors, time_unit));
        case 5:
            return std::make_unique<PrioritySRTFScheduler>(PrioritySRTFScheduler(num_of_procesors, time_unit));
        case 6:
            return std::make_unique<NonPreemPriorityFCFSScheduler>(NonPreemPriorityFCFSScheduler(num_of_procesors, time_unit));
        default:
            return nullptr;
    }

    return nullptr;
}

int main(int argc, char** argv)
{
    uint sh_algorithm = 0;
    uint num_of_procesors = 1;
    uint time_unit = 1;
    std::unique_ptr<Scheduler> scheduler;
    std::vector<Process> new_processes;
    uint time = 0;

    switch(argc)
    {
        case 1:
            break;

        case 2:
            if(!charToUint(sh_algorithm, argv[1]))
                return 1;
            break;

        case 3:
            if(!charToUint(sh_algorithm, argv[1]))
                return 1;
            if(!charToUint(num_of_procesors, argv[2]))
                return 1;
            break;

        case 4:
            if(!charToUint(sh_algorithm, argv[1]))
                return 1;
            if(!charToUint(num_of_procesors, argv[2]))
                return 1;
            if(!charToUint(time_unit, argv[3]))
                return 1;
            break;

        default:
            break;
    }

    scheduler = makeScheduler(sh_algorithm, num_of_procesors, time_unit);

while(1)
{
    while(readStepLine(new_processes, time))
    {
        std::cout << scheduler->execStrategyStep(time, new_processes) << std::endl;
        ++time;
    }

    while (1)
    {
        std::string temp = scheduler->execStrategyStep(time);
        std::cout << temp << std::endl;
        if(std::count(temp.begin(), temp.end(), '-') == num_of_procesors)
            return 0;
        ++time;
    }
}
    return 0;
}