#pragma once

#ifndef SID_RINGBUFFER_INCLUDED
#define SID_RINGBUFFER_INCLUDED

#include <cstddef>
#include <iostream>

// a flaky ring buffer implementation

template <typename T, size_t _size>
class RingBuffer {
private:
	T values[_size];
	size_t max_size = _size;
	size_t size = 0;
	size_t head = 0;
	size_t tail = 0;

public:
	RingBuffer() {
		clear();
	}

	void clear() {
		size = 0;
		head = 0;
		tail = 0;
	}

	bool empty() {
		return size == 0;
	}

	bool full() {
		return size == max_size;
	}

	void put(const T& elem) {
		values[tail] = elem;
		tail = (tail + 1) % max_size;

		if(tail == head) {
			size = max_size;
		} else {
			size = (tail - head + max_size) % max_size;
		}
	}

	T& pop_head() {
		T& elem = values[head];

		head = (head + 1) % max_size;

		size = (tail - head + max_size) % max_size;

		return elem;
	}

	T& pop_tail() {
		if(tail == 0) {
			tail = max_size - 1;
		} else {
			tail = tail - 1;
		}

		T& elem = values[tail];

		size = (tail - head + max_size) % max_size;

		return elem;
	}
};

#endif // SID_RINGBUFFER_INCLUDED
