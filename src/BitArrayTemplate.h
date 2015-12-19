/*
  BitArrayTemplate.h

  Copyright (c) 2015, Tobias Meyer

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef BitArrayTemplate_h
#define BitArrayTemplate_h
#include <stdlib.h>


/* Class that allows storing a number of bits in an internal array of bytes,
 * and allows access to the bits and bytes in a conveniant way.
 * This is not meant to be implemented in the most efficient way (though efficiency was kept in mind), 
 * but tries to minimize programming errors by clear design.
 * Will NOT do bounds checking!
 */
template <uint16_t MAX_BITS> class BitArrayTemplate 
{
	private:
	    uint8_t* _buffer;
	
	public:
		BitArrayTemplate();
		void set(const uint8_t &offset, bool value);
		void set(const uint8_t &offset, const int &num_bits, uint8_t value);
		void set(const uint8_t &offset, const int &num_bits, uint16_t value);
		void set(const uint8_t &offset, const int &num_bits, uint32_t value);
		
		bool getBit(const uint8_t &offset);
		uint8_t getByte(const uint8_t &offset, const int &num_bits);
		uint16_t getWord(const uint8_t &offset, const int &num_bits);
		uint32_t getDWord(const uint8_t &offset, const int &num_bits);
		uint8_t* getBuffer();
		uint16_t getBufferSize();
};

//================================================================================
//================================================================================
//	BitArrayTemplate

template <uint16_t MAX_BITS>
BitArrayTemplate<MAX_BITS>::BitArrayTemplate() {

	_buffer = (uint8_t*)malloc(((MAX_BITS+7) >> 3));  // note: >>3 equals /8
	for (int i=0; i < ((MAX_BITS+7) >> 3); i++) {
		_buffer[i]=0;
	}
}

template <uint16_t MAX_BITS>
void BitArrayTemplate<MAX_BITS>::set(const uint8_t &offset, bool value){
    uint8_t byte = offset/8;
	uint8_t bit = (1 << (offset % 8));
	if (value) {
		_buffer[byte] = _buffer[byte] | bit; // set bit
	} else {
		_buffer[byte] = _buffer[byte] & ~bit; // clear bit
	}
}
	
template <uint16_t MAX_BITS>
void BitArrayTemplate<MAX_BITS>::set(const uint8_t &offset, const int &num_bits, uint8_t value)
{
	for (int i=0; i <num_bits; i++) {
		set(offset+i, (value & 1));
		value = value >> 1;
	}
}

template <uint16_t MAX_BITS>
void BitArrayTemplate<MAX_BITS>::set(const uint8_t &offset, const int &num_bits, uint16_t value)
{
	if (num_bits>8) {
		set(offset  ,8          ,(uint8_t)(value&0xff));
		set(offset+8,num_bits-8 ,(uint8_t)(value>>8));
	} else {
		set(offset  ,num_bits   ,(uint8_t)(value&0xff));
	}	
}

template <uint16_t MAX_BITS>
void BitArrayTemplate<MAX_BITS>::set(const uint8_t &offset, const int &num_bits, uint32_t value)
{
	for (int i=num_bits; i>0; i--) {
		set(offset+i-1, (value & 1));
		value = value >> 1;
	}
}

template <uint16_t MAX_BITS>
bool BitArrayTemplate<MAX_BITS>::getBit(const uint8_t &offset)
{
    uint8_t byte = offset/8;
	uint8_t bit = (1 << (offset % 8));
	return _buffer[byte] & bit;
}

template <uint16_t MAX_BITS>
uint8_t BitArrayTemplate<MAX_BITS>::getByte(const uint8_t &offset, const int &num_bits)
{
	uint8_t result = 0;
	for (int i = 0 ; i <num_bits; i++) {
		result = (result <<1) | getBit(offset+i);
	}
	return result;
}

template <uint16_t MAX_BITS>
uint16_t BitArrayTemplate<MAX_BITS>::getWord(const uint8_t &offset, const int &num_bits)
{
	uint16_t result = 0;
	for (int i = 0 ; i <num_bits; i++) {
		result = (result <<1) | getBit(offset+i);
	}
	return result;
}

template <uint16_t MAX_BITS>
uint32_t BitArrayTemplate<MAX_BITS>::getDWord(const uint8_t &offset, const int &num_bits)
{
	uint32_t result = 0;
	for (int i = 0 ; i <num_bits; i++) {
		result = (result <<1) | getBit(offset+i);
	}
	return result;
}
template <uint16_t MAX_BITS>
uint8_t* BitArrayTemplate<MAX_BITS>::getBuffer() 
{
	return _buffer;
}
template <uint16_t MAX_BITS>
uint16_t BitArrayTemplate<MAX_BITS>::getBufferSize() 
{
	return ((MAX_BITS+7) >> 3);
}


#endif