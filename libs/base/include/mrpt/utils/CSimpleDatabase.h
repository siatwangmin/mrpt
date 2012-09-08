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
#ifndef CSimpleDatabase_H
#define CSimpleDatabase_H

#include <mrpt/utils/utils_defs.h>
#include <mrpt/utils/CSerializable.h>

namespace mrpt
{
namespace utils
{

	// This must be added to any CSerializable derived class:
	DEFINE_SERIALIZABLE_PRE_CUSTOM_BASE( CSimpleDatabase, mrpt::utils::CSerializable )
	// This must be added to any CSerializable derived class:
	DEFINE_SERIALIZABLE_PRE_CUSTOM_BASE( CSimpleDatabaseTable, mrpt::utils::CSerializable )

/**  This class implements the tables of databases.
 * \sa CSimpleDatabase \ingroup mrpt_base_grp
 */
class BASE_IMPEXP CSimpleDatabaseTable : public mrpt::utils::CSerializable
{
	// This must be added to any CSerializable derived class:
	DEFINE_SERIALIZABLE( CSimpleDatabaseTable )
public:
	/** Default constructor
	  */
	CSimpleDatabaseTable( );

	/** Destructor
	  */
	virtual ~CSimpleDatabaseTable();

	/** Get the count of fields.
	  */
	size_t fieldsCount() const;

	/** Append a new and empty record at the end of the table, and return the index of the newly added record.
	  * \sa deleteRecord
	  */
	size_t appendRecord();

	/** Add a new field to the table. The table is cleared in this operation.  */
	void  addField(const char *fieldName);

	/** Add a new field to the table. The table is cleared in this operation.  */
	void  addField(const std::string &fieldName) {
		addField(fieldName.c_str());
	}

	/** Get the name of a field by its index
	  * \exception std::exception On index out of bounds
	  */
	std::string  getFieldName(size_t fieldIndex) const;

	/** Get the index for a given field name
	  * \exception std::exception On field not found
	  */
	size_t fieldIndex(const char *fieldName) const;
	
	/** Get the index for a given field name
	  * \exception std::exception On field not found
	  */
	size_t fieldIndex(const std::string &fieldName) const {
		return fieldIndex(fieldName.c_str());
	}

	/** Get the records count in the table
	  */
	size_t getRecordCount() const;

	/**  Returns the cell content of the record indicates by its index, and the field indicated in "field".
	  * \exception std::exception On field or record not found
	  */
	std::string  get(size_t recordIndex, std::string field) const;

	/**  Returns the cell content of the record indicates by its index, and the field indicated by its index.
	  * \exception std::exception On field or record not found
	  */
	std::string  get(size_t recordIndex, size_t fieldIndex) const;

	/**  Sets the cell content of the record indicates by its index, and the field indicated in "field".
	  * \exception std::exception On field or record not found
	  */
	void  set(size_t recordIndex, std::string field, std::string value);

	/**  Sets the cell content of the record indicates by its index, and the field indicated by its index.
	  * \exception std::exception On field or record not found
	  */
	void  set(size_t recordIndex, size_t fieldIndex, std::string value);

	/**  Executes a query in the table, returning the record index which a given field has a given value, case insensitive, or -1 if not found.
	  */
	int  query(std::string field, std::string value) const;

	/** Delete the record at the given index \sa appendRecord */
	void deleteRecord(size_t recordIndex);

private:
	vector_string				field_names;	//!< Field names
	std::vector<vector_string>	data;	//!< Data for each cell

}; // end of class definition

/**  This class impements a very simple database system. A database is
  *   a collection of tables, each one being a CSimpleDatabaseTable object. Tables are
  *   a rectangular arrrangement of cells, organized as records of fields.
  *  There are XML export/import methods in saveAsXML, loadFromXML.
  *
  * \note This class is NOT safe for read/write access from different threads. If needed, use critical sections.
  *
  * \sa CSimpleDatabaseTable
  */
class BASE_IMPEXP CSimpleDatabase  : public mrpt::utils::CSerializable
{
	// This must be added to any CSerializable derived class:
	DEFINE_SERIALIZABLE( CSimpleDatabase )

public:
	/** Default constructor
	  */
	CSimpleDatabase( );

	/** Destructor
	  */
	virtual ~CSimpleDatabase( );

	/** Clears the DB.
	  */
	void  clear();

	/** Creates a new table in the DB, initially empty.
	  */
	CSimpleDatabaseTablePtr  createTable(const std::string &name);

	/** Returns the table with the indicated name
	  * \exception std::exception On table not found.
	  */
	CSimpleDatabaseTablePtr getTable(const std::string &tableName);

	/** Deletes the given table.
	  * \exception std::exception On table not found.
	  */
	void dropTable(const std::string &tableName);

	/** Changes the name of a given table 
	  * \exception std::exception On table not found or new name already existed.
	  */
	void renameTable(
		const std::string &tableName,
		const std::string &newTableName );

	/** Returns the table by index.
	  * \exception std::exception On index out of bounds
	  */
	CSimpleDatabaseTablePtr getTable(size_t tableIndex);

	/** Returns the tables count in the DB.
	  */
	size_t	 tablesCount() const;

	/** Returns the tables names in the DB.
	  * \exception std::exception On index out of bounds
	  */
	std::string	 tablesName(size_t tableIndex) const;

	/** Saves this database as a XML file.
	  * \return false on any error, true if successful.
	  * \sa loadFromXML
	  */
	bool saveAsXML( const std::string &fileName ) const;

	/** Loads the content of this database from a a XML file.
	  * \return false on any error, true if successful.
	  * \sa saveAsXML
	  */
	bool loadFromXML( const std::string &fileName );


private:

	/** The tables of the DB indexed by their names: */
	typedef std::map<std::string, CSimpleDatabaseTablePtr>					TTableList;
	typedef std::map<std::string, CSimpleDatabaseTablePtr>::iterator		iterator;
	typedef std::map<std::string, CSimpleDatabaseTablePtr>::const_iterator	const_iterator;

	TTableList	m_tables;


}; // end of class definition


} // End of namespace
} // End of namespace


#endif
