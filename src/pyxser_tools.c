/* -*- mode: c; indent-tabs-mode: t; tab-width: 4; c-file-style: "python" -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 noexpandtab: */
/* kate: space-indent off; indent-width 4; mixedindent off; indent-mode cstyle; */
/*
    Copyright (c) 2009 Daniel Molina Wegener <dmw@coder.cl>

    This file is part of pyxser.

    pyxser is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    pyxser is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with pyxser.  If not, see <http://www.gnu.org/licenses/>.

    <!DOCTYPE pyxs:obj
              PUBLIC "-//coder.cl//DTD pyxser 1.0//EN"
              "http://projects.coder.cl/pyxser/dtd/pyxser-1.0.dtd">
 */
#ifndef lint
static const char Id[] = "$Id$";
#endif /* !lint */

#include "include/pyxser.h"

static xmlNsPtr pyxser_default_ns;

/* numbers */
int pyxserInt_Check(PyObject *o);
int pyxserInt_CheckExact(PyObject *o);
int pyxserBool_Check(PyObject *o);
int pyxserLong_CheckExact(PyObject *o);
int pyxserFloat_Check(PyObject *o);
int pyxserFloat_CheckExact(PyObject *o);
int pyxserComplex_Check(PyObject *o);
int pyxserComplex_CheckExact(PyObject *o);

/* strings */
int pyxserString_Check(PyObject *o);
int pyxserString_CheckExact(PyObject *o);
int pyxserUnicode_Check(PyObject *o);
int pyxserUnicodeExact_CheckExact(PyObject *o);
int pyxserBuffer_Check(PyObject *o);

/* tuples */
int pyxserTuple_Check(PyObject *o);
int pyxserTuple_CheckExact(PyObject *o);

/* lists */
int pyxserList_Check(PyObject *o);
int pyxserList_CheckExact(PyObject *o);

/* dictionaries */
int pyxserDict_Check(PyObject *o);
int pyxserDict_CheckExact(PyObject *o);

/* files */
int pyxserFile_Check(PyObject *o);
int pyxserFile_CheckExact(PyObject *o);

/* instrospection */
int pyxserInstance_Check(PyObject *o);
int pyxserFunction_Check(PyObject *o);
int pyxserMethod_Check(PyObject *o);
int pyxserModule_Check(PyObject *o);
int pyxserModule_CheckExact(PyObject *o);
int pyxserSeqIter_Check(PyObject *o);
int pyxserSlice_Check(PyObject *o);
int pyxserCell_Check(PyObject *o);
int pyxserGen_Check(PyObject *o);
int pyxserDate_CheckExact(PyObject *o);
int pyxserAnySet_Check(PyObject *o);
int pyxserAnySet_CheckExact(PyObject *o);
int pyxserFrozenSet_CheckExact(PyObject *o);

/* numbers */
int pyxunserInt_Check(xmlNodePtr node);
int pyxunserInt_CheckExact(xmlNodePtr node);
int pyxunserBool_Check(xmlNodePtr node);
int pyxunserLong_CheckExact(xmlNodePtr node);
int pyxunserFloat_Check(xmlNodePtr node);
int pyxunserFloat_CheckExact(xmlNodePtr node);
int pyxunserComplex_Check(xmlNodePtr node);
int pyxunserComplex_CheckExact(xmlNodePtr node);

/* strings */
int pyxunserString_Check(xmlNodePtr node);
int pyxunserString_CheckExact(xmlNodePtr node);
int pyxunserUnicode_Check(xmlNodePtr node);
int pyxunserUnicodeExact_CheckExact(xmlNodePtr node);
int pyxunserBuffer_Check(xmlNodePtr node);

/* tuples */
int pyxunserTuple_Check(xmlNodePtr node);
int pyxunserTuple_CheckExact(xmlNodePtr node);

/* lists */
int pyxunserList_Check(xmlNodePtr node);
int pyxunserList_CheckExact(xmlNodePtr node);

/* dictionaries */
int pyxunserDict_Check(xmlNodePtr node);
int pyxunserDict_CheckExact(xmlNodePtr node);

/* files */
int pyxunserFile_Check(xmlNodePtr node);
int pyxunserFile_CheckExact(xmlNodePtr node);

/* instrospection */
int pyxunserInstance_Check(xmlNodePtr node);
int pyxunserFunction_Check(xmlNodePtr node);
int pyxunserMethod_Check(xmlNodePtr node);
int pyxunserModule_Check(xmlNodePtr node);
int pyxunserModule_CheckExact(xmlNodePtr node);
int pyxunserSeqIter_Check(xmlNodePtr node);
int pyxunserSlice_Check(xmlNodePtr node);
int pyxunserCell_Check(xmlNodePtr node);
int pyxunserGen_Check(xmlNodePtr node);
int pyxunserDate_CheckExact(xmlNodePtr node);
int pyxunserAnySet_Check(xmlNodePtr node);
int pyxunserAnySet_CheckExact(xmlNodePtr node);
int pyxunserFrozenSet_CheckExact(xmlNodePtr node);


static const char type_int[] = "int";
static const char type_long[] = "long";
static const char type_bool[] = "bool";
static const char type_str[] = "str";
static const char type_unicode[] = "unicode";
static const char type_float[] = "float";
static const char type_double[] = "double";
static const char type_complex[] = "complex";
static const char type_buffer[] = "buffer";
static const char type_list[] = "list";
static const char type_tuple[] = "tuple";
static const char type_dict[] = "dict";
static const char type_main[] = "__main__";

xmlDtdPtr pyxser_dtd_object = (xmlDtdPtr)NULL;

