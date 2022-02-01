#pragma once
#include <iostream>

enum class Status {NEW, BUSY, DONE};

class Process
{
private:
    const uint m_id;
    const uint m_priority;
    const uint m_time;

    uint m_progress;

public:
    Process() = delete;

    Process(uint id, uint priority, uint time) : m_id(id), m_priority(priority), m_time(time), m_progress(0) {}

    Status execProcess()
    {
        if(++m_progress >= m_time)
            return Status::DONE;
        
        return Status::BUSY;
    }

    Status getStatus() const
    {
        if(m_progress == 0)
            return Status::NEW;

        return (m_progress >= m_time ? Status::DONE : Status::BUSY);
    }

    uint getPriority() const
    {
        return m_priority;
    }

    uint getId() const
    {
        return m_id;
    }

    uint getTime() const
    {
        return m_time;
    }

    uint getProgress() const
    {
        return m_progress;
    }

    uint getTimeToEnd() const
    {
        return m_time - m_progress;
    }


    void print() const
    {
        std::cerr << "Id: " << m_id << " Time: " << m_time << " Priority: " << m_priority << " Progress: " << m_progress;
    }
};