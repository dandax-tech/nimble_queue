# nimble_queue

*Fast lockless queue in C++*

Example:

	const max_queue_size = 10000;
	nimble::queue<my_struct> my_queue(max_queue_size);

	...

	writer() {
		my_struct data = {...};
		my_queue.try_push(data);

	...

	reader() {
		my_struct data;
		my_queue.try_pop(data);

NOTE:
* The queue does not support sleeping, only polling.
* The queue does not resize.
* Must run on 64 bit CPU.

This is a very fast implementation of a lockless queue of any copiable type T.
The implemtation uses atomic integers to keep track of the head and tail pointers.
It also allows writers to write and forget (if there is space in the queue),
eliminating writer lock contention.

_Implementation_

The memory is preallocated on construction, and never resized. While this may
waste some space, this eliminates any need for locking to resize.

The head and tail pointers are kept track of using 64 bit atomics always increase
removing the need to lock them on update. 

The bitset to keep track of "in flight" queue elements is striped in memory,
such that adjacent elements do not share a cacheline to it.

_Restrictions_
* Only one element type per queue
* No way to resize
* Must poll
* CPU must support 64 bit atomics

_TODO_
* Sleeping or polling when queue is drained
* Calculate optimal bitset stripe (possibly on the fly for current CPU)
* Possibly allow for queue to resize
* Possibly allow for variable sized elements