const PythonTypeSerialize serxConcreteTypes[] = {
	/* Numeric Types */
	{1, pyxserInt_Check, pyxser_SerializeInt},
	{1, pyxserInt_CheckExact, pyxser_SerializeExactInt},
	{1, pyxserBool_Check, pyxser_SerializeBoolean},
	{1, pyxserLong_CheckExact, pyxser_SerializeLong},
	{1, pyxserFloat_Check, pyxser_SerializeFloat},
	{1, pyxserFloat_CheckExact, pyxser_SerializeExactFloat},
	{1, pyxserComplex_Check, pyxser_SerializeComplex},
	{1, pyxserComplex_CheckExact, pyxser_SerializeExactComplex},
	/* Strings */
	{1, pyxserString_Check, pyxser_SerializeString},
	{1, pyxserString_CheckExact, pyxser_SerializeExactString},
	{1, pyxserUnicode_Check, pyxser_SerializeUnicode},
	{1, pyxserUnicodeExact_CheckExact, pyxser_SerializeExactUnicode},
	{1, pyxserBuffer_Check, pyxser_SerializeBuffer},
	/* Tuples */
	{1, pyxserTuple_Check, pyxser_SerializeTuple},
	/* Lists */
	{1, pyxserList_Check, pyxser_SerializeList},
	/* Dictionaries */
	{1, pyxserDict_Check, pyxser_SerializeDict},
#if 0 /* I near future, according to user proposals we will
         join a better representation of those objects */
	/* Files */
	{1, pyxserFile_Check, pyxser_SerializeFile},
	{1, pyxserFile_CheckExact, pyxser_SerializeExactFile},
	/* Instrospection */
	{1, pyxserInstance_Check, pyxser_SerializeInstance},
	{1, pyxserFunction_Check, pyxser_SerializeFunction},
	{1, pyxserMethod_Check, pyxser_SerializeMethod},
	{1, pyxserModule_Check, pyxser_SerializeModule},
	{1, pyxserModule_CheckExact, pyxser_SerializeExactModule},
	{1, pyxserSeqIter_Check, pyxser_SerializeIterator},
	{1, pyxserSlice_Check, pyxser_SerializeSlice},
	{1, pyxserCell_Check, pyxser_SerializeCell},
	{1, pyxserGen_Check, pyxser_SerializeGenerator},
	{1, pyxserDate_CheckExact, pyxser_SerializeGenerator},
	{1, pyxserAnySet_Check, pyxser_SerializeSet},
	{1, pyxserAnySet_CheckExact, pyxser_SerializeExactSet},
	{1, pyxserFrozenSet_CheckExact, pyxser_SerializeFrozenSet},
#endif /* !0 */
	{0, NULL, NULL}
};

const PythonTypeDeserialize unserxConcreteTypes[] = {
	/* Numeric Types */
	{1, pyxunserInt_Check, pyxunser_SerializeInt},
	{1, pyxunserInt_CheckExact, pyxunser_SerializeExactInt},
	{1, pyxunserBool_Check, pyxunser_SerializeBoolean},
	{1, pyxunserLong_CheckExact, pyxunser_SerializeLong},
	{1, pyxunserFloat_Check, pyxunser_SerializeFloat},
	{1, pyxunserFloat_CheckExact, pyxunser_SerializeExactFloat},
	{1, pyxunserComplex_Check, pyxunser_SerializeComplex},
	{1, pyxunserComplex_CheckExact, pyxunser_SerializeExactComplex},
	/* Strings */
	{1, pyxunserString_Check, pyxunser_SerializeString},
	{1, pyxunserString_CheckExact, pyxunser_SerializeExactString},
	{1, pyxunserUnicode_Check, pyxunser_SerializeUnicode},
	{1, pyxunserUnicodeExact_CheckExact, pyxunser_SerializeExactUnicode},
	/* Dictionaries */
	{1, pyxunserDict_Check, pyxunser_SerializeDict},
	/* Lists */
	{1, pyxunserList_Check, pyxunser_SerializeList},
	/* Tuples */
	{1, pyxunserTuple_Check, pyxunser_SerializeTuple},
#if 0 /* I near future, according to user proposals we will
         join a better representation of those objects */
	{1, pyxunserBuffer_Check, pyxunser_SerializeBuffer},
	/* Files */
	{1, pyxunserFile_Check, pyxunser_SerializeFile},
	{1, pyxunserFile_CheckExact, pyxunser_SerializeExactFile},
	/* Instrospection */
	{1, pyxunserInstance_Check, pyxunser_SerializeInstance},
	{1, pyxunserFunction_Check, pyxunser_SerializeFunction},
	{1, pyxunserMethod_Check, pyxunser_SerializeMethod},
	{1, pyxunserModule_Check, pyxunser_SerializeModule},
	{1, pyxunserModule_CheckExact, pyxunser_SerializeExactModule},
	{1, pyxunserSeqIter_Check, pyxunser_SerializeIterator},
	{1, pyxunserSlice_Check, pyxunser_SerializeSlice},
	{1, pyxunserCell_Check, pyxunser_SerializeCell},
	{1, pyxunserGen_Check, pyxunser_SerializeGenerator},
	{1, pyxunserDate_CheckExact, pyxunser_SerializeGenerator},
	{1, pyxunserAnySet_Check, pyxunser_SerializeSet},
	{1, pyxunserAnySet_CheckExact, pyxunser_SerializeExactSet},
	{1, pyxunserFrozenSet_CheckExact, pyxunser_SerializeFrozenSet},
#endif /* !0 */
	{0, NULL, NULL}
};


