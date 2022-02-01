#pragma once

#include "Scheduler.hpp"
#include <algorithm>

class PrioritySRTFScheduler: public Scheduler
{
private:

    std::list<std::unique_ptr<Process>>::iterator m_findHighestPriority()
    {
        std::list<std::unique_ptr<Process>>::iterator it_max = m_process_list.begin();

        for(auto it = m_process_list.begin(); it != m_process_list.end(); it++)
        {
            //std::cerr << "Compare PRIORITY("; it->get()->print(); std::cerr << " AND "; it_max->get()->print(); std::cerr << ')' << std::endl;
            if(it->get()->getPriority() < it_max->get()->getPriority())
                it_max = it;
            else if(it->get()->getPriority() == it_max->get()->getPriority())
            {
            //std::cerr << "Compare ID("; it->get()->print(); std::cerr << " AND "; it_max->get()->print(); std::cerr << ')' << std::endl;
                if(it->get()->getTimeToEnd() < it_max->get()->getTimeToEnd())
                {
                    it_max = it;
                }
                else if(it->get()->getTimeToEnd() == it_max->get()->getTimeToEnd())
                {
                    if(it->get()->getId() < it_max->get()->getId())
                        it_max = it;
                }
                
            }
        }
        //std::cerr << "Winner("; it_max->get()->print(); std::cerr << ')' << std::endl;

        return it_max;
    }


public:
    PrioritySRTFScheduler() : Scheduler() {}

    PrioritySRTFScheduler(uint num_cores, uint time_unit) : Scheduler(num_cores, time_unit) {}

    void makeScheduleStep()
    {
        for (auto i = m_processor.rbegin(); i != m_processor.rend(); ++i)
        { 
            if(i->isReleasable() || !i->isWorking())
            {
                auto old = i->getProcess();
                if (old != nullptr)
                {
                    if(old->getStatus() != Status::DONE)
                    {
                        m_process_list.push_front(std::move(old));  // Put all non-preemptive processes on front
                    }

                    for(auto n_it = i; n_it != m_processor.rbegin(); --n_it)
                    {
                        *n_it = std::move(*(n_it - 1));
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
                    auto it = m_findHighestPriority();
                    it_c->setProcess(std::move(*it), m_time_unit);
                    m_process_list.erase(it);

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

