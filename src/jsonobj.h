#if !defined(__JSONOBJ_H__)
#define __JSONOBJ_H__

/*
jsonapi - c++ JSON parser

Copyright (C) 2012  Syd Logan

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
USA.

Copyright (c) 2012, Syd Logan
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <string>
#include <list>

/**
 * Enums for the various JSON types, plus a couple of types for
 * internal use (e.g., root node of DOM).
 *
 * The code in this file is intended mainly for internal use by
 * the parser. For classes that are intended to be used by apps,
 * see jsonapi.h
 */

typedef enum {
    JsonType_Object,
    JsonType_Array,
    JsonType_String,
    JsonType_Number,
    JsonType_Double,
    JsonType_Bool,
    JsonType_Root,
    JsonType_Tuple,
    JsonType_Null
} JsonType;

/**
 * Base class for all Json values. Basically manages the type of the
 * value, and its list of children. Arrays and Objects both have 
 * children, in the case of Arrays, children are values. In the case
 * of Objects, children consist of tuples. Tuples have a string key,
 * and a value (which could itself be an object, or an array). 
 */

class JsonNode
{
public:
    JsonNode();
    virtual ~JsonNode();
    void AddChild(JsonNode *node) {node->SetParent(this); m_children.push_back(node);}
    void SetParent(JsonNode *node) {m_parent = node;}
    JsonNode *GetParent() {return m_parent;}
    int GetNumChildren() {return m_children.size();}
    std::list<JsonNode *>& GetChildren() {return m_children;};
    void SetType(JsonType type) {m_type = type;}
    JsonType GetType() {return m_type;}
    void DeleteChildren();
private:
    JsonType m_type;
    JsonNode *m_parent;
    std::list<JsonNode *> m_children;
};


class JsonTuple;

/**
 * Class to represent a JSON value. Mainly just setters and 
 * getters, and code to serialize to string. The class is 
 * almost like a union or a variant type, but not quite.
 */

class JsonValue : public JsonNode 
{
public:
    JsonValue() {}
    JsonValue(bool val) {SetValue(val);}
    JsonValue(long val) {SetValue(val);}
    JsonValue(double val) {SetValue(val);}
    JsonValue(char *val) {SetValue(val);}
    JsonValue(std::string &val) {SetValue(val);}
    JsonValue(JsonTuple *val) {SetValue(val);}
    void SetValue(bool val);
    void SetValue(long val);
    void SetValue(double val);
    void SetValue(char *val);
    void SetValue(std::string &val);
    bool GetValue(bool &val);
    bool GetValue(long &val);
    bool GetValue(double &val);
    bool GetValue(std::string &val);
    std::string ToJson(std::string &str);
private:
    std::string DumpChildren(std::string &str);
    bool m_boolVal;
    double m_dblVal;
    long m_longVal;
    std::string m_strVal;
};


/** 
 * Class used to represent tuples (i.e., objects.) 
 * Since tuples are a more complex, specialized value,
 * they derive from JsonValue.
 */

class JsonTuple : public JsonValue
{
public:
    JsonTuple(JsonValue *key, JsonValue *val)
    {
        SetType(JsonType_Tuple);
        m_key = key; 
        m_val = val;
    }
    ~JsonTuple() {delete m_key; delete m_val;}
    std::string GetKey() {std::string ret; m_key->GetValue(ret); return ret;}
    JsonValue *GetKeyValue() {return m_val;}
    void SetKeyValue(JsonValue *val) {m_val = val;}
private:
    JsonValue *m_key;
    JsonValue *m_val;
};

#endif
