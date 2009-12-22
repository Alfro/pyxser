/* -*- mode: c; indent-tabs-mode: nil; tab-width: 4; c-file-style: "python" -*- */
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

#include <Python.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "include/pyxser.h"
#include "include/pyxser_tools.h"


static xmlNodePtr pyxser_GlobalTupleSerialization(PyxSerializationArgsPtr args);
static xmlNodePtr pyxser_GlobalListSerialization(PyxSerializationArgsPtr args);
static xmlNodePtr pyxser_GlobalDictSerialization(PyxSerializationArgsPtr args);
static xmlNodePtr pyxser_RunSerializationCol(PyxSerializationArgsPtr args);


/* Tuples */
xmlNodePtr
pyxser_SerializeTuple(PyxSerializationArgsPtr args)
{
	return pyxser_GlobalTupleSerialization(args);
}

xmlNodePtr
pyxser_SerializeExactTuple(PyxSerializationArgsPtr args)
{
	return pyxser_GlobalTupleSerialization(args);
}

/* Lists */
xmlNodePtr
pyxser_SerializeList(PyxSerializationArgsPtr args)
{
	return pyxser_GlobalListSerialization(args);
}

xmlNodePtr
pyxser_SerializeExactList(PyxSerializationArgsPtr args)
{
	return pyxser_GlobalListSerialization(args);
}
/* Dictionaries */

xmlNodePtr
pyxser_SerializeDict(PyxSerializationArgsPtr args)
{
	return pyxser_GlobalDictSerialization(args);
}

xmlNodePtr
pyxser_SerializeExactDict(PyxSerializationArgsPtr args)
{
	return pyxser_GlobalDictSerialization(args);
}

static xmlNodePtr
pyxser_RunSerializationCol(PyxSerializationArgsPtr args)
{
	int d = 0;

	xmlNodePtr newElementNode = *args->currentNode;
	xmlNodePtr csn = (xmlNodePtr)NULL;
	xmlNodePtr *rootNodeOld = args->rootNode;
    xmlNodePtr *currentNodeOld = args->currentNode;

	xmlNodePtr newSerNode = (xmlNodePtr)NULL;
	xmlAttrPtr pyxserType = (xmlAttrPtr)NULL;
	xmlAttrPtr nameAttr = (xmlAttrPtr)NULL;
	PythonTypeSerialize currentSerialization;
	char *objectName = (char *)NULL;
	char *typeName = (char *)NULL;
    char *name = (char *)NULL;

    PyObject *item = *args->item;
    PyObject **oold = args->o;
    PyObject *currentKey = *args->ck;
    PyListObject *dupItems = *args->dupSrcItems;
    xmlDocPtr *docPtr = args->docptr;
    int *depthcnt = args->depthcnt;

	currentSerialization = serxConcreteTypes[d];
	while (currentSerialization.available == 1) {
		if (currentSerialization.checker(item)) {
			if (PYTHON_IS_NONE(currentKey)) {
                args->o = &item;
                args->item = &item;
                name = (char *)NULL;
                args->name = name;
				newSerNode = currentSerialization.serializer(args);
                args->o = oold;
                args->currentNode = currentNodeOld;
                args->rootNode = rootNodeOld;
			} else {
                args->o = &item;
                args->item = &item;
                name = PyString_AS_STRING(currentKey);
                args->name = name;
				newSerNode = currentSerialization.serializer(args);
                args->o = oold;
                args->currentNode = currentNodeOld;
                args->rootNode = rootNodeOld;
			}
			if (newSerNode != (xmlNodePtr)NULL) {
				xmlAddChild(newElementNode, newSerNode);
				break;
			}
		}
		currentSerialization = serxConcreteTypes[++d];
		if (currentSerialization.available == 0) {
			if ((pyxser_PyListContains(dupItems, item)) == PYXSER_FOUND) {
				typeName = pyxser_GetClassName(item);
				if (typeName != (char *)NULL) {
					newSerNode = xmlNewDocNode(*docPtr, pyxser_GetDefaultNs(),
											   BAD_CAST pyxser_xml_element_object,
											   NULL);
					pyxser_AddReference(item, newSerNode);
					xmlAddChild(newElementNode, newSerNode);
					break;
				}
			} else {
				objectName = pyxser_GetClassName(item);
				csn = xmlNewDocNode(*docPtr, pyxser_GetDefaultNs(),
											BAD_CAST pyxser_xml_element_object,
											NULL);
				pyxserType = xmlNewProp(csn,
										BAD_CAST pyxser_xml_attr_type,
										BAD_CAST objectName);
				if (PYTHON_IS_NOT_NONE(currentKey)) {
					nameAttr = xmlNewProp(csn,
										  BAD_CAST pyxser_xml_attr_name,
										  BAD_CAST PyString_AS_STRING(currentKey));
				}
				pyxser_AddModuleAttr(item, csn);
                (*depthcnt)++;

                args->o = &item;
                args->item = &item;
                args->rootNode = &csn;
                args->currentNode = &csn;

                newSerNode = pyxser_SerializeXml(args);

                args->o = oold;
                args->item = oold;
                args->currentNode = currentNodeOld;
                args->rootNode = rootNodeOld;

				xmlAddChild(newElementNode, csn);

                (*depthcnt)--;
				break;
			}
		}
	}
	return newSerNode;
}

