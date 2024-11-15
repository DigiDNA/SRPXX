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

#ifndef SRPXX_CLIENT_HPP
#define SRPXX_CLIENT_HPP

#include <SRPXX/Base.hpp>
#include <SRPXX/BigNum.hpp>
#include <memory>
#include <string>
#include <cstdint>
#include <vector>

namespace SRP
{
    class Client: public Base
    {
        public:
        
            enum class Options: uint64_t
            {
                NoUsernameInX = 1 << 0
            };
            
            Client( const std::string & identity, HashAlgorithm hashAlgorithm, GroupType groupType );
            Client( const std::string & identity, HashAlgorithm hashAlgorithm, GroupType groupType, const BigNum & a );
            virtual ~Client() override;
            
            Client( const Client & o )              = delete;
            Client & operator =( const Client & o ) = delete;
            
            void setPassword( const std::string & value );
            void setPassword( const std::vector< uint8_t > & value );
            void setB( const BigNum & value );
            
            void setOptions( uint64_t options );
            void addOption( Options option );
            void removeOption( Options option );
            bool hasOption( Options option ) const;
            
            BigNum A() const override;
            BigNum B() const override;
            BigNum S() const override;
            
            BigNum a() const;
            BigNum x() const;
            BigNum v() const;
            
        private:
            
            class IMPL;
            
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* SRPXX_CLIENT_HPP */
