Documentation Author: Niko Procopi 2020

This tutorial was designed for Visual Studio 2019
If the solution does not compile, retarget the solution
to a different version of the Windows SDK. If you do not
have any version of the Windows SDK, it can be installed
from the Visual Studio Installer Tool

AtlasEngine - Pools Part1
Prerequisites
	Intro C tutorials, Basic OpenGL tutorials, and 
	physics tutorials are required before starting
	the engine series

Pools are used to temporarily allocate
and erase small amounts of data, without
worrying about bookmarks or other data 
getting erased that we need.
	
With a stack allocator in a full game like Crash 
Team Racing, what happens if Player 1 drops a bomb
on the track, then Player 2 drops a bomb on the track,
and then Player 1's bomb explodes? Now, Player 1's 
bomb can not be deallocated without also erasing 
Player 2's bomb.

InstancePool and ThreadPool were added in this tutorial.
This way, we have a block of 10 instances, and a block
of 10 threads, then we can make more allocations in 
Mempack, then we can allocate/deallocate members of 
the pool without touching Mempack

Pool.h shows a structure Item, which we use for casting
any kind of object that we put in the pool. Instance.h
and Thread.h also now have "prev" and "next" added.

We use prev and next to make linked lists in the pool
for all "free" items and all "taken" items. This way 
we can allocate and deallocate any Item, anywhere in 
the pool, just by touching the first members of 
linkedList_FreeItems and linkedList_TakenItems

Pool_Init allocates an array of all Items in the pool,
keep in mind "Item" can be any size, set by Pool_Init's parameter.
To prepare the pool for allocations, we add all items to the
freeItems linked list. Pool_EraseItem and Pool_NewItem simply
swap an item from one linked list to the other.

Thread.cpp was changed to pull allocations from the pool,
instead of from Mempack. Main.cpp was changed to allocate
instances in the instance pool instead of from mempack.

Even better, now that all these objects are linked together,
in the game loop, we can loop through the pool to draw all
items and update all threads.

This means, any time we create an instance or create a thread,
the engine is smart enough to update it, without a programmer
needing to specify which ones to update manually.

Coming soon:

Next tutorial:
	Using pools to recursively check threads and 
their instances for collision (between player and coins),
then erasing elements and reallocating them.

Tutorial after that:
	Creating additional pools that can dynamically
handle any kind of data, so one pool can have multiple 
kinds of data. This is how Crash Team Racing can 
allocate and deallocate weapons