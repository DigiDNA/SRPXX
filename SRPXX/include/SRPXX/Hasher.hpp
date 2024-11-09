/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2024 Jean-David Gadina - www.xs-labs.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#ifndef SRPXX_HASHER_HPP
#define SRPXX_HASHER_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <SRPXX/String.hpp>

namespace SRP
{
    class Hasher
    {
        public:
            
            virtual ~Hasher() = default;
            
            virtual bool update( const std::vector< uint8_t > & data ) = 0;
            virtual bool update( const uint8_t * data, size_t length ) = 0;
            virtual bool update( const std::string & data )            = 0;
            virtual bool finalize()                                    = 0;
            
            virtual std::vector< uint8_t > getBytes()                                                           = 0;
            virtual std::string            getString( String::HexFormat format = String::HexFormat::Uppercase ) = 0;
    };
}

#endif /* SRPXX_HASHER_HPP */