xmlNodePtr
pyxser_SerializeXml(PyObject *o, xmlDocPtr *docPtr, xmlNodePtr *rootNode,
					xmlNodePtr *currentNode, PyListObject *dupSrcItems)
{
	char *objectName = (char *)NULL;
	int phase = PYTHON_SERX_PHASE_NONE;
	int c = 0;
	int serialized = 0;
	long listIterator = 0;
	long listSize = 0;
    xmlDocPtr doc = (xmlDocPtr)NULL;
	xmlNodePtr newSerializedNode = (xmlNodePtr)NULL;
	xmlNodePtr *xmlRootNode = (xmlNodePtr *)NULL;
	xmlNodePtr currentSubNode = (xmlNodePtr)NULL;
	xmlAttrPtr pyxserNs = (xmlAttrPtr)NULL;
	xmlAttrPtr pyxserVersion = (xmlAttrPtr)NULL;
	xmlAttrPtr pyxserType = (xmlAttrPtr)NULL;
	xmlAttrPtr pyxserName = (xmlAttrPtr)NULL;
	PythonTypeSerialize currentSerialization;
	PyListObject *dupItems = dupSrcItems;
	PyObject *lstItems = (PyObject *)NULL;
	PyObject *item = (PyObject *)o;
	PyObject *objKeys = Py_None;
	PyObject *currentKey = Py_None;
	if (PYTHON_IS_NONE(dupItems)) {
		dupItems = (PyListObject *)PyList_New(0);
	}
	if (PYTHON_IS_NONE(o)) {
		return (xmlNodePtr)NULL;
	}
	if ((pyxser_PyListContains(dupItems, o)) == PYXSER_NOT_FOUND) {
		PyList_Append((PyObject *)dupItems, o);
		objectName = pyxser_GetClassName(o);
		if (objectName == (char *)NULL) {
			return (xmlNodePtr)NULL;
		}
		if (docPtr != (xmlDocPtr *)NULL &&
			*docPtr == (xmlDocPtr)NULL
			&& rootNode != (xmlNodePtr *)NULL
			&& *rootNode == (xmlNodePtr)NULL) {
			doc = xmlNewDoc(BAD_CAST pyxser_xml_version);
			doc->encoding = (BAD_CAST xmlStrdup(
								 BAD_CAST pyxser_xml_encoding));
			doc->charset = XML_CHAR_ENCODING_UTF8;
			*docPtr = doc;
			*rootNode = xmlNewDocNode(doc,
									  pyxser_GetDefaultNs(),
									  BAD_CAST pyxser_xml_element_object,
									  NULL);
			pyxserNs = xmlNewProp(*rootNode,
								  BAD_CAST pyxser_xml_attr_xml_ns,
								  BAD_CAST pyxser_xml_attr_ns);
			pyxserVersion = xmlNewProp(*rootNode,
									   BAD_CAST pyxser_xml_attr_version,
									   BAD_CAST pyxser_xml_version);
			pyxserType = xmlNewProp(*rootNode,
									BAD_CAST pyxser_xml_attr_type,
									BAD_CAST objectName);
			pyxser_AddModuleAttr(o, *rootNode);
			currentNode = rootNode;
			xmlRootNode = rootNode;
			xmlDocSetRootElement(doc, *xmlRootNode);
		}
		pyxser_AddIdentifier(o, *currentNode);
		lstItems = PyObject_GetAttrString(o, pyxser_attr_dict);
		if (PYTHON_IS_NONE(lstItems)
			|| (PYTHON_IS_NOT_NONE(lstItems)
				&& (long)(PyDict_Size((PyObject *)lstItems)) <= 0)) {
			if (lstItems != Py_None) {
				Py_DECREF(lstItems);
			}
			return (xmlNodePtr)NULL;
		}
		objKeys = PyDict_Keys(lstItems);
		if (PYTHON_IS_NONE(objKeys)
			|| (PYTHON_IS_NOT_NONE(objKeys)
				&& (long)(PyList_Size((PyObject *)objKeys)) <= 0)) {
			if (lstItems != Py_None) {
				Py_DECREF(lstItems);
			}
			if (objKeys != Py_None) {
				Py_DECREF(objKeys);
			}
			return (xmlNodePtr)NULL;
		}
		listIterator = 0;
		listSize = (long)(PyList_Size((PyObject *)objKeys));
		while (listIterator < listSize) {
			currentKey = PyList_GetItem(objKeys, listIterator);
			phase = PYTHON_SERX_PHASE_NONE;
			item = PyDict_GetItem(lstItems, currentKey);
			serialized = 0;
			c = 0;
			currentSerialization = serxConcreteTypes[c];
			while (currentSerialization.available == 1
				   && phase == PYTHON_SERX_PHASE_NONE
				   && serialized == 0) {
				if (currentSerialization.checker(item)
					&& serialized == 0) {
					newSerializedNode = currentSerialization.serializer(
						item,
						PyString_AS_STRING(currentKey),
						dupItems,
						*docPtr);
					if (newSerializedNode != (xmlNodePtr)NULL) {
						xmlAddChild(*currentNode, newSerializedNode);
						phase = PYTHON_SERX_PHASE_CONCRETE;
						serialized = 1;
					}
				}
				currentSerialization = serxConcreteTypes[++c];
				if (currentSerialization.available == 0
					&& phase == PYTHON_SERX_PHASE_NONE
					&& serialized == 0) {
					objectName = pyxser_GetClassName(item);
					phase = PYTHON_SERX_PHASE_OBJECT;
					currentSubNode = xmlNewDocNode(*docPtr,
												   pyxser_GetDefaultNs(),
												   BAD_CAST pyxser_xml_element_object,
												   NULL);
					pyxser_AddModuleAttr(o, currentSubNode);
					pyxserType = xmlNewProp(currentSubNode,
											BAD_CAST pyxser_xml_attr_type,
											BAD_CAST objectName);
					pyxserName = xmlNewProp(currentSubNode,
											BAD_CAST pyxser_xml_attr_name,
											BAD_CAST PyString_AS_STRING(currentKey));
					xmlAddChild(*currentNode, currentSubNode);
					newSerializedNode = pyxser_SerializeXml(item, docPtr,
															rootNode,
															&currentSubNode,
															dupItems);
					serialized = 1;
					c = 0;
				}
			}
			listIterator++;
		}
		Py_DECREF(objKeys);
		Py_DECREF(lstItems);
	} else {
		newSerializedNode = pyxser_AddReference(o, *currentNode);
	}
	return *rootNode;
}

