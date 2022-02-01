#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <memory>

#include "Process.hpp"
#include "Core.hpp"

class Scheduler
{
protected:
    uint m_num_cores;
    uint m_time_unit;

    std::vector<Core> m_processor;
    std::list<std::unique_ptr<Process>> m_process_list;

public:
    Scheduler() : m_num_cores(1), m_time_unit(1), m_processor(m_num_cores) {}

    Scheduler(uint num_cores, uint time_unit) : m_num_cores(num_cores), m_time_unit(time_unit), m_processor(m_num_cores) {}

    void addProcess(std::unique_ptr<Process> p)
    {
        m_process_list.push_back(std::move(p));
    }

    virtual void makeScheduleStep() = 0;

    void runProcessor()
    {
        for (auto &c : m_processor)
        {
            if(!c.isEmpty())
                c.runProcess();
        }
    }

    std::string execStrategyStep(uint current_time, std::vector<Process> new_processes = std::vector<Process>())
    {
        std::ostringstream str;
        std::cerr << "======STEP " << current_time << "======" << std::endl;

        printProcessList();
        for(auto & p : new_processes)
        {
            addProcess(std::make_unique<Process>(p));
        }
        printProcessList();

        printProcessor();
        makeScheduleStep();
        printProcessor();

        runProcessor();

        str << current_time;
        for (const auto &c : m_processor)
        {
            str << ' ' << c.getCurrProcessId();
        }

        return str.str();
    }

    void printProcessList()
    {
        std::cerr << "Process List:" << std::endl;

        if(m_process_list.empty())
            std::cerr << "\t<Empty>" << std::endl;
        else
        {
            for(const auto & p : m_process_list)
            {
                std::cerr << '\t';
                p->print();
                std::cerr << std::endl;
            }
        }
    }

    void printProcessor()
    {
        std::cerr << "Processor:" << std::endl;
        uint i = 0;
        for(const auto & c : m_processor)
        {
            std::cerr << "\t Core " << i++ << ": ";
            c.print();
            std::cerr << std::endl;
        }
    }
};