static xmlNodePtr
pyxser_GlobalListSerialization(PyxSerializationArgsPtr args)
{

    PyObject *o = *args->o;
    PyObject **oold = args->o;
    char *name = args->name;

    xmlDocPtr docPtr = *args->docptr;
	xmlNodePtr *currentNodeOld = args->currentNode;
	xmlNodePtr *rootNodeOld = args->rootNode;

	xmlNodePtr newElementNode = (xmlNodePtr)NULL;
	xmlAttrPtr typeAttr = (xmlAttrPtr)NULL;
	xmlAttrPtr namePtr = (xmlAttrPtr)NULL;
	xmlNodePtr newSerNode = (xmlNodePtr)NULL;
	PyObject *classPtr = (PyObject *)NULL;
	PyObject *className = (PyObject *)NULL;
	PyObject *item = (PyObject *)NULL;
	long tupleSize = 0;
	long counter = 0;
	char *nptr = (char *)NULL;

	if (PYTHON_IS_NONE(o)) {
		return (xmlNodePtr)NULL;
	}
	classPtr = PyObject_GetAttrString(o, pyxser_attr_class);
	if (PYTHON_IS_NONE(classPtr)) {
        PyErr_Clear();
		return (xmlNodePtr)NULL;
	}
	className = PyObject_GetAttrString(classPtr, pyxser_attr_name);
	if (PYTHON_IS_NONE(className)) {
        PyErr_Clear();
        PYXSER_FREE_OBJECT(classPtr);
		return (xmlNodePtr)NULL;
	}
	nptr = PyString_AS_STRING(className);
	if (nptr != (char *)NULL) {
		newElementNode = xmlNewDocNode(docPtr, pyxser_GetDefaultNs(),
									   BAD_CAST pyxser_xml_element_collection,
									   NULL);
		typeAttr = xmlNewProp(newElementNode,
							  BAD_CAST pyxser_xml_attr_type,
							  BAD_CAST nptr);
		namePtr = xmlNewProp(newElementNode,
							 BAD_CAST pyxser_xml_attr_name,
							 BAD_CAST name);
		tupleSize = PyList_GET_SIZE(o);
		for (counter = 0; counter < tupleSize; counter++) {
			item = PyList_GetItem(o, counter);
			if (PYTHON_IS_NONE(item)) {
				continue;
			}
            args->o = &item;
            args->item = &item;
            args->currentNode = &newElementNode;

			newSerNode = pyxser_RunSerializationCol(args);

            args->o = oold;
            args->item = oold;
            args->currentNode = currentNodeOld;
            args->rootNode = rootNodeOld;
		}
	}
    PYXSER_FREE_OBJECT(className);
    PYXSER_FREE_OBJECT(classPtr);
	return newElementNode;
}