int
pyxser_ModuleNotMain(const char *mod)
{
	if (strncmp(type_main, mod, strlen(type_main)) == 0) {
		return 0;
	}
	return 1;
}


PyObject *
pyxser_UnserializeBlock(PythonUnserializationArgumentsPtr obj)
{
	/* unserialization arguments */
	PyObject **current = obj->current;
	xmlNodePtr *currentNode = obj->currentNode;
	PyDictObject **dups = obj->dups;
	PyDictObject **modules = obj->modules;

	char *attr_name = (char *)NULL;
	int c = 0, ctrl = 0, serialized = 0;
	char *n_type = (char *)NULL;
	char *n_module = (char *)NULL;
	char *n_id = (char *)NULL;
	char *n_ref = (char *)NULL;
	PyObject *cacheMod = (PyObject *)NULL;
	PyObject *currentType = (PyObject *)NULL;
	PyObject *unser = (PyObject *)NULL;
	PythonTypeDeserialize *machine = (PythonTypeDeserialize *)unserxConcreteTypes;
	xmlNodePtr ron = (xmlNodePtr)NULL;
	xmlNodePtr cacheCurrentNode = (xmlNodePtr)NULL;
	PyObject *cacheCurrent = (PyObject *)NULL;
	PyObject *chkMod = (PyObject *)NULL;
	int lenElement = strlen((char *)pyxser_xml_element_item);
	int lenCollection = strlen((char *)pyxser_xml_element_collection);
	int lenObject = strlen((char *)pyxser_xml_element_object);

	for (ron = (*currentNode)->children;
		 ron != (xmlNodePtr)NULL;
		 ron = ron->next) {
		*currentNode = ron;
		if (ron->type == XML_ELEMENT_NODE) {
			if ((strncmp((char *)ron->name,
						 (char *)pyxser_xml_element_item,
						 lenElement)) == 0
				|| (strncmp((char *)ron->name,
							(char *)pyxser_xml_element_collection,
							lenCollection))	== 0) {
				c = 0;
				serialized = 0;
				while (machine[c].available == 1
					   && ron != (xmlNodePtr)NULL
					   && serialized == 0) {
					if ((machine[c].check(ron)) == 1) {
						unser = machine[c].deserializer(obj);
						if (PYTHON_IS_NOT_NONE(unser)) {
							attr_name = pyxser_ExtractPropertyName(
								pyxser_xml_attr_name,
								ron);
							if (attr_name != (char *)NULL) {
								ctrl = PyObject_SetAttrString(*current,
															  attr_name, unser);
								serialized = 1;
							}
						}
					}
					c++;
				}
			} else if ((strncmp((char *)ron->name,
								(char *)pyxser_xml_element_object,
								lenObject)) == 0) {
				n_type = (char *)xmlGetProp(ron,
											BAD_CAST pyxser_xml_attr_type);
				n_module = (char *)xmlGetProp(ron,
											  BAD_CAST pyxser_xml_attr_module);
				n_id = (char *)xmlGetProp(ron,
										  BAD_CAST pyxser_xml_attr_id);
				n_ref = (char *)xmlGetProp(ron,
										   BAD_CAST pyxser_xml_attr_ref);
				unser = pyxser_CheckAvailableObject((PyObject *)*dups,
													(char *)n_ref);
				if (n_type != (char *)NULL
					&& n_module != (char *)NULL
					&& n_id != (char *)NULL
					&& unser == (PyObject *)NULL) {
					if ((pyxser_ModuleNotMain(n_module)) == 0) {
						continue;
					}
					cacheMod = pyxser_SearchModule(n_module);
					cacheMod = cacheMod == (PyObject *)NULL ?
						PyImport_ImportModule(n_module) : cacheMod;
					if (PYTHON_IS_NONE(cacheMod)) {
						continue;
					}
					chkMod = pyxser_CacheModule((PyObject *)*modules,
												n_module);
					if (!PYTHON_IS_NOT_NONE(chkMod)) {
						PyDict_SetItemString((PyObject *)*modules,
											 n_module, cacheMod);
					}
					currentType = (PyObject *)pyxser_SearchModuleType(cacheMod,
																	  n_type);
					if (PYTHON_IS_NOT_NONE(currentType)) {
						unser = PyInstance_NewRaw(currentType, PyDict_New());
						attr_name = pyxser_ExtractPropertyName(pyxser_xml_attr_name,
															   ron);
						if (PYTHON_IS_NOT_NONE(unser)) {
							if (attr_name != (char *)NULL) {
								ctrl = PyObject_SetAttrString(*current,
															  attr_name, unser);
							}
							pyxser_AddAvailableObject((PyObject *)*dups,
													  (char *)n_id, unser);
							cacheCurrent = *(obj->current);
							cacheCurrentNode = ron;
							*(obj->current) = unser;
							*(obj->currentNode) = ron;
							unser = pyxser_UnserializeBlock(obj);
							*(obj->current) = cacheCurrent;
							*(obj->currentNode) = cacheCurrentNode;
							ron = cacheCurrentNode;
						}
					} else {
						currentType = pyxser_SearchObjectInMain(n_type);
						if (PYTHON_IS_NOT_NONE(currentType)) {
							unser = PyInstance_NewRaw(currentType, PyDict_New());
							attr_name =	pyxser_ExtractPropertyName(pyxser_xml_attr_name,
																   ron);
							if (attr_name != (char *)NULL
								&& PYTHON_IS_NOT_NONE(unser)) {
								ctrl = PyObject_SetAttrString(*current,
															  attr_name, unser);
								pyxser_AddAvailableObject((PyObject *)*dups,
														  (char *)n_id, unser);
								if (ctrl == 0) {
									cacheCurrent = *(obj->current);
									cacheCurrentNode = ron;
									*(obj->current) = unser;
									*(obj->currentNode) = ron;
									unser = pyxser_UnserializeBlock(obj);
									*(obj->current) = cacheCurrent;
									*(obj->currentNode) = cacheCurrentNode;
									ron = cacheCurrentNode;
								}
							}
						}
					}
				} else {
					attr_name = pyxser_ExtractPropertyName(pyxser_xml_attr_name, ron);
					if (attr_name != (char *)NULL
						&& PYTHON_IS_NOT_NONE(unser)) {
						ctrl = PyObject_SetAttrString(*current, attr_name, unser);
					}
				}
			}
		}
	}
	return *(obj->current);
}


