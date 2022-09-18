#ifndef __MYOS__MEMORYMANAGEMENT_H
#define __MYOS__MEMORYMANAGEMENT_H

#include <common/types.h>

namespace myos
{

struct MemoryChunk
{
	MemoryChunk* next;
	MemoryChunk* prev;
	bool allocated;
	myos::common::size_t size;
};

class MemoryManager
{
protected:
	MemoryChunk* _first;
public:
	static MemoryManager* _activeMemoryManager;

	MemoryManager( myos::common::size_t first, myos::common::size_t size );
	~MemoryManager();

	void* malloc( myos::common::size_t size );
	void free( void* ptr );
};

}

void* operator new( unsigned size );

void* operator new[]( unsigned size );

void operator delete( void* ptr );

void operator delete[]( void* ptr );

#endif // __MYOS__MEMORYMANAGEMENT_H
