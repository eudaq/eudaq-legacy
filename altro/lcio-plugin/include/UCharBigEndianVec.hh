/*
 * UCharBigEndianVec.hh
 *
 *  Created on: Sep 30, 2009
 *      Author: caiazza
 */

#ifndef UCHARBIGENDIANVEC_HH_
#define UCHARBIGENDIANVEC_HH_

#include <cstdlib>
#include <vector>

namespace eudaq
{

/** A helper class to read the byte sequence of big endian 32 bit data as 10 bit, 32 bit or
 *  40 bit data.
 *  This class only contains a reference to the data, not a copy, to avoid copying of the data.
 */
class UCharBigEndianVec
{
	private:
    /** A reference to the data vector.
     *  Can only be accessed through the GetNNbitWord() functions.
     */
    std::vector<unsigned char> const & _bytedata;
    std::vector<unsigned char>::const_iterator _dataindex;

	protected:
    bool _altrowordsreversed;

	public:
    /** The constructor.
     *  It requires a reference of the actual data vector.
     */
    UCharBigEndianVec(std::vector<unsigned char> const & datavec, bool altrowordsreversed);

    /** Helper function to get a 40 bit word with correct endinanness out of the byte vector.
     *  The offset32bit is the position of the first 10 bit word within the 32bit stream
     */
    unsigned short Get10bitWord(unsigned int index10bit, unsigned int n40bitwords,
				unsigned int offset32bit) const;

    /** Helper function to get a 32 bit word with correct endinanness out of the byte vector.
     */
    unsigned int Get32bitWord(unsigned int index32bit) const;

    /** Helper function to get a 40 bit word with correct endinanness out of the byte vector.
     *  The offset32bit is the position of the first 40 bit word within the 32bit stream
     */
    unsigned long long int Get40bitWord(unsigned int index40bit, unsigned int n40bitwords,
					unsigned int offset32bit) const;

    /// size in 32 bit words
    size_t Size(){ return _bytedata.size() / 4 ; }

    std::vector<unsigned char>::const_iterator PointDataWord(unsigned int index32bit);
    std::vector<unsigned char>::const_iterator end();
};

}


#endif /* UCHARBIGENDIANVEC_HH_ */