PyObject *
pyxser_UnserializeXml(PythonUnserializationArgumentsPtr obj)
{
	PyObject **doc = obj->doc;
	PyObject **current = obj->current;
	PyObject **tree = obj->tree;
	PyDictObject **dups = obj->dups;
	PyDictObject **modules = obj->modules;
	PyObject *chkMod = (PyObject *)NULL;
	xmlDocPtr *docPtr = obj->docPtr;
	xmlNodePtr *rootNode = obj->rootNode;
	xmlNodePtr *currentNode = obj->currentNode;

	xmlNodePtr cacheCurrentNode = (xmlNodePtr)NULL;
	PyObject *cacheCurrent = (PyObject *)NULL;

	PyObject *cacheMod = (PyObject *)NULL;
	PyObject *currentType = (PyObject *)NULL;
	char *n_type = (char *)NULL;
	char *n_module = (char *)NULL;
	char *n_id = (char *)NULL;
	char *strdoc = (char *)NULL;
	if (PYTHON_IS_NONE(doc)) {
		return Py_None;
	}
	strdoc = PyString_AS_STRING(*doc);
	if (strdoc == (char *)NULL) {
		return Py_None;
	}
	*docPtr = xmlReadMemory((const char *)strdoc, strlen(strdoc), NULL,
							(const char *)pyxser_xml_encoding, 0);
	if (*docPtr != (xmlDocPtr)NULL) {
		if ((pyxser_ValidateDocument(*docPtr)) == 1) {
			*rootNode = xmlDocGetRootElement(*docPtr);
			currentNode = rootNode;
			*currentNode = *rootNode;
			obj->currentNode = currentNode;
			n_type = (char *)xmlGetProp(*currentNode,
										BAD_CAST pyxser_xml_attr_type);
			n_module = (char *)xmlGetProp(*currentNode,
										  BAD_CAST pyxser_xml_attr_module);
			n_id = (char *)xmlGetProp(*currentNode,
									  BAD_CAST pyxser_xml_attr_id);
			if (*dups == (PyDictObject *)NULL) {
				*dups = (PyDictObject *)PyDict_New();
			}
			if (*modules == (PyDictObject *)NULL) {
				*modules = (PyDictObject *)PyDict_New();
			}
			if (n_type != (char *)NULL
				&& n_module != (char *)NULL
				&& n_id != (char *)NULL) {
				if ((pyxser_ModuleNotMain(n_module)) == 1) {
					cacheMod = pyxser_SearchModule(n_module);
					cacheMod = cacheMod == (PyObject *)NULL ?
						PyImport_ImportModule(n_module) : cacheMod;
					if (PYTHON_IS_NOT_NONE(cacheMod)) {
						chkMod = pyxser_CacheModule((PyObject *)*modules,
													n_module);
						if (!PYTHON_IS_NOT_NONE(chkMod)) {
							PyDict_SetItemString((PyObject *)*modules,
												 n_module, cacheMod);
						}
						currentType = (PyObject *)pyxser_SearchModuleType(cacheMod, n_type);
						if (PYTHON_IS_NOT_NONE(currentType)) {
							if (*tree == (PyObject *)NULL) {
								*tree = PyInstance_NewRaw(currentType, PyDict_New());
								*current = *tree;
								obj->current = current;
								obj->tree = tree;
								pyxser_UnserializeBlock(obj);
								*(obj->current) = cacheCurrent;
								*(obj->currentNode) = cacheCurrentNode;
								obj->tree = tree;
							}
						}
					}
				} else {
					currentType = pyxser_SearchObjectInMain(n_type);
					if (PYTHON_IS_NOT_NONE(currentType)) {
						if (*tree == (PyObject *)NULL) {
							*tree = PyInstance_NewRaw(currentType, PyDict_New());
							*current = *tree;
							obj->current = current;
							obj->tree = tree;
							pyxser_UnserializeBlock(obj);
							*(obj->current) = cacheCurrent;
							*(obj->currentNode) = cacheCurrentNode;
						}
					}
				}
			}
		}
	}
	if (*docPtr != (xmlDocPtr)NULL) {
		xmlFreeDoc(*docPtr);
	}
	Py_DECREF(*modules);
	Py_DECREF(*dups);
	return *tree;
}


PyObject *
pyxser_SearchModuleType(PyObject *mod, const char *name)
{
	PyObject *dict = (PyObject *)NULL;
	PyObject *item = (PyObject *)NULL;
	PyObject *objKeys = (PyObject *)NULL;
	PyObject *currentKey = (PyObject *)NULL;
	long listIterator = 0;
	long listSize = 0;
	char *keyName = (char *)NULL;
	if (PYTHON_IS_NOT_NONE(mod)
		&& name != (const char *)NULL) {
		objKeys = PyObject_GetAttrString(mod, pyxser_attr_all);
		dict = PyObject_GetAttrString(mod, pyxser_attr_dict);
		if (PYTHON_IS_NOT_NONE(objKeys)
			&& (long)(PyList_Size((PyObject *)objKeys)) > 0
			&& PYTHON_IS_NOT_NONE(dict)) {
			listIterator = 0;
			listSize = (long)(PyList_Size((PyObject *)objKeys));
			while (listIterator < listSize) {
				currentKey = PyList_GetItem(objKeys, listIterator);
				keyName = PyString_AS_STRING(currentKey);
				if ((strncmp(keyName, name, strlen(keyName))) == 0) {
					item = PyDict_GetItem(dict, currentKey);
					break;
				}
				listIterator++;
			}
		}
	}
	return item;
}


