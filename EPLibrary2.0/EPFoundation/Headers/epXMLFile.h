/*! 
@file epXMLFile.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date October 01, 2011
@brief XML File Interface
@version 2.0

@section LICENSE

Copyright (C) 2012  Woong Gyu La <juhgiyo@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

@section DESCRIPTION

An Interface for the XML File Class.

*/
#ifndef __EP_XML_FILE_H__
#define __EP_XML_FILE_H__
#include "epLib.h"
#include "epSystem.h"
#include "epXMLite.h"
#include "epBaseFile.h"

#ifndef TEST_NEW
#define TEST_NEW 1
#endif //TEST_NEW
namespace epl
{
#if TEST_NEW
	/*!
	@class XMLFile epXMLFile.h
	@brief A class for XML File.
	*/
	class EP_FOUNDATION XMLFile:public XNode, public BaseFile{
	public:
		/*!
		Default Constructor

		Initializes the XML File 
		@param[in] encodingType the encoding type for this file
		*/
		XMLFile(FileEncodingType encodingType=FILE_ENCODING_TYPE_UTF16);

		/*!
		Default Destructor

		Destroy the XML File 
		*/
		virtual ~XMLFile();

		/*!
		Recursively set all nodes of the given node name with the node value given
		@param[in] nodeName the name of the node to replace
		@param[in] attrName the attribute name of the node
		@param[in] attrVal the attribute value of the node with given attribute name
		@param[in] nodeVal the value for replacement when found
		*/
		void SetNodeValue(CString nodeName, CString attrName, CString attrVal, CString nodeVal);

		/*!
		Clear the list of the xml
		*/
		void Clear();

		/*!
		Save the xml to the given file
		@param[in] fileName the name of the file to save the xml
		@return true if successfully saved, otherwise false
		*/
		bool SaveToFile(CString fileName);

		/*!
		Load the xml from the given file
		@param[in] fileName the name of the file to load the xml
		@return true if successfully loaded, otherwise false
		*/
		bool LoadFromFile(CString fileName);

	protected:
		/*!
		Loop Function that writes to the file
		** Sub classes should implement this function
		*/
		virtual void writeLoop();

		/*!
		Actual load Function that loads values from the file
		** Sub classes should implement this function
		*/
		virtual void loadFromFile(CString line);

		/*!
		Recursively find all nodes with the given node name
		@param[in] node the root of the xml
		@param[in] nodeName the node name to find the nodes
		@return the list of the nodes with the given node name
		*/
		XNodes findAllNode(XNode *node, CString nodeName);
	};
#else
	/*!
	@class XMLFile epXMLFile.h
	@brief A class for XML File.
	*/
	class EP_FOUNDATION XMLFile:public XNode{
	public:
		/*!
		Default Constructor

		Initializes the XML File 
		@param[in] encodingType the encoding type for this file
		*/
		XMLFile(FileEncodingType encodingType=FILE_ENCODING_TYPE_UTF16);

		/*!
		Default Destructor

		Destroy the XML File 
		*/
		virtual ~XMLFile();

		/*!
		Recursively set all nodes of the given node name with the node value given
		@param[in] nodeName the name of the node to replace
		@param[in] attrName the attribute name of the node
		@param[in] attrVal the attribute value of the node with given attribute name
		@param[in] nodeVal the value for replacement when found
		*/
		void SetNodeValue(CString nodeName, CString attrName, CString attrVal, CString nodeVal);

		/*!
		Clear the list of the xml
		*/
		void Clear();

		/*!
		Save the xml to the given file
		@param[in] fileName the name of the file to save the xml
		@return true if successfully saved, otherwise false
		*/
		bool SaveToFile(CString fileName);

		/*!
		Load the xml from the given file
		@param[in] fileName the name of the file to load the xml
		@return true if successfully loaded, otherwise false
		*/
		bool LoadFromFile(CString fileName);

	protected:

		/*!
		Recursively find all nodes with the given node name
		@param[in] node the root of the xml
		@param[in] nodeName the node name to find the nodes
		@return the list of the nodes with the given node name
		*/
		XNodes findAllNode(XNode *node, CString nodeName);

		/// Encoding type of the file
		FileEncodingType m_encodingType;
	};

#endif 

}

#endif //__EP_XML_FILE_H__