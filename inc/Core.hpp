#pragma once

#include <iostream>
#include <memory>
#include <exception>

#include "Process.hpp"

class Core
{
private:
    std::unique_ptr<Process> m_curr_process;
    uint m_time_to_release;
public:
    Core() : m_curr_process(nullptr), m_time_to_release(0) {}

    std::unique_ptr<Process> changeProcess(std::unique_ptr<Process> new_process)
    {
        auto temp = std::move(m_curr_process);
        m_curr_process = std::move(new_process);
        return std::make_unique<Process>(*temp);
    }

    void setProcess(std::unique_ptr<Process> new_process, uint time_to_release = 0)
    {
        if(m_curr_process != nullptr)
            throw std::invalid_argument("Another process on core");

        m_curr_process = std::make_unique<Process>(*new_process);
        m_time_to_release = time_to_release;
    }

    std::unique_ptr<Process> getProcess()
    {
        auto temp = std::move(m_curr_process);
        return (temp != nullptr ? std::make_unique<Process>(*temp) : nullptr);
    }

    bool isEmpty() const
    {
        return (m_curr_process == nullptr ? true : false);
    }

    int getCurrProcessId() const
    {
        return (m_curr_process != nullptr ? (int)(m_curr_process->getId()) : -1);
    }

    bool isWorking() const
    {
        if(m_curr_process != nullptr)
        {
            return (m_curr_process->getStatus() != Status::DONE ? true : false);
        }

        return false;
    }

    bool isReleasable() const
    {
        return (m_time_to_release ? false : true);
    }

    Status runProcess()
    {
        if(m_curr_process != nullptr)
        {
            if(m_time_to_release)
                --m_time_to_release;
            return m_curr_process->execProcess();
        }
        
        throw std::invalid_argument("No process on core");
    }

    void print() const
    {
        if(m_curr_process == nullptr)
            std::cerr << "<Empty>";
        else
        {
            m_curr_process->print();
            std::cerr << " Release for: " << m_time_to_release;
        }
    }

};