char *
pyxser_GetClassName(PyObject *obj)
{
	char *cn = (char *)NULL;
	PyObject *klass = Py_None;
	PyObject *cname = Py_None;
	if (PYTHON_IS_NOT_NONE(obj)) {
		klass = PyObject_GetAttrString(obj, pyxser_attr_class);
		if (PYTHON_IS_NONE(klass)) {
			return cn;
		}
		cname = PyObject_GetAttrString(klass, pyxser_attr_name);
		if (PYTHON_IS_NONE(cname)) {
			Py_DECREF(klass);
			return cn;
		}
		if (!PyString_Check(cname)) {
			Py_DECREF(klass);
			Py_DECREF(cname);
		}
		cn = PyString_AS_STRING(cname);
		Py_DECREF(klass);
		Py_DECREF(cname);
	}
	return cn;
}


xmlNodePtr
pyxser_SerializePrimitiveType(PyObject *o, char *name,
							  PyListObject *dupItems,
							  xmlDocPtr doc)
{
	xmlNodePtr newElementNode = (xmlNodePtr)NULL;
	xmlNodePtr newTextNode = (xmlNodePtr)NULL;
	xmlAttrPtr typeAttr = (xmlAttrPtr)NULL;
	xmlAttrPtr nameAttr = (xmlAttrPtr)NULL;
	PyObject *str = (PyObject *)NULL;
	PyObject *classPtr = (PyObject *)NULL;
	PyObject *className = (PyObject *)NULL;
	char *sptr = (char *)NULL;
	char *nptr = (char *)NULL;
	if (PYTHON_IS_NOT_NONE(o)) {
		str = PyObject_Str(o);
		if (PYTHON_IS_NOT_NONE(str)) {
			sptr = PyString_AS_STRING(str);
			classPtr = PyObject_GetAttrString(o, pyxser_attr_class);
			if (PYTHON_IS_NOT_NONE(classPtr)
				&& sptr != (char *)NULL) {
				className = PyObject_GetAttrString(classPtr, pyxser_attr_name);
				if (PYTHON_IS_NOT_NONE(className)) {
					nptr = PyString_AS_STRING(className);
					if (sptr != (char *)NULL
						&& nptr != (char *)NULL) {
						newElementNode = xmlNewDocNode(doc, pyxser_GetDefaultNs(),
													   BAD_CAST pyxser_xml_attr_item,
													   NULL);
						newTextNode = xmlNewDocText(doc, BAD_CAST sptr);
						typeAttr = xmlNewProp(newElementNode,
											  BAD_CAST pyxser_xml_attr_type,
											  BAD_CAST nptr);
						nameAttr = xmlNewProp(newElementNode,
											  BAD_CAST pyxser_xml_attr_name,
											  BAD_CAST name);
						xmlAddChild(newElementNode, newTextNode);
					}
					Py_DECREF(className);
				}
				Py_DECREF(classPtr);
			}
		}
	}
	return newElementNode;
}


xmlNodePtr
pyxser_AddReference(PyObject *o, xmlNodePtr currentNode)
{
	PyObject *longIdentifier = (PyObject *)NULL;
	PyObject *stringRepr = (PyObject *)NULL;
	char *charpRepr = (char *)NULL;
	char newRef[33] = "id-";
	xmlAttrPtr refAttr = (xmlAttrPtr)NULL;
	xmlNodePtr refNode = (xmlNodePtr)NULL;
	long hash = -1;
	if (PYTHON_IS_NOT_NONE(o)
		&& currentNode != (xmlNodePtr)NULL) {
		hash = PyObject_Hash(o);
		if (hash != -1) {
			longIdentifier = PyLong_FromLong(hash);
			stringRepr = PyObject_Str(longIdentifier);
			charpRepr = PyString_AS_STRING(stringRepr);
			if (charpRepr != (char *)NULL) {
				strncat(newRef, charpRepr, 32);
				refAttr = xmlNewProp(currentNode,
									 BAD_CAST pyxser_xml_attr_ref,
									 BAD_CAST newRef);
			}
			Py_DECREF(longIdentifier);
			Py_DECREF(stringRepr);
		}
	}
	return refNode;
}


void
pyxser_AddIdentifier(PyObject *o, xmlNodePtr currentNode)
{
	PyObject *longIdentifier = (PyObject *)NULL;
	PyObject *stringRepr = (PyObject *)NULL;
	char *charpRepr = (char *)NULL;
	char newRef[33] = "id-";
	xmlAttrPtr idAttr = (xmlAttrPtr)NULL;
	long hash = -1;
	if (PYTHON_IS_NOT_NONE(o)
		&& currentNode != (xmlNodePtr)NULL) {
		hash = PyObject_Hash(o);
		if (hash != -1) {
			longIdentifier = PyLong_FromLong(hash);
			stringRepr = PyObject_Str(longIdentifier);
			charpRepr = PyString_AS_STRING(stringRepr);
			strncat(newRef, charpRepr, 32);
			idAttr = xmlNewProp(currentNode,
								BAD_CAST pyxser_xml_attr_id,
								BAD_CAST newRef);
			Py_DECREF(stringRepr);
			Py_DECREF(longIdentifier);
		}
	}
}