static xmlNodePtr
pyxser_GlobalTupleSerialization(PyxSerializationArgsPtr args)
{

    PyObject *o = *args->o;
    char *name = args->name;
    xmlDocPtr docPtr = *args->docptr;

    PyObject **oold = args->o;
    xmlNodePtr *currentNodeOld = args->currentNode;
    xmlNodePtr *rootNodeOld = args->currentNode;

	xmlNodePtr newElementNode = (xmlNodePtr)NULL;
	xmlAttrPtr typeAttr = (xmlAttrPtr)NULL;
	xmlAttrPtr namePtr = (xmlAttrPtr)NULL;
	xmlNodePtr newSerNode = (xmlNodePtr)NULL;

	PyObject *classPtr = (PyObject *)NULL;
	PyObject *className = (PyObject *)NULL;
	PyObject *item = (PyObject *)NULL;
	long tupleSize = 0;
	long counter = 0;
	char *nptr = (char *)NULL;
	if (PYTHON_IS_NONE(o)) {
		return (xmlNodePtr)NULL;
	}
	classPtr = PyObject_GetAttrString(o, pyxser_attr_class);
	if (PYTHON_IS_NONE(classPtr)) {
        PyErr_Clear();
		return (xmlNodePtr)NULL;
	}
	className = PyObject_GetAttrString(classPtr, pyxser_attr_name);
	if (PYTHON_IS_NONE(className)) {
        PyErr_Clear();
        PYXSER_FREE_OBJECT(classPtr);
		return (xmlNodePtr)NULL;
	}
	nptr = PyString_AS_STRING(className);
	if (nptr != (char *)NULL) {
		newElementNode = xmlNewDocNode(docPtr, pyxser_GetDefaultNs(),
									   BAD_CAST pyxser_xml_element_collection,
									   NULL);
		typeAttr = xmlNewProp(newElementNode,
							  BAD_CAST pyxser_xml_attr_type,
							  BAD_CAST nptr);
		namePtr = xmlNewProp(newElementNode,
							 BAD_CAST pyxser_xml_attr_name,
							 BAD_CAST name);
		tupleSize = PyTuple_GET_SIZE(o);
		for (counter = 0; counter < tupleSize; counter++) {
			item = PyTuple_GetItem(o, counter);
			if (PYTHON_IS_NONE(item)) {
				continue;
			}
            args->o = &item;
            args->item = &item;
            args->currentNode = &newElementNode;

			newSerNode = pyxser_RunSerializationCol(args);

            args->o = oold;
            args->item = oold;
            args->currentNode = currentNodeOld;
            args->rootNode = rootNodeOld;
		}
	}
    PYXSER_FREE_OBJECT(className);
    PYXSER_FREE_OBJECT(classPtr);
	return newElementNode;
}

static xmlNodePtr
pyxser_GlobalDictSerialization(PyxSerializationArgsPtr args)
{
    xmlDocPtr docPtr = *args->docptr;
    xmlNodePtr *currentNodeOld = args->currentNode;
    xmlNodePtr *rootNodeOld = args->currentNode;
	xmlNodePtr newElementNode = (xmlNodePtr)NULL;
	xmlAttrPtr typeAttr = (xmlAttrPtr)NULL;
	xmlAttrPtr namePtr = (xmlAttrPtr)NULL;
	xmlNodePtr newSerNode = (xmlNodePtr)NULL;

    PyObject *o = *args->o;
    PyObject **oold = args->o;
	PyObject *classPtr = (PyObject *)NULL;
	PyObject *className = (PyObject *)NULL;
	PyObject *item = (PyObject *)NULL;
	PyObject *dictKeys = (PyObject *)NULL;
	PyObject *currentKey = (PyObject *)NULL;

    char *name = args->name;
	long tupleSize = 0;
	long counter = 0, d = 0;
	char *nptr = (char *)NULL;

	if (PYTHON_IS_NONE(o)) {
		return (xmlNodePtr)NULL;
	}
	classPtr = PyObject_GetAttrString(o, pyxser_attr_class);
	if (PYTHON_IS_NONE(classPtr)) {
        PyErr_Clear();
		return (xmlNodePtr)NULL;
	}
	className = PyObject_GetAttrString(classPtr, pyxser_attr_name);
	if (PYTHON_IS_NONE(className)) {
        PyErr_Clear();
        PYXSER_FREE_OBJECT(classPtr);
		return (xmlNodePtr)NULL;
	}
	nptr = PyString_AS_STRING(className);
	if (nptr != (char *)NULL) {
		newElementNode = xmlNewDocNode(docPtr, pyxser_GetDefaultNs(),
									   BAD_CAST pyxser_xml_element_collection,
									   NULL);
		typeAttr = xmlNewProp(newElementNode,
							  BAD_CAST pyxser_xml_attr_type,
							  BAD_CAST nptr);
		namePtr = xmlNewProp(newElementNode,
							 BAD_CAST pyxser_xml_attr_name,
							 BAD_CAST name);
		tupleSize = PyDict_Size(o);
		dictKeys = PyDict_Keys(o);
		if (PYTHON_IS_NOT_NONE(dictKeys)) {
			for (counter = 0; counter < tupleSize; counter++) {
				currentKey = PyList_GetItem(dictKeys, counter);
				item = PyDict_GetItem(o, currentKey);
				if (PYTHON_IS_NONE(item)) {
                    Py_XDECREF(item);
					continue;
				}
				d = 0;

                args->o = &item;
                args->item = &item;
                args->currentNode = &newElementNode;
                args->ck = &currentKey;

				newSerNode = pyxser_RunSerializationCol(args);

                args->o = oold;
                args->item = oold;
                args->currentNode = currentNodeOld;
                args->rootNode = rootNodeOld;

			}
            PYXSER_FREE_OBJECT(dictKeys);
		}
	}
    PYXSER_FREE_OBJECT(className);
    PYXSER_FREE_OBJECT(classPtr);
	return newElementNode;
}

