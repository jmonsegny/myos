#include <memorymanagement.h>
using namespace myos;
using namespace myos::common;

MemoryManager* MemoryManager::_activeMemoryManager = 0;

MemoryManager::
MemoryManager( size_t first, size_t size )
{
	_activeMemoryManager = this;

	if( size < sizeof(MemoryChunk))
		_first = 0;
	else {
		_first = (MemoryChunk*)first;
		_first->allocated = false;
		_first->prev = 0;
		_first->next = 0;
		_first->size = size - sizeof(MemoryChunk); 
	}
}

MemoryManager::
~MemoryManager()
{
}

void* MemoryManager::
malloc( myos::common::size_t size )
{
	MemoryChunk *result = 0;
	
	for( MemoryChunk* chunk = _first; chunk != 0; chunk = chunk->next ) {
		if( chunk->size > size && !chunk->allocated ) {
			result = chunk;
			break;
		}
	}

	if( result == 0)
		return 0;

	if( result->size < size + sizeof(MemoryChunk) + 1 ) {
		// Just enough space for the chunk
		result->allocated = true;
	} else {
		// We are going to chop the chunk	
		MemoryChunk* temp = (MemoryChunk*)((size_t)result + sizeof(MemoryChunk) + size);
		temp->allocated = false;
		temp->size = result->size - size - sizeof(MemoryChunk);
		temp->next = result->next;
		if( temp->next != 0 )
			temp->next->prev = temp;
		result->size = size;
		result->next = temp;
		result->allocated = true;
	}
	// Return address just after the memry chunk header.
	return (void*)(((size_t)result) + sizeof(MemoryChunk));
}

void MemoryManager::
free( void* ptr )
{
	MemoryChunk* chunk = (MemoryChunk*)((size_t)ptr - sizeof(MemoryChunk));
	chunk->allocated = false;
	
	// Merges unallocated adjacent chunks
	// Previous
	if( chunk->prev != 0 && !chunk->prev->allocated ) {
		chunk->prev->next = chunk->next;
		chunk->prev->size += chunk->size + sizeof(MemoryChunk);
		if( chunk->next != 0 )
			chunk->next->prev = chunk->prev;

		chunk = chunk->prev;
	}		

	// Next
	if( chunk->next !=0 && !chunk->next->allocated ) {
		chunk->size += chunk->next->size + sizeof(MemoryChunk);
		chunk->next = chunk->next->next;
		if( chunk->next != 0 )
			chunk->next->prev = chunk;
	}
}

void* operator new( unsigned size )
{
    if( myos::MemoryManager::_activeMemoryManager == 0 )
        return 0;
    return myos::MemoryManager::_activeMemoryManager->malloc(size);
}

void* operator new[]( unsigned size )
{
    if( myos::MemoryManager::_activeMemoryManager == 0 )
        return 0;
    return myos::MemoryManager::_activeMemoryManager->malloc(size);
}

void operator delete( void* ptr )
{
    if( myos::MemoryManager::_activeMemoryManager != 0 )
        myos::MemoryManager::_activeMemoryManager->free( ptr );
}

void operator delete[]( void* ptr )
{
    if( myos::MemoryManager::_activeMemoryManager != 0 )
        myos::MemoryManager::_activeMemoryManager->free( ptr );
}

