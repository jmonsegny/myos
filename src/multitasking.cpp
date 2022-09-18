#include <multitasking.h>
using namespace myos::common;
using namespace myos;

Task::
Task( GlobalDescriptorTable *gdt, void entrypoint() )
{
    _cpustate = (CPUState*)(_stack + 4096 - sizeof(CPUState));

    _cpustate->eax = 0;
    _cpustate->ebx = 0;
    _cpustate->ecx = 0;
    _cpustate->edx = 0;

    _cpustate->esi = 0;
    _cpustate->edi = 0;
    _cpustate->ebp = 0;
    /*
        _cpustate->gs  = 0;
        _cpustate->fs  = 0;
        _cpustate->es  = 0;
        _cpustate->ds  = 0;
    */
//    _cpustate->error = 0;

//    _cpustate->esp = 0;
    _cpustate->eip = (uint32_t)entrypoint;
    _cpustate->cs  = gdt->codeSegmentSelector();
//    _cpustate->ss  = 0;
    _cpustate->eflags = 0x202;
}

Task::
~Task()
{
}

TaskManager::
TaskManager()
    :_numTasks(0), _currentTask(-1)
{
}

TaskManager::
~TaskManager()
{
}

bool TaskManager::
addTask( Task * task )
{
    if( _numTasks >= 256 )
        return false;
    _tasks[_numTasks++] = task;
    return true;
}

CPUState* TaskManager::
schedule( CPUState* cpustate )
{
    if( _numTasks <= 0 )
        return cpustate;

    if( _currentTask >= 0 )
        _tasks[_currentTask]->_cpustate = cpustate;

    _currentTask++;
    if( _currentTask >= _numTasks )
        _currentTask  %= _numTasks;

    return _tasks[_currentTask]->_cpustate;
}
