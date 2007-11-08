/* Copyright (C) 2007 Jan Kundrát <jkt@gentoo.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef IMAP_EXCEPTIONS_H
#define IMAP_EXCEPTIONS_H
#include <exception>

/**
 * @file
 * Common IMAP-related exceptions
 *
 * @author Jan Kundrát <jkt@gentoo.org>
 */

/** Namespace for IMAP interaction */
namespace Imap {

    /** General exception class */
    class Exception : public std::exception {
        /** The error message */
        std::string _msg;
    public:
        Exception( const std::string& msg ) : _msg( msg ) {};
        virtual const char* what() const throw() { return _msg.c_str(); };
        virtual ~Exception() throw() {};
    };

    /** Invalid argument was passed to some function */
    class InvalidArgumentException: public Exception {
    public:
        InvalidArgumentException( const std::string& msg ) : Exception( msg ) {};
    };

    /** Socket error */
    class SocketException : public Exception {
    public:
        SocketException( const std::string& msg ) : Exception( msg ) {};
    };

}
#endif /* IMAP_EXCEPTIONS_H */