void
pyxser_AddModuleAttr(PyObject *o, xmlNodePtr currentNode)
{
	char *cn = (char *)NULL;
	PyObject *klass = Py_None;
	PyObject *mname = Py_None;
	xmlAttrPtr moduleAttr = (xmlAttrPtr)NULL;
	if (PYTHON_IS_NOT_NONE(o)
		&& currentNode != (xmlNodePtr)NULL) {
		klass = PyObject_GetAttrString(o, pyxser_attr_class);
		if (PYTHON_IS_NONE(klass)) {
			return;
		}
		mname = PyObject_GetAttrString(klass, pyxser_attr_module);
		if (PYTHON_IS_NONE(mname)) {
			Py_DECREF(klass);
			return;
		}
		if (!PyString_Check(mname)) {
			Py_DECREF(klass);
			Py_DECREF(mname);
		}
		cn = PyString_AS_STRING(mname);
		Py_DECREF(klass);
		Py_DECREF(mname);
		moduleAttr = xmlNewProp(currentNode,
								BAD_CAST pyxser_xml_attr_module,
								BAD_CAST cn);
	}
}


int
pyxser_PyListContains(PyListObject *lst, PyObject *o)
{
	PyObject *iterLst = (PyObject *)NULL;
	PyObject *item = (PyObject *)NULL;
	if (PYTHON_IS_NOT_NONE(lst)
		&& PYTHON_IS_NOT_NONE(o)) {
		iterLst = PyObject_GetIter((PyObject *)lst);
		if (PYTHON_IS_NOT_NONE(iterLst)) {
			while ((item = PyIter_Next(iterLst)) != (PyObject *)NULL) {
				if (item == o) {
					Py_DECREF(iterLst);
					return PYXSER_FOUND;
				}
			}
			Py_DECREF(iterLst);
			return PYXSER_NOT_FOUND;
		}
	}
	return PYXSER_INVALID_ARGUMENT;
}


/* numbers */
int
pyxserInt_Check(PyObject *o)
{
	return PyInt_Check(o);
}

int
pyxserInt_CheckExact(PyObject *o)
{
	return PyInt_CheckExact(o);
}

int
pyxserBool_Check(PyObject *o)
{
	return PyBool_Check(o);
}

int
pyxserLong_CheckExact(PyObject *o)
{
	return PyLong_CheckExact(o);
}

int
pyxserFloat_Check(PyObject *o)
{
	return PyFloat_Check(o);
}

int
pyxserFloat_CheckExact(PyObject *o)
{
	return PyFloat_CheckExact(o);
}

int
pyxserComplex_Check(PyObject *o)
{
	return PyComplex_Check(o);
}

int
pyxserComplex_CheckExact(PyObject *o)
{
	return PyComplex_CheckExact(o);
}

/* strings */
int
pyxserString_Check(PyObject *o)
{
	return PyString_Check(o);
}

int
pyxserString_CheckExact(PyObject *o)
{
	return PyString_CheckExact(o);
}

int
pyxserUnicode_Check(PyObject *o)
{
	return PyUnicode_Check(o);
}

int
pyxserUnicodeExact_CheckExact(PyObject *o)
{
	return PyUnicode_CheckExact(o);
}

int
pyxserBuffer_Check(PyObject *o)
{
	return PyBuffer_Check(o);
}

/* tuples */
int
pyxserTuple_Check(PyObject *o)
{
	return PyTuple_Check(o);
}

int
pyxserTuple_CheckExact(PyObject *o)
{
	return PyTuple_CheckExact(o);
}

/* lists */
int
pyxserList_Check(PyObject *o)
{
	return PyList_Check(o);
}

int
pyxserList_CheckExact(PyObject *o)
{
	return PyList_CheckExact(o);
}

/* dictionaries */
int pyxserDict_Check(PyObject *o)
{
	return PyDict_Check(o);
}

int
pyxserDict_CheckExact(PyObject *o)
{
	return PyDict_CheckExact(o);
}

/* files */
int
pyxserFile_Check(PyObject *o)
{
	return PyFile_Check(o);
}

int pyxserFile_CheckExact(PyObject *o)
{
	return PyFile_CheckExact(o);
}

/* instrospection */
int
pyxserInstance_Check(PyObject *o)
{
	return PyInstance_Check(o);
}

int
pyxserFunction_Check(PyObject *o)
{
	return PyFunction_Check(o);
}

int
pyxserMethod_Check(PyObject *o)
{
	return PyMethod_Check(o);
}

int
pyxserModule_Check(PyObject *o)
{
	return PyModule_Check(o);
}

int
pyxserModule_CheckExact(PyObject *o)
{
	return PyModule_CheckExact(o);
}

int
pyxserSeqIter_Check(PyObject *o)
{
	return PySeqIter_Check(o);
}

int
pyxserSlice_Check(PyObject *o)
{
	return PySlice_Check(o);
}

int
pyxserCell_Check(PyObject *o)
{
	return PyCell_Check(o);
}

int
pyxserGen_Check(PyObject *o)
{
	return PyGen_Check(o);
}

int
pyxserDate_CheckExact(PyObject *o)
{
	PyDateTime_IMPORT;
	return PyDate_Check(o);
}

int
pyxserAnySet_Check(PyObject *o)
{
	return PyAnySet_Check(o);
}

int
pyxserAnySet_CheckExact(PyObject *o)
{
	return PyAnySet_CheckExact(o);
}

int
pyxserFrozenSet_CheckExact(PyObject *o)
{
	return PyFrozenSet_CheckExact(o);
}


int
pyxunserInt_Check(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_int, strlen(type_int))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserInt_CheckExact(xmlNodePtr node)
{
	return pyxunserInt_Check(node);
}


