#pragma once

#include "Scheduler.hpp"
#include <algorithm>

class SJFScheduler: public Scheduler
{
private:
  std::list<std::unique_ptr<Process>>::iterator m_findShortestTask()
  {
      std::list<std::unique_ptr<Process>>::iterator it_min = m_process_list.begin();

      for(auto it = m_process_list.begin(); it != m_process_list.end(); it++)
      {
          if(it->get()->getTime() < it_min->get()->getTime())
            it_min = it;
      }

      return it_min;
  }


public:
    SJFScheduler() : Scheduler() {}

    SJFScheduler(uint num_cores, uint time_unit) : Scheduler(num_cores, time_unit) {}

    void makeScheduleStep()
    {
        for (auto i = m_processor.rbegin(); i != m_processor.rend(); ++i)
        {
            auto old = i->getProcess();
            if (old != nullptr)
                if(old->getStatus() != Status::DONE)
                    m_process_list.push_front(std::move(old));  // Put all busy processes on front
        }


        for (auto &c : m_processor)
        {
            if (!m_process_list.empty())
            {
                if(m_process_list.front()->getStatus() == Status::BUSY) //First, use all busy processes from front 
                {
                    c.setProcess(std::move(m_process_list.front()));
                    m_process_list.pop_front();
                }
                else
                {
                    auto it = m_findShortestTask();
                    c.setProcess(std::move(*it));
                    m_process_list.erase(it);
                }
            }
        }
    }

};

