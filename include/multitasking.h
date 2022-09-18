#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

namespace myos
{
struct CPUState
{
    // Pushed by us
    myos::common::uint32_t eax;
    myos::common::uint32_t ebx;
    myos::common::uint32_t ecx;
    myos::common::uint32_t edx;

    myos::common::uint32_t esi;
    myos::common::uint32_t edi;
    myos::common::uint32_t ebp;
    /*
            myos::common::uint32_t gs;
            myos::common::uint32_t fs;
            myos::common::uint32_t es;
            myos::common::uint32_t eds;
    */
    myos::common::uint32_t error;
    // Pushed by processor
    myos::common::uint32_t eip;
    myos::common::uint32_t cs;
    myos::common::uint32_t eflags;
    myos::common::uint32_t esp;
    myos::common::uint32_t ess;
} __attribute__((packed));

class Task
{
    friend class TaskManager;
private:
    myos::common::uint8_t _stack[4096];
    CPUState* _cpustate;
public:
    Task( myos::GlobalDescriptorTable *gdt, void entrypoint() );
    ~Task();
};

class TaskManager
{
private:
    Task* _tasks[256];
    int _numTasks;
    int _currentTask;
public:
    TaskManager();
    ~TaskManager();
    bool addTask( Task * task );
    CPUState* schedule( CPUState* cpustate );
};
}

#endif // __MYOS__MULTITASKING_H