int
pyxunserBool_Check(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_bool, strlen(type_bool))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserLong_CheckExact(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_long, strlen(type_long))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserFloat_Check(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_float, strlen(type_float))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserFloat_CheckExact(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_float, strlen(type_float))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserComplex_Check(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_complex, strlen(type_complex))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserComplex_CheckExact(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_complex, strlen(type_complex))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserString_Check(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_str, strlen(type_str))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserString_CheckExact(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_str, strlen(type_str))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserUnicode_Check(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_unicode, strlen(type_unicode))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserUnicodeExact_CheckExact(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_unicode, strlen(type_unicode))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserBuffer_Check(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_buffer, strlen(type_buffer))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserTuple_Check(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_tuple, strlen(type_tuple))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserList_Check(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_list, strlen(type_list))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int
pyxunserDict_Check(xmlNodePtr node)
{
	char *prop;
	if (node != (xmlNodePtr)NULL) {
		prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
		if (prop != (char *)NULL) {
			if ((strncmp(prop, type_dict, strlen(type_dict))) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

xmlNsPtr
pyxser_GetDefaultNs()
{
	if (pyxser_default_ns != (xmlNsPtr)NULL) {
		return pyxser_default_ns;
	} else {
		pyxser_default_ns = xmlNewNs(NULL,
									 BAD_CAST pyxser_xml_attr_ns,
									 BAD_CAST pyxser_xml_attr_ns_prefix);
		return pyxser_default_ns;
	}
}


char *
pyxser_ExtractPropertyName(const char *name, xmlNodePtr node)
{
	if (node != (xmlNodePtr)NULL && name != (char *)NULL) {
		return (char *)xmlGetProp(node, BAD_CAST name);
	}
	return (char *)NULL;
}


PyObject *
pyxser_SearchObjectInMain(const char *name)
{
	PyObject *dictMod = PyImport_GetModuleDict();
	PyObject *item = (PyObject *)NULL;
	PyObject *currentKey = (PyObject *)NULL;
	PyObject *dictKeys = (PyObject *)NULL;
	PyObject *currentType = (PyObject *)NULL;
	int counter = 0;
	long tupleSize = 0;
	if (PYTHON_IS_NOT_NONE(dictMod)) {
		dictKeys = PyDict_Keys(dictMod);
		tupleSize = PyDict_Size(dictMod);
		if (PYTHON_IS_NOT_NONE(dictKeys)) {
			for (counter = 0; counter < tupleSize; counter++) {
				currentKey = PyList_GetItem(dictKeys, counter);
				if ((strncmp(PyString_AS_STRING(currentKey),
							 type_main,
							 strlen(type_main))) == 0) {
					item = PyDict_GetItem(dictMod, currentKey);
					if (PYTHON_IS_NOT_NONE(item)) {
						currentType = (PyObject *)pyxser_SearchModuleType(item, name);
					}
				}
			}
		}
	}
	return currentType;
}

PyObject *
pyxser_SearchModule(const char *name)
{
	PyObject *dictMod = PyImport_GetModuleDict();
	PyObject *item = (PyObject *)NULL;
	PyObject *currentKey = (PyObject *)NULL;
	PyObject *dictKeys = (PyObject *)NULL;
	int counter = 0;
	long tupleSize = 0;
	if (PYTHON_IS_NOT_NONE(dictMod)) {
		dictKeys = PyDict_Keys(dictMod);
		tupleSize = PyDict_Size(dictMod);
		if (PYTHON_IS_NOT_NONE(dictKeys)) {
			for (counter = 0; counter < tupleSize; counter++) {
				currentKey = PyList_GetItem(dictKeys, counter);
				if ((strncmp(PyString_AS_STRING(currentKey),
							 name,
							 strlen(type_main))) == 0) {
					item = PyDict_GetItem(dictMod, currentKey);
				}
			}
		}
	}
	return item;
}

PyObject *
pyxser_CacheModule(PyObject *d, const char *name)
{
	PyObject *dictMod = d;
	PyObject *item = (PyObject *)NULL;
	PyObject *currentKey = (PyObject *)NULL;
	PyObject *dictKeys = (PyObject *)NULL;
	int counter = 0;
	long tupleSize = 0;
	if (PYTHON_IS_NOT_NONE(dictMod)) {
		tupleSize = PyDict_Size(dictMod);
		if (tupleSize > 0) {
			dictKeys = PyDict_Keys(dictMod);
			if (PYTHON_IS_NOT_NONE(dictKeys)) {
				for (counter = 0; counter < tupleSize; counter++) {
					currentKey = PyList_GetItem(dictKeys, counter);
					if ((strncmp(PyString_AS_STRING(currentKey),
								 name,
								 strlen(type_main))) == 0) {
						item = PyDict_GetItem(dictMod, currentKey);
					}
				}
			}
		}
	}
	return item;
}

inline char *
pyxser_GetObjectIdentifier(xmlNodePtr node)
{
	if (node != (xmlNodePtr)NULL) {
		return (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_id);
	}
	return (char *)NULL;
}

PyObject *
pyxser_CheckAvailableObject(PyObject *dict, char *id)
{
	PyObject *ret = (PyObject *)NULL;
	if (dict != (PyObject *)NULL
		&& id != (char *)NULL) {
		ret = PyDict_GetItemString(dict, id);
	}
	return ret;
}

void
pyxser_AddAvailableObject(PyObject *dict, char *id, PyObject *o)
{
	if (dict != (PyObject *)NULL
		&& id != (char *)NULL
		&& o != (PyObject *)NULL) {
		PyDict_SetItemString(dict, id, o);
	}
}


xmlDtdPtr
pyxser_GetPyxserDTD()
{
	if (pyxser_dtd_object == (xmlDtdPtr)NULL) {
		pyxser_dtd_object = xmlParseDTD(BAD_CAST NULL,
										BAD_CAST pyxser_xml_dtd_location);
	}
	return pyxser_dtd_object;
}

int
pyxser_ValidateDocument(xmlDocPtr doc)
{
	xmlDtdPtr dtd = pyxser_GetPyxserDTD();
	xmlValidCtxtPtr cvp;
	if ((cvp = xmlNewValidCtxt()) == NULL) {
		return 0;
	}
	cvp->userData = (void *) stderr;
	cvp->error    = (xmlValidityErrorFunc) fprintf;
	cvp->warning  = (xmlValidityWarningFunc) fprintf;
	if (!xmlValidateDtd(cvp, doc, dtd)) {
		return 0;
	}
	xmlFreeValidCtxt(cvp);
	return 1;
}


/* pyserx_tools.h ends here */