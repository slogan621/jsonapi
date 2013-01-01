/*
    Copyright © 2012 Syd Logan. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products
derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Syd Logan "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include "jsonobj.h"
#include <stdlib.h>
#include <memory.h>

/**
 * Constructor
 */

JsonNode::JsonNode() :
    m_parent(NULL)
{
}

/**
 * Destructor. Remove children.
 */

JsonNode::~JsonNode()
{
    DeleteChildren();
}


/**
 * Delete all children of the node.
 */

void
JsonNode::DeleteChildren()
{
    JsonNode *p;

    while (!m_children.empty()) {
        p = m_children.front();
        delete p;
        m_children.pop_front();
    }
}


/**
 * Convert all children of a node to a string, concatenating to passed in 
 * string, and returning the result.
 *
 * @param[in] str string to concat result to.
 * 
 * @return the children, as a JSON string, concatenated to the pass in str.
 *
 * @note this is not a part of the public API.
 */

std::string 
JsonValue::DumpChildren(std::string &str)
{
    std::list<JsonNode *>::iterator iter;
    int count = 0;

    for (iter = GetChildren().begin(); iter != GetChildren().end(); ++iter) {
        JsonValue *val = static_cast<JsonValue *>(*iter);
        if (count) {
            str += ", ";
        }
        if (val) {
            str = val->ToJson(str);
            count++;
        }
    }
    return str;
}


/**
 * Convert a value object to a JSON string and concat to passed in string.
 * 
 * @param[in] str the current JSON object as a string.
 *
 * @return Value as a string, concatenated to str.
 *
 * @note This function is not a part of the public API.
 */

std::string 
JsonValue::ToJson(std::string &str)
{
    char buf[128];
    JsonTuple *tup;

    switch(GetType()) {
    case JsonType_Bool:
        if (m_boolVal == true) {
            str += "true";
        } else {
            str += "false";
        }
        break;
    case JsonType_Null:
        str += "null";
        break;
    case JsonType_Double:
        sprintf(buf, "%g", m_dblVal);
        str += buf;
        break;
    case JsonType_Number:
        sprintf(buf, "%ld", m_longVal);
        str += buf;
        break;
    case JsonType_String:
        snprintf(buf, sizeof(buf), "%s", m_strVal.c_str());
        str += buf;
        break;
    case JsonType_Object:
        str += "{"; 
        str = DumpChildren(str);
        str += "}";
        break;
    case JsonType_Array:
        str += "["; 
        str = DumpChildren(str);
        str += "]";
        break;
    case JsonType_Tuple:
        tup = static_cast<JsonTuple *>(this);
        str += tup->GetKey();
        str += " : ";
        str = tup->GetKeyValue()->ToJson(str);
        break;
    case JsonType_Root:
        str = DumpChildren(str);
        break;
    } 
    return str;
}


/**
 * Set value object's value as a boolean.
 *
 * @param[in] val value as a bool
 */

void 
JsonValue::SetValue(bool val)
{
    m_boolVal = val;
    SetType(JsonType_Bool);
}


/**
 * Set value object's value as a long (Number).
 *
 * @param[in] val value as a long
 */

void 
JsonValue::SetValue(long val)
{
    m_longVal = val;
    SetType(JsonType_Number);
}


/**
 * Set value object's value as a double.
 *
 * @param[in] val value as a double
 */

void 
JsonValue::SetValue(double val) 
{
    m_dblVal = val;
    SetType(JsonType_Double);
}


/**
 * Set value object's value as a string.
 *
 * @param[in] val value as a string
 */

void 
JsonValue::SetValue(char *val)
{
    std::string str(val);
    m_strVal = ProcessEscapes(str);
    SetType(JsonType_String);
}


/**
 * Set value object's value as a string.
 *
 * @param[in] val value as a string
 */

void 
JsonValue::SetValue(std::string &val)
{
    m_strVal = ProcessEscapes(val);
    SetType(JsonType_String);
}


std::string
JsonValue::ProcessEscapes(std::string &in)
{
    return in;

#if 0
    int len = in.size();
    std::string tmp("");
    if (len == 0) {
        return tmp;
    }
    char *buf = (char *) malloc(len * 2 + 1);
    char *buf2 = (char *) malloc(len + 1);

    memcpy(buf2, in.c_str(), len);
    char *p = buf;
    char *q = buf2;
    int i = 0;
    while(i < len) {
        if (*q == '\\' && i < len - 1 && *(q+1) != '\\') {
            *p++ = '\\';
            *p++ = '\\';
            *p++ = '\\';
        } else if (*q == '\b') {
            *p++ = '\\';
            *p++ = '\\';
            *p++ = 'b';
        } else if (*q == '\f') {
            *p++ = '\\';
            *p++ = '\\';
            *p++ = 'f';
        } else if (*q == '\n') {
            *p++ = '\\';
            *p++ = '\\';
            *p++ = 'n';
        } else if (*q == '\t') {
            *p++ = '\\';
            *p++ = '\\';
            *p++ = 't';
        } else {
            *p++ = *q;
        }
        q++;
        i++;
    }
    *p = '\0';
    tmp = buf;
    free(buf);
    free(buf2);
    return tmp;
#endif
}


/**
 * Get value object's value as a boolean.
 *
 * @param[out] val value as a bool
 *
 * @return true on success, false on failure.
 */

bool 
JsonValue::GetValue(bool &val)
{
    bool ret = false;
    if (GetType() == JsonType_Bool) {
        ret = true;
        val = m_boolVal;
    }
    return ret;
}


/**
 * Get value object's value as a long (Number).
 *
 * @param[out] val value as a long
 *
 * @return true on success, false on failure.
 */

bool 
JsonValue::GetValue(long &val)
{
    bool ret = false;
    if (GetType() == JsonType_Number) {
        ret = true;
        val = m_longVal;
    }
    return ret;
}


/**
 * Get value object's value as a double.
 *
 * @param[out] val value as a double
 *
 * @return true on success, false on failure.
 */

bool 
JsonValue::GetValue(double &val)
{
    bool ret = false;
    if (GetType() == JsonType_Double) {
        ret = true;
        val = m_dblVal;
    }
    return ret;
}


/**
 * Get value object's value as a string.
 *
 * @param[out] val value as a string
 *
 * @return true on success, false on failure.
 */

bool 
JsonValue::GetValue(std::string &val)
{
    bool ret = false;
    if (GetType() == JsonType_String) {
        ret = true;
        val = m_strVal;
    }
    return ret;
}

