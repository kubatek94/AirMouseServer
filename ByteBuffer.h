/**
 ByteBuffer
 ByteBuffer.h
 Copyright 2011 - 2013 Ramsey Kant

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 Modfied 2015 by Ashley Davis (SgtCoDFish)
 
 Modified 2016 by kubatek94 (change to generic template class)
 */

#ifndef _ByteBuffer_H_
#define _ByteBuffer_H_

// Default number of uint8_ts to allocate in the backing buffer if no size is provided
#define BB_DEFAULT_SIZE 5

// If defined, utility functions within the class are enabled
#define BB_UTILITY

// If defined, places the class into a namespace called bb
#define BB_USE_NS

#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <vector>
#include <memory>

#ifdef BB_UTILITY
#include <iostream>
#include <cstdio>
#endif

#ifdef BB_USE_NS
namespace bb {
#endif

template <class E>
class ByteBuffer {
public:
    
 /**
 * ByteBuffer constructor
 * Reserves specified size in internal vector
 *
 * @param size Size (in bytes) of space to preallocate internally. Default is set in DEFAULT_SIZE
 */
	ByteBuffer(uint32_t size = BB_DEFAULT_SIZE)  {
            buf.reserve(size);
            clear();
#ifdef BB_UTILITY
            name = "";
#endif
}
        
/**
 * ByteBuffer constructor
 * Consume an entire uint8_t array of length len in the ByteBuffer
 *
 * @param arr uint8_t array of data (should be of length len)
 * @param size Size of space to allocate
 */
ByteBuffer(E* arr, uint32_t size) {
	// If the provided array is NULL, allocate a blank buffer of the provided size
	if (arr == NULL) {
		buf.reserve(size);
		clear();
	} else { // Consume the provided array
		buf.reserve(size);
		clear();
		putBytes(arr, size);
	}

#ifdef BB_UTILITY
	name = "";
#endif
}
        
	~ByteBuffer() = default;

/**
 * Bytes Remaining
 * Returns the number of bytes from the current read position till the end of the buffer
 *
 * @return Number of bytes from rpos to the end (size())
 */
uint32_t bytesRemaining() {
	return size() - rpos;
}

/**
 * Clear
 * Clears out all data from the internal vector (original preallocated size remains), resets the positions to 0
 */
void clear() {
	rpos = 0;
	wpos = 0;
	buf.clear();
}

/**
 * Equals, test for data equivilancy
 * Compare this ByteBuffer to another by looking at each byte in the internal buffers and making sure they are the same
 *
 * @param other A pointer to a ByteBuffer to compare to this one
 * @return True if the internal buffers match. False if otherwise
 */
bool equals(ByteBuffer<E>* other) {
	// If sizes aren't equal, they can't be equal
	if (size() != other->size())
		return false;

	// Compare byte by byte
	uint32_t len = size();
	for (uint32_t i = 0; i < len; i++) {
		if ((E) get(i) != (E) other->get(i))
			return false;
	}

	return true;
}

/**
 * Resize
 * Reallocates memory for the internal buffer of size newSize. Read and write positions will also be reset
 *
 * @param newSize The amount of memory to allocate
 */
void resize(uint32_t newSize) {
	buf.resize(newSize);
	rpos = 0;
	wpos = 0;
}


/**
 * Size
 * Returns the size of the internal buffer...not necessarily the length of bytes used as data!
 *
 * @return size of the internal buffer
 */
uint32_t size() {
	return buf.size();
}


