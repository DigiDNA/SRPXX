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

#include <SRPXX/BigNum.hpp>
#include <SRPXX/String.hpp>
#include <algorithm>
#include <exception>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#endif
#include <openssl/ssl.h>
#include <openssl/bn.h>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

namespace SRP
{
    class BigNum::IMPL
    {
        public:
            
            IMPL( BIGNUM * bn );
            IMPL( uint64_t value );
            IMPL( const BigNum & o );
            ~IMPL();
            
            BIGNUM * _bn;
    };
    
    std::optional< BigNum > BigNum::fromString( const std::string & value, StringFormat format )
    {
        BigNum n;
        int    r = 0;
        
        switch( format )
        {
            case StringFormat::Auto:        r = BN_asc2bn( &( n.impl->_bn ), value.c_str() ); break;
            case StringFormat::Decimal:     r = BN_dec2bn( &( n.impl->_bn ), value.c_str() ); break;
            case StringFormat::Hexadecimal: r = BN_hex2bn( &( n.impl->_bn ), value.c_str() ); break;
        }
        
        if( r != 0 )
        {
            return n;
        }
        
        return {};
    }
    
    BigNum::BigNum():
        BigNum( 0 )
    {}
    
    BigNum::BigNum( uint64_t value ):
        impl( std::make_unique< IMPL >( value ) )
    {}
    
    BigNum::BigNum( const BigNum & o ):
        impl( std::make_unique< IMPL >( o ) )
    {}
    
    BigNum::BigNum( BigNum && o ) noexcept:
        impl( std::move( o.impl ) )
    {}
    
    BigNum::~BigNum()
    {}
    
    BigNum & BigNum::operator =( BigNum o )
    {
        swap( *( this ), o );
        
        return *( this );
    }
    
    BigNum & BigNum::operator =( uint64_t value )
    {
        return *( this ) = BigNum( value );
    }
    
    bool BigNum::operator ==( const BigNum & o ) const
    {
        return BN_cmp( this->impl->_bn, o.impl->_bn ) == 0;
    }
    
    bool BigNum::operator ==( uint64_t value ) const
    {
        return *( this ) == BigNum( value );
    }
    
    bool BigNum::operator ==( const std::string & value ) const
    {
        if( value.length() == 0 )
        {
            return false;
        }
        
        StringFormat format = StringFormat::Decimal;
        
        if
        (
               String::hasPrefix( value, "-0x" )
            || String::hasPrefix( value, "-0X" )
            || String::hasPrefix( value, "0x" )
            || String::hasPrefix( value, "0X" ) )
        {
            format = StringFormat::Hexadecimal;
        }
        
        std::string s1 = this->toString( format );
        std::string s2 = String::toLower( value );
        
        return s1 == s2;
    }
    
    bool BigNum::operator !=( const BigNum & o ) const
    {
        return !( *( this ) == o );
    }
    
    bool BigNum::operator !=( uint64_t value ) const
    {
        return !( *( this ) == value );
    }
    
    bool BigNum::operator !=( const std::string & value ) const
    {
        return !( *( this ) == value );
    }
    
    std::string BigNum::toString( StringFormat format ) const
    {
        BigNum abs = this->positive();
        char * cp  = nullptr;
        
        switch( format )
        {
            case StringFormat::Auto:        cp = BN_bn2dec( abs.impl->_bn ); break;
            case StringFormat::Decimal:     cp = BN_bn2dec( abs.impl->_bn ); break;
            case StringFormat::Hexadecimal: cp = BN_bn2hex( abs.impl->_bn ); break;
        }
        
        if( cp == nullptr )
        {
            return {};
        }
        
        std::string s( cp );
        
        OPENSSL_free( cp );
        
        if( format == StringFormat::Hexadecimal && this->isNegative() )
        {
            s = "-0x" + s;
        }
        else if( format == StringFormat::Hexadecimal )
        {
            s = "0x" + s;
        }
        else if( this->isNegative() )
        {
            s = "-" + s;
        }
        
        return String::toLower( s );
    }
    
    BigNum BigNum::negative() const
    {
        BigNum n = *( this );
        
        if( BN_is_negative( n.impl->_bn ) == false )
        {
            BN_set_negative( n.impl->_bn, 1 );
        }
        
        return n;
    }
    
    BigNum BigNum::positive() const
    {
        BigNum n = *( this );
        
        if( BN_is_negative( n.impl->_bn ) )
        {
            BN_set_negative( n.impl->_bn, 0 );
        }
        
        return n;
    }
            
    bool BigNum::isNegative() const
    {
        return BN_is_negative( this->impl->_bn ) == 1;
    }
    
    bool BigNum::isPositive() const
    {
        return BN_is_negative( this->impl->_bn ) == 0;
    }
    
    bool BigNum::isOdd() const
    {
        return BN_is_odd( this->impl->_bn ) == 1;
    }
    
    bool BigNum::isEven() const
    {
        return BN_is_odd( this->impl->_bn ) == 0;
    }
    
    void swap( BigNum & o1, BigNum & o2 ) noexcept
    {
        using std::swap;
        
        swap( o1.impl, o2.impl );
    }
    
    BigNum::IMPL::IMPL( BIGNUM * bn ):
        _bn( bn )
    {}
    
    BigNum::IMPL::IMPL( uint64_t value ):
        IMPL( BN_new() )
    {
        BN_set_u64( this->_bn, value );
    }
    
    BigNum::IMPL::IMPL( const BigNum & o ):
        IMPL( BN_dup( o.impl->_bn ) )
    {}
    
    BigNum::IMPL::~IMPL()
    {
        BN_free( this->_bn );
    }
}