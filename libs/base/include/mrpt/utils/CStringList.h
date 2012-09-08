/* +---------------------------------------------------------------------------+
   |                 The Mobile Robot Programming Toolkit (MRPT)               |
   |                                                                           |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2012, Individual contributors, see AUTHORS file        |
   | Copyright (c) 2005-2012, MAPIR group, University of Malaga                |
   | Copyright (c) 2012, University of Almeria                                 |
   | All rights reserved.                                                      |
   |                                                                           |
   | Redistribution and use in source and binary forms, with or without        |
   | modification, are permitted provided that the following conditions are    |
   | met:                                                                      |
   |    * Redistributions of source code must retain the above copyright       |
   |      notice, this list of conditions and the following disclaimer.        |
   |    * Redistributions in binary form must reproduce the above copyright    |
   |      notice, this list of conditions and the following disclaimer in the  |
   |      documentation and/or other materials provided with the distribution. |
   |    * Neither the name of the copyright holders nor the                    |
   |      names of its contributors may be used to endorse or promote products |
   |      derived from this software without specific prior written permission.|
   |                                                                           |
   | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       |
   | 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED |
   | TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR|
   | PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE |
   | FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL|
   | DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR|
   |  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)       |
   | HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,       |
   | STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN  |
   | ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           |
   | POSSIBILITY OF SUCH DAMAGE.                                               |
   +---------------------------------------------------------------------------+ */
#ifndef CStringList_H
#define CStringList_H

#include <mrpt/utils/utils_defs.h>
#include <mrpt/utils/CSerializable.h>

namespace mrpt
{
	namespace utils
	{
		// This must be added to any CSerializable derived class:
		DEFINE_SERIALIZABLE_PRE_CUSTOM_BASE( CStringList, mrpt::utils::CSerializable )

		/** A class for storing a list of text lines.
		  *  This class is optimized for managing strings on a "per-line" basis, although methods are also provided to obtain/set the
		  *   whole string list as a single, multi-line string.
		  *  There are methods for saving and loading to/from text files.
		  *  You can access to lines directly by "CStringList::get" or through the operator "CStringList::operator ()". The later can be
		  *   used both, to read and to write elements.
		  *  Also methods are provided for accesing the text by key if they are formated as "key=value" lines.
		 * \ingroup mrpt_base_grp
		 */
		class BASE_IMPEXP CStringList : public mrpt::utils::CSerializable
		{
			// This must be added to any CSerializable derived class:
			DEFINE_SERIALIZABLE( CStringList )

		protected:
			/** The internal list of strings
			  */
			std::deque<std::string>		m_strings;

		public:
			/** Default constructor (empty string list)
			  */
			CStringList();

			/** Constructor from a text
			  */
			CStringList(const std::string& text);

			/** Explicit constructor from deque<string> */
			explicit CStringList(const std::deque<std::string>& lines) : m_strings(lines) { }

			/** Explicit constructor from vector<string> */
			explicit CStringList(const std::vector<std::string>& lines)
			{
				std::copy(lines.begin(),lines.end(),std::back_inserter(m_strings));
			}

			/** Appends a new string at the end of the string list.
			  * \sa insert,set
			  */
			void  add( const std::string &str );

			/** An alternative way of adding strings to the list */
			CStringList & operator << (const std::string &s) { add(s); return *this; }

			/** Inserts a new item at a given position (0=insert at the beggining,1=put into the second position,...)
			  * \sa add,set
			  */
			void  insert( size_t index, const std::string &str );

			/** Overwrites an existing position with a new value (0=first elements)
			  * \sa insert
			  */
			void  set( size_t index, const std::string &str );

			/** Clear the whole list
			  */
			void  clear();

			/** Returns the number of text lines in the list
			  */
			size_t  size() const;

			/** Delete the element at a given position (0=first element)
			  */
			void  remove(size_t index);

			/** Looks for a given string in the list, and returns its index, or returns "false" otherwise.
			  * \return true if string has been found.
			  */
			bool  find(
				const std::string		&compareText,
				size_t					foundIndex,
				bool					caseSensitive = true) const;

			/** Returns one string from the line list
			  */
			void  get(size_t index, std::string &outText) const;

			/** Returns one string from the line list
			  */
			std::string  operator ()(size_t index) const;

			/** Returns a reference to one string from the line list
			  */
			std::string&  operator ()(size_t index);

			/** Returns the whole string list as a single string with '\r\n' characters for newlines.
			  */
			void  getText(std::string &outText) const;

			/** Returns the whole string list as a single string with '\r\n' characters for newlines.
			  */
			inline std::string getText() const
			{
				std::string s;
				getText(s);
				return s;
			}

			/** Fills the string list by parsing a single string with '\r', '\n', or '\r\n' characters indicatng newlines.
			  */
			void  setText(const std::string &inText);

			/** Load the string list from a file.
			  */
			void  loadFromFile(const std::string &fileName);

			/** Save the string list to a file.
			  */
			void  saveToFile(const std::string &fileName) const;

			/** Returns the value of the given key ("key=value").
			  * \exception std::exception If the key is not found in the string list.
			  */
			std::string  get_string( const std::string &keyName );

			/** Returns the value of the given key ("key=value").
			  * \exception std::exception If the key is not found in the string list.
			  */
			float  get_float( const std::string &keyName );

			/** Returns the value of the given key ("key=value").
			  * \exception std::exception If the key is not found in the string list.
			  */
			int  get_int( const std::string &keyName );

			/** Returns the value of the given key ("key=value").
			  * \exception std::exception If the key is not found in the string list.
			  */
			double  get_double( const std::string &keyName );

			/** Returns the value of the given key ("key=value").
			  * \exception std::exception If the key is not found in the string list.
			  */
			bool  get_bool( const std::string &keyName );

			/** Sets the value of a given key ("key=value"), overwritten previous value if it existed.
			  */
			void  set( const std::string &keyName, const std::string &value );

			/** Sets the value of a given key ("key=value"), overwritten previous value if it existed.
			  */
			void  set( const std::string &keyName, const int &value );

			/** Sets the value of a given key ("key=value"), overwritten previous value if it existed.
			  */
			void  set( const std::string &keyName, const float &value );

			/** Sets the value of a given key ("key=value"), overwritten previous value if it existed.
			  */
			void  set( const std::string &keyName, const double &value );

			/** Sets the value of a given key ("key=value"), overwritten previous value if it existed.
			  */
			void  set( const std::string &keyName, const bool &value );

		};

	} // End of namespace
} // End of namespace

#endif