	// Basic Searching (Linear)
	template<typename T> int32_t find(T key, uint32_t start = 0) {
		int32_t ret = -1;
		uint32_t len = buf.size();
		for (uint32_t i = start; i < len; i++) {
			T data = read<T>(i);
			// Wasn't actually found, bounds of buffer were exceeded
			if ((key != 0) && (data == 0))
				break;

			// Key was found in array
			if (data == key) {
				ret = (int32_t) i;
				break;
			}
		}
		return ret;
	}

/**
 * Replace
 * Replace occurance of a particular uint8_t, key, with the uint8_t rep
 *
 * @param key uint8_t to find for replacement
 * @param rep uint8_t to replace the found key with
 * @param start Index to start from. By default, start is 0
 * @param firstOccuranceOnly If true, only replace the first occurance of the key. If false, replace all occurances. False by default
 */

void replace(E key, E rep, uint32_t start, bool firstOccuranceOnly) {
	uint32_t len = buf.size();
	for (uint32_t i = start; i < len; i++) {
		E data = read<E>(i);
		// Wasn't actually found, bounds of buffer were exceeded
		if ((key != 0) && (data == 0))
			break;

		// Key was found in array, perform replacement
		if (data == key) {
			buf[i] = rep;
			if (firstOccuranceOnly)
				return;
		}
	}
}

// Read Functions

E peek() const {
	return read<E>(rpos);
}


E get() const {
	return read<E>();
}


E get(uint32_t index) const {
	return read<E>(index);
}


void getBytes(E* buf, uint32_t len) const {
	for (uint32_t i = 0; i < len; i++) {
		buf[i] = read<E>();
	}
}


char getChar() const {
	return read<char>();
}


char getChar(uint32_t index) const {
	return read<char>(index);
}


double getDouble() const {
	return read<double>();
}


double getDouble(uint32_t index) const {
	return read<double>(index);
}


float getFloat() const {
	return read<float>();
}


float getFloat(uint32_t index) const {
	return read<float>(index);
}


uint32_t getInt() const {
	return read<uint32_t>();
}


uint32_t getInt(uint32_t index) const {
	return read<uint32_t>(index);
}


uint64_t getLong() const {
	return read<uint64_t>();
}


uint64_t getLong(uint32_t index) const {
	return read<uint64_t>(index);
}


uint16_t getShort() const {
	return read<uint16_t>();
}


uint16_t getShort(uint32_t index) const {
	return read<uint16_t>(index);
}

// Write Functions

void put(ByteBuffer* src) {
	uint32_t len = src->size();
	for (uint32_t i = 0; i < len; i++)
		append<E>(src->get(i));
}


void put(E b) {
	append<E>(b);
}


void put(E b, uint32_t index) {
	insert<E>(b, index);
}


void putBytes(E* b, uint32_t len) {
	// Insert the data one byte at a time into the internal buffer at position i+starting index
	for (uint32_t i = 0; i < len; i++)
		append<E>(b[i]);
}


void putBytes(E* b, uint32_t len, uint32_t index) {
	wpos = index;

	// Insert the data one byte at a time into the internal buffer at position i+starting index
	for (uint32_t i = 0; i < len; i++)
		append<E>(b[i]);
}


void putChar(char value) {
	append<char>(value);
}


void putChar(char value, uint32_t index) {
	insert<char>(value, index);
}


void putDouble(double value) {
	append<double>(value);
}


void putDouble(double value, uint32_t index) {
	insert<double>(value, index);
}


void putFloat(float value) {
	append<float>(value);
}


void putFloat(float value, uint32_t index) {
	insert<float>(value, index);
}


void putInt(uint32_t value) {
	append<uint32_t>(value);
}


void putInt(uint32_t value, uint32_t index) {
	insert<uint32_t>(value, index);
}


void putLong(uint64_t value) {
	append<uint64_t>(value);
}


void putLong(uint64_t value, uint32_t index) {
	insert<uint64_t>(value, index);
}


void putShort(uint16_t value) {
	append<uint16_t>(value);
}


void putShort(uint16_t value, uint32_t index) {
	insert<uint16_t>(value, index);
}

        // Access to internal vector
        std::vector<E>& getBuffer() {
            return buf;
        }
        
	// Buffer Position Accessors & Mutators
	void setReadPos(uint32_t r) {
		rpos = r;
	}

	uint32_t getReadPos() const {
		return rpos;
	}

	void setWritePos(uint32_t w) {
		wpos = w;
	}

	uint32_t getWritePos() const {
		return wpos;
	}

	// Utility Functions
#ifdef BB_UTILITY
void setName(std::string n) {
	name = n;
}


std::string getName() {
	return name;
}


void printInfo() {
	uint32_t length = buf.size();
	std::cout << "ByteBuffer " << name.c_str() << " Length: " << length << ". Info Print" << std::endl;
}


void printAH() {
	uint32_t length = buf.size();
	std::cout << "ByteBuffer " << name.c_str() << " Length: " << length << ". ASCII & Hex Print" << std::endl;

	for (uint32_t i = 0; i < length; i++) {
            E x = buf[i];
            std::printf("%s0x%02x ", x<0 ? "-" : "", x < 0 ? -(unsigned) x : x);
	}

	std::printf("\n");
	for (uint32_t i = 0; i < length; i++) {
		std::printf("%c ", buf[i]);
	}

	std::printf("\n");
}


void printAscii() {
	uint32_t length = buf.size();
	std::cout << "ByteBuffer " << name.c_str() << " Length: " << length << ". ASCII Print" << std::endl;

	for (uint32_t i = 0; i < length; i++) {
		std::printf("%c ", buf[i]);
	}

	std::printf("\n");
}


void printHex() {
	uint32_t length = buf.size();
	std::cout << "ByteBuffer " << name.c_str() << " Length: " << length << ". Hex Print" << std::endl;

	for (uint32_t i = 0; i < length; i++) {
            E x = buf[i];
            std::printf("%s0x%02x ", x<0 ? "-" : "", x < 0 ? -(unsigned) x : x);
	}

	std::printf("\n");
}


void printPosition() {
	uint32_t length = buf.size();
	std::cout << "ByteBuffer " << name.c_str() << " Length: " << length << " Read Pos: " << rpos << ". Write Pos: "
	        << wpos << std::endl;
}
#endif

private:
	uint32_t wpos;
	mutable uint32_t rpos;
	std::vector<E> buf;

#ifdef BB_UTILITY
	std::string name;
#endif

	template<typename T> T read() const {
		T data = read<T>(rpos);
		rpos += sizeof(T);
		return data;
	}

	template<typename T> T read(uint32_t index) const {
		if (index + sizeof(T) <= buf.size())
			return *((T*) &buf[index]);
		return 0;
	}

	template<typename T> void append(T data) {
		uint32_t s = sizeof(data);

		if (size() < (wpos + s))
			buf.resize(wpos + s);
		memcpy(&buf[wpos], (E*) &data, s);
		//printf("writing %c to %i\n", (uint8_t)data, wpos);

		wpos += s;
	}

	template<typename T> void insert(T data, uint32_t index) {
		if ((index + sizeof(data)) > size())
			return;

		memcpy(&buf[index], (E*) &data, sizeof(data));
		wpos = index + sizeof(data);
	}
};

#ifdef BB_USE_NS
}
#endif

#endif