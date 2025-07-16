#include "binaryinteger.h"

using namespace std;

size_t BinaryInteger::number_of_blocks = 0;

void BinaryInteger::set_number_of_blocks(size_t number_of_bits)     // Static
{
    number_of_blocks = (number_of_bits + 63)/64;
}
    
BinaryInteger::BinaryInteger() 
    : blocks(number_of_blocks, 0)
{

}
   
void BinaryInteger::set_bit(int i)
{
    blocks[i/64] |= uint64_t(1) << (i % 64);
}
   
BinaryInteger& BinaryInteger::operator|=(const BinaryInteger& other)
{
    for(size_t i = 0; i < blocks.size(); ++i)
    {
        blocks[i] |= other.blocks[i];
    }
    return *this;
}
   
bool BinaryInteger::operator==(const BinaryInteger& other) const
{
    return blocks == other.blocks;
}
   
BinaryInteger BinaryInteger::operator|(const BinaryInteger& other) const
{
    BinaryInteger result = *this;
    result |= other;
    return result;
}

bool BinaryInteger::is_subset_of(const BinaryInteger& other) const
{
    for(size_t i = 0; i < blocks.size(); ++i)
    {
        if((blocks[i] | other.blocks[i]) != other.blocks[i])
            return false;
    }
    return true;
}