PyObject *
pyxunser_SerializeTuple(PyxSerDeserializationArgsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr ron = (xmlNodePtr)NULL;
	xmlNodePtr cacheNode = (xmlNodePtr)NULL;
	xmlNodePtr cacheCurrentNode = (xmlNodePtr)NULL;

	PyDictObject **dups = obj->dups;
	PyDictObject **modules = obj->modules;
    PyObject *ndict = (PyObject *)NULL;
	PyObject *currentType = (PyObject *)NULL;
	PythonTypeDeserialize *machine = (PythonTypeDeserialize *)unserxConcreteTypes;
	PyObject *tuple = (PyObject *)NULL;
	PyObject *unser = (PyObject *)NULL;
	PyObject *cacheCurrent = (PyObject *)NULL;
	Py_ssize_t tsz = 0;

	char *n_type = (char *)NULL;
	char *n_module = (char *)NULL;
	char *n_id = (char *)NULL;
	char *n_ref = (char *)NULL;
	int c = 0;
	int lenItem = strlen((char *)pyxser_xml_element_prop);
	int lenObject = strlen((char *)pyxser_xml_element_object);

	if (node == (xmlNodePtr)NULL) {
		return Py_None;
	}

	tuple = PyTuple_New(0);
	for (ron = node->children; ron; ron = ron->next) {
		*(obj->currentNode) = ron;
		if (ron->type == XML_ELEMENT_NODE) {
			if ((strncmp((char *)ron->name,
						 (char *)pyxser_xml_element_prop,
						 lenItem)) == 0) {
				c = 0;
				while (machine[c].available == 1) {
					if ((machine[c].check(ron)) == 1) {
						unser = machine[c].deserializer(obj);
						if (PYTHON_IS_NOT_NONE(unser)) {
							++tsz;
							_PyTuple_Resize(&tuple, tsz);
							PyTuple_SET_ITEM(tuple, (tsz - 1), unser);
							break;
						}
					}
					c++;
				}
			} else if ((strncmp((char *)ron->name,
								(char *)pyxser_xml_element_object,
								lenObject)) == 0) {
				cacheNode = ron;
				n_type = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_type);
				n_module = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_module);
				n_id = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_id);
				n_ref = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_ref);
				unser = pyxser_CheckAvailableObject((PyObject *)*dups, (char *)n_ref);
				if (n_type != (char *)NULL
					&& n_module != (char *)NULL
					&& n_id != (char *)NULL
					&& unser == (PyObject *)NULL) {
					if ((pyxser_ModuleNotMain(n_module)) == 1) {
                        currentType = pyxser_SearchTypesInModules(n_module,
                                                                  n_type,
                                                                  modules);
                        if (PYTHON_IS_NOT_NONE(currentType)) {
                            ndict = PyDict_New();
                            unser = PyInstance_NewRaw(currentType, ndict);
                            Py_XDECREF(ndict);
                            if (PYTHON_IS_NOT_NONE(unser)) {
                                ++tsz;
                                _PyTuple_Resize(&tuple, tsz);
                                PyTuple_SET_ITEM(tuple, (tsz - 1), unser);
                                pyxser_AddAvailableObject((PyObject *)*dups, (char *)n_id, unser);
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
					} else {
						currentType = pyxser_SearchObjectInMain(n_type);
						if (PYTHON_IS_NOT_NONE(currentType)) {
                            ndict = PyDict_New();
                            unser = PyInstance_NewRaw(currentType, ndict);
                            Py_XDECREF(ndict);
							if (PYTHON_IS_NOT_NONE(unser)) {
                                ++tsz;
                                _PyTuple_Resize(&tuple, tsz);
                                PyTuple_SET_ITEM(tuple, (tsz - 1), unser);
                                pyxser_AddAvailableObject((PyObject *)*dups, (char *)n_id, unser);
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
                    PYXSER_XMLFREE(n_type);
                    PYXSER_XMLFREE(n_module);
                    PYXSER_XMLFREE(n_id);
                    PYXSER_XMLFREE(n_ref);
				} else {
					++tsz;
					_PyTuple_Resize(&tuple, tsz);
					PyTuple_SET_ITEM(tuple, (tsz - 1), unser);
				}
			}
            PYXSER_XMLFREE(n_type);
            PYXSER_XMLFREE(n_module);
            PYXSER_XMLFREE(n_id);
            PYXSER_XMLFREE(n_ref);
		}
	}
	return tuple;
}

PyObject *
pyxunser_SerializeList(PyxSerDeserializationArgsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr ron = (xmlNodePtr)NULL;
	xmlNodePtr cacheCurrentNode = (xmlNodePtr)NULL;

	PyDictObject **dups = obj->dups;
	PyDictObject **modules = obj->modules;
	PyObject *currentType = (PyObject *)NULL;
	PythonTypeDeserialize *machine = (PythonTypeDeserialize *)unserxConcreteTypes;
	PyObject *unser = (PyObject *)NULL;
	PyObject *cacheCurrent = (PyObject *)NULL;
	PyObject *list = (PyObject *)NULL;
    PyObject *ndict = (PyObject *)NULL;

	char *n_type = (char *)NULL;
	char *n_module = (char *)NULL;
	char *n_id = (char *)NULL;
	char *n_ref = (char *)NULL;
	int c = 0;
	int lenObject = strlen((char *)pyxser_xml_element_object);

	if (node == (xmlNodePtr)NULL) {
		return Py_None;
	}

	list = PyList_New(0);
	for (ron = node->children; ron; ron = ron->next) {
		*(obj->currentNode) = ron;
		if (ron->type == XML_ELEMENT_NODE) {
			if ((strncmp((char *)ron->name,
						 (char *)pyxser_xml_element_prop,
						 strlen((char *)pyxser_xml_element_prop))) == 0) {
				c = 0;
				while (machine[c].available == 1) {
					if ((machine[c].check(ron)) == 1) {
						unser = machine[c].deserializer(obj);
						if (PYTHON_IS_NOT_NONE(unser)) {
							PyList_Append(list, unser);
                            Py_XDECREF(unser);
							break;
						}
					}
					c++;
				}
			} else if ((strncmp((char *)ron->name,
								(char *)pyxser_xml_element_object,
								lenObject)) == 0) {
				n_type = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_type);
				n_module = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_module);
				n_id = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_id);
				n_ref = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_ref);
				unser = pyxser_CheckAvailableObject((PyObject *)*dups, (char *)n_ref);
				if (n_type != (char *)NULL
					&& n_module != (char *)NULL
					&& n_id != (char *)NULL
					&& unser == (PyObject *)NULL) {
					if ((pyxser_ModuleNotMain(n_module)) == 1) {
                        currentType = pyxser_SearchTypesInModules(n_module,
                                                                  n_type,
                                                                  modules);
                        if (PYTHON_IS_NOT_NONE(currentType)) {
                            ndict = PyDict_New();
                            unser = PyInstance_NewRaw(currentType, ndict);
                            Py_XDECREF(ndict);
                            if (PYTHON_IS_NOT_NONE(unser)) {
                                PyList_Append(list, unser);
                                pyxser_AddAvailableObject((PyObject *)*dups, (char *)n_id, unser);
                                Py_XDECREF(unser);
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
					} else {
						currentType = pyxser_SearchObjectInMain(n_type);
						if (PYTHON_IS_NOT_NONE(currentType)) {
                            ndict = PyDict_New();
                            unser = PyInstance_NewRaw(currentType, ndict);
                            Py_XDECREF(ndict);
                            if (PYTHON_IS_NOT_NONE(unser)) {
                                PyList_Append(list, unser);
                                pyxser_AddAvailableObject((PyObject *)*dups, (char *)n_id, unser);
                                Py_XDECREF(unser);
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
                    PYXSER_XMLFREE(n_type);
                    PYXSER_XMLFREE(n_module);
                    PYXSER_XMLFREE(n_id);
                    PYXSER_XMLFREE(n_ref);
				} else {
					PyList_Append(list, unser);
				}
			}
            PYXSER_XMLFREE(n_type);
            PYXSER_XMLFREE(n_module);
            PYXSER_XMLFREE(n_id);
            PYXSER_XMLFREE(n_ref);
		}
	}
	return list;
}

PyObject *
pyxunser_SerializeDict(PyxSerDeserializationArgsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr ron = (xmlNodePtr)NULL;
	xmlNodePtr cacheCurrentNode = (xmlNodePtr)NULL;

	PyDictObject **dups = obj->dups;
	PyDictObject **modules = obj->modules;
	PyObject *currentType = (PyObject *)NULL;
	PythonTypeDeserialize *machine = (PythonTypeDeserialize *)unserxConcreteTypes;
	PyObject *unser = (PyObject *)NULL;
	PyObject *cacheCurrent = (PyObject *)NULL;
	PyObject *dict = (PyObject *)NULL;
	PyObject *ndict = (PyObject *)NULL;

	int c = 0;
	int lenObject = strlen((char *)pyxser_xml_element_object);
	char *key = (char *)NULL;
	char *n_type = (char *)NULL;
	char *n_module = (char *)NULL;
	char *n_id = (char *)NULL;
	char *n_ref = (char *)NULL;

	if (node == (xmlNodePtr)NULL) {
		return Py_None;
	}

	dict = PyDict_New();
	for (ron = node->children; ron; ron = ron->next) {
		*(obj->currentNode) = ron;
		if (ron->type == XML_ELEMENT_NODE) {
			if ((strncmp((char *)ron->name,
						 (char *)pyxser_xml_element_prop,
						 strlen((char *)pyxser_xml_element_prop)))	== 0) {
				c = 0;
				while (machine[c].available == 1) {
					if ((machine[c].check(ron))	== 1) {
						unser = machine[c].deserializer(obj);
						if (PYTHON_IS_NOT_NONE(unser)) {
							key = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_name);
							PyDict_SetItemString(dict, key, unser);
                            PYXSER_XMLFREE(key);
                            Py_XDECREF(unser);
							break;
						}
					}
					c++;
				}
			} else if ((strncmp((char *)ron->name,
								(char *)pyxser_xml_element_object,
								lenObject)) == 0) {
				n_type = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_type);
				n_module = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_module);
				n_id = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_id);
				n_ref = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_ref);
				unser = pyxser_CheckAvailableObject((PyObject *)*dups, (char *)n_ref);
				if (n_type != (char *)NULL
					&& n_module != (char *)NULL
					&& n_id != (char *)NULL
					&& unser == (PyObject *)NULL) {
					if ((pyxser_ModuleNotMain(n_module)) == 1) {
                        currentType = pyxser_SearchTypesInModules(n_module,
                                                                  n_type,
                                                                  modules);
                        if (PYTHON_IS_NOT_NONE(currentType)) {
                            ndict = PyDict_New();
                            unser = PyInstance_NewRaw(currentType, ndict);
                            Py_XDECREF(ndict);
                            if (PYTHON_IS_NOT_NONE(unser)) {
                                key = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_name);
                                PyDict_SetItemString(dict, key, unser);
                                PYXSER_XMLFREE(key);
                                pyxser_AddAvailableObject((PyObject *)*dups, (char *)n_id, unser);
                                Py_XDECREF(unser);
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
					} else {
						currentType = pyxser_SearchObjectInMain(n_type);
						if (PYTHON_IS_NOT_NONE(currentType)) {
                            ndict = PyDict_New();
                            unser = PyInstance_NewRaw(currentType, ndict);
                            Py_XDECREF(ndict);
                            if (PYTHON_IS_NOT_NONE(unser)) {
                                key = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_name);
                                PyDict_SetItemString(dict, key, unser);
                                PYXSER_XMLFREE(key);
                                pyxser_AddAvailableObject((PyObject *)*dups, (char *)n_id, unser);
                                Py_XDECREF(unser);
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
                    PYXSER_XMLFREE(n_type);
                    PYXSER_XMLFREE(n_module);
                    PYXSER_XMLFREE(n_id);
                    PYXSER_XMLFREE(n_ref);
				} else {
					key = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_name);
                    if (PYTHON_IS_NOT_NONE(unser)
                        && key != (char *)NULL) {
                        PyDict_SetItemString(dict, key, unser);
                    }
				}
                PYXSER_XMLFREE(n_type);
                PYXSER_XMLFREE(n_module);
                PYXSER_XMLFREE(n_id);
                PYXSER_XMLFREE(n_ref);
			}
		}
	}
	return dict;
}

/* pyserx_collections.c ends here */
