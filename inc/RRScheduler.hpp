#pragma once

#include "Scheduler.hpp"
#include <algorithm>

class RRScheduler: public Scheduler
{
private:

public:
    RRScheduler() : Scheduler() {}

    RRScheduler(uint num_cores, uint time_unit) : Scheduler(num_cores, time_unit) {}

    void makeScheduleStep()
    {
        for (auto i = m_processor.begin(); i != m_processor.end(); ++i)
        {
            if(i->isReleasable() || !i->isWorking())
            {
                auto old = i->getProcess();
                if (old != nullptr)
                {
                    if(old->getStatus() != Status::DONE)
                    {
                        m_process_list.push_back(std::move(old));
                    }

                    for(auto n_it = i; n_it != m_processor.end(); n_it++)
                    {
                        *n_it = std::move(*(n_it + 1));
                    }

                    i--;
                }
            }
        }

        std::cerr << "LIST AFTER PUSH BACK: "; printProcessList(); std::cerr << std::endl;
        std::cerr << "CORES AFTER PUSH BACK: "; printProcessor(); std::cerr << std::endl;


        for (auto it_c = m_processor.begin(); it_c != m_processor.end(); it_c++)
        {
            if (!m_process_list.empty() && it_c->isEmpty())
            {
                    it_c->setProcess(std::move(m_process_list.front()), m_time_unit);
                    m_process_list.pop_front();

                    if(it_c != m_processor.begin())
                    {
                        std::vector<Core>::iterator it_prev = it_c;
                        do
                        {
                            if(it_prev->getCurrProcessId() < (--it_prev)->getCurrProcessId())
                                std::swap(*(it_prev + 1), *it_prev);
                        } 
                        while(it_prev != m_processor.begin());
                    }
            }
        }
    }

};

