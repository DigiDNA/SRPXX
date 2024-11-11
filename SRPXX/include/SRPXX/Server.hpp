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

#ifndef SRPXX_SERVER_HPP
#define SRPXX_SERVER_HPP

#include <SRPXX/Base.hpp>
#include <SRPXX/BigNum.hpp>
#include <memory>
#include <string>

namespace SRP
{
    class Server: public Base
    {
        public:
            
            Server( const std::string & identity, HashAlgorithm hashAlgorithm, GroupType groupType );
            Server( const std::string & identity, HashAlgorithm hashAlgorithm, GroupType groupType, const BigNum & b );
            virtual ~Server() override;
            
            Server( const Server & o )              = delete;
            Server & operator =( const Server & o ) = delete;
            
            void setV( const BigNum & value );
            void setA( const BigNum & value );
            
            BigNum A() const override;
            BigNum B() const override;
            BigNum S() const override;
            
            BigNum v() const;
            BigNum b() const;
            
        private:
            
            class IMPL;
            
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* SRPXX_SERVER_HPP */
