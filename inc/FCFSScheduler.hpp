#pragma once

#include "Scheduler.hpp"

class FCFSScheduler: public Scheduler
{
private:
   
public:
    FCFSScheduler() : Scheduler() {}

    FCFSScheduler(uint num_cores, uint time_unit) : Scheduler(num_cores, time_unit) {}

    void makeScheduleStep()
    {
        for (auto i = m_processor.rbegin(); i != m_processor.rend(); ++i)
        {
            auto old = i->getProcess();
            if (old != nullptr)
                if(old->getStatus() != Status::DONE)
                    m_process_list.push_front(std::move(old));
        }

        for (auto &c : m_processor)
        {
            if (!m_process_list.empty())
            {
                c.setProcess(std::move(m_process_list.front()));
                m_process_list.pop_front();
            }
        }
    }

};

