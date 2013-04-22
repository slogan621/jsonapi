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

#include "jsonapi.h"
#include "jsonobj.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

/**
 * Get the JSONAPI singleton.
 *
 * @return JSONAPI singleton, or null if unable to allocate.
 */
 
JSONAPI *
JSONAPI::GetInstance()
{
    static JSONAPI *m_instance = NULL;

    if (!m_instance) {
        m_instance = new JSONAPI;
    }
    return m_instance;
}


/**
 * Given a parsed JSON input, get the object/array in a form compatible
 * with JSONAPI.
 *
 * @param[in] parse a parsed JSON string/input.
 * 
 * @return a JSONValue pointer representing the object/array, or NULL.
 */

JSONValue *
JSONAPI::GetRootObject(JsonParse *parse)
{
    JSONValue *val = (JSONValue *) NULL;

    JsonNode *node = parse->GetRoot();
    if (node) {
        val = JSONAPI::GetInstance()->ToJsonValue(node);
    }
    return val;
}


/**
 * Constructor. Set the appropriate type.
 */

JSONObject::JSONObject() 
{
    m_type = JsonType_Object;
}


/**
 * Convert a JSON object to it's string version. Concatenate the
 * result to the passed in string reference.
 *
 * @param[in] str the currently formed JSON string
 *
 * @return the JSON string object as a string, concatenated to str.
 */

std::string &
JSONObject::ToJSON(std::string &str)
{       
    std::string ret;
    ret += "{";
    for (int i = 0; i < GetSize(); i++) {
        Get(i)->ToJSON(ret);
        if (GetSize() > 1 && i != GetSize() - 1) {
            ret += ",";
        }
    }
    ret += "}";
    str += ret;
    return str;
}


/**
 * Constructor. Set the appropriate type.
 */

JSONArray::JSONArray() 
{
    m_type = JsonType_Array;
}


/**
 * Convert a JSON array to it's string version. Concatenate the
 * result to the passed in string reference.
 *
 * @param[in] str the currently formed JSON string
 *
 * @return the JSON array as a string, concatenated to str.
 */

std::string &
JSONArray::ToJSON(std::string &str)
{       
    std::string ret = "[";
    for (int i = 0; i < GetSize(); i++) {
        ret = Get(i)->ToJSON(ret);
        if (GetSize() > 1 && i != GetSize() - 1) {
            ret += ",";
        }
    }
    ret += "]";
    str += ret;
    return str;
}


/**
 * Constructor. Set the appropriate type.
 */

JSONString::JSONString(std::string &str) 
{
    m_type = JsonType_String;
    m_value = str;
}


/**
 * Constructor. Set the appropriate type.
 */

JSONString::JSONString(const char *str) 
{
    m_type = JsonType_String;
    m_value = str; 
}


/**
 * Return the value of the string object with escapes processed.
 *
 * For example, "Hello\nWorld" --> "Hello\\nWorld"
 *
 * @note No support for unicode (\u) in this version.
 *
 * @return the JSON string with escapes processed.
 */

std::string
JSONString::ProcessEscapes(std::string &in)
{       
    int len = in.size();
    std::string tmp("");

    if (len == 0) {
        return tmp;
    }

    char *buf = (char *) malloc(len * 2 + 1); 
    char *buf2 = (char *) malloc(len + 1);
    memset(buf2, '\0', len + 1); 
    strncpy(buf2, in.c_str(), len); 
    char *p = buf;
    char *q = buf2;
    int i = 0;
    while(i < len) {
        if (*q == '\\') {
            *p++ = '\\';
            if (i != len - 1 && *(q + 1) == 'u') {
                *p++ = 'u';
                q++;
            }
        } else if (*q == '\f') {
            *p++ = '\\';
            *p++ = 'f';
        } else if (*q == '\n') {
            *p++ = '\\';
            *p++ = 'n';
        } else if (*q == '\r') {
            *p++ = '\\';
            *p++ = 'r';
        } else if (*q == '\t') {
            *p++ = '\\';
            *p++ = 't';
        } else if (*q == '\b') {
            *p++ = '\\';
            *p++ = 'b';
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
}


extern "C" {
char *FromUTF8ToStr(char *u, int *len);
char *FromStrToUTF8(char *p, int *lenout);
int IsMultibyteUTF8(char *p, int *lenout);
}

/**
 * Given a UTF-8 byte buffer, store as std::string. Any multi-byte 
 * UTF-8 sequences are converted to \uxxxx format. If such UTF-8
 * sequences are > 3 bytes in length, an error is returned.
 *
 * @param[in] str byte stream to be converted to a UTF-8 string.
 *            We assume the string is NULL terminated (NULL is 
 *            valid UTF-8, after all).
 * 
 * @return true on success, false on failure (unsupported UTF-8 len)
 *         or some other failure.
 */

bool 
JSONString::SetAsUTF8(const char *str)
{
    char *buf = NULL;
    char *p = const_cast<char *>(str);
    char *q;
    bool ret = false;
    int curLen;
    int len;

    if (!p) {
        goto out;
    }

    curLen = 0;
    buf = NULL;
    q = buf; 

    while (*p) {
        char *u;
        u = FromUTF8ToStr(p, &len);
        if (u != (char *) NULL) {
            char *t;
            // allocate a new buffer
            t = (char *) malloc(curLen + len);
            len--;
            // copy old buffer over
            if (buf) {
                memcpy(t, buf, curLen);
            }
            // copy the UTF-8 byte stream
            memcpy(t + curLen, u, len);
            // new buffer len += size of byte stream
            curLen += len;
            // point q at next insertion point
            q = t + curLen;
            // free the old buffer, point at new one
            if (buf) {
                free(buf);
            }
            buf = t;
            p+=len;
        }
        else {
            goto out;
        }
    }
    if (q) {
        *q = '\0';
        ret = true;
        Set(buf);
        free(buf);
    }
out:
    return ret;
}


/**
 * Get string as a UTF-8 byte stream. Any portions of the string
 * that are in the form of \\uxxxx will be converted to UTF-8 
 * bytes in the returned result.
 * 
 * @return UTF-8 byte stream, NULL terminated. NULL on failure.
 * 
 */

char *
JSONString::GetAsUTF8()
{
    char *ret = NULL;
    char *p;
    char *q;
    char *u;
    int len;
    std::string value;

    /* any \uabcd will be at most 3 bytes in size, so we can allocate
       the entire buffer now and know we have enough space. */

    value = Get();
    p = const_cast<char *>(value.c_str());
    if (!p) {
        goto out;
    }

    q = ret = (char *) malloc(Get().length() + 1);
    if (!ret) {
        goto out;
    }

    while (*p) {
        if (*p == '\\' && *(p+1) == 'u') {
            p += 2;
            u = FromStrToUTF8(p, &len);
            if (!u) {
                free(ret);
                ret = NULL;
                goto out;
            } else {
                // copy to the buffer
                memcpy(q, u, len);
                q += len;
                // skip the 4 bytes of the UTF-8 portion
                p += 4;
            }            
        } else {
            // nothing special, just copy
            *q++ = *p++;
        }
    }
    *q = '\0';
out: 
    return ret;
}


/**
 * Given a string, convert any multibyte UTF-8 sequences to 
 * form \\uabcd. If the UTF-8 is 1 byte encoding, just leave
 * it as is, because ascii is same as UTF-8. 
 * 
 * @return string with multibyte UTF-8 encoded as \\uabcd, NULL 
           terminated. NULL on failure.
 * 
 */

char *
JSONString::ConvertUTF8Multibyte()
{
    char *ret = NULL;
    char *p;
    char *q;
    char *u;
    int len;
    int skiplen;
    std::string value;

    value = Get();
    p = const_cast<char *>(value.c_str());
    if (!p) {
        goto out;
    }

    /* any \uabcd will be at most 7 bytes in size, so we can allocate
       the entire buffer now worse case size and know we have enough 
       space. */

    q = ret = (char *) malloc(Get().length() * 7);
    if (!ret) {
        goto out;
    }

    while (*p) {
        if (IsMultibyteUTF8(p, &skiplen)) {
            u = FromUTF8ToStr(p, &len);
            if (!u) {
                free(ret);
                ret = NULL;
                goto out;
            } else {
                // copy to the buffer
                len -= 1;
                memcpy(q, u, len);
                q += len;
                // skip the UTF-8 portion
                p += skiplen;
            }            
        } else {
            // nothing special, just copy
            *q++ = *p++;
        }
    }
    *q = '\0';
out: 
    return ret;
}


/**
 * Convert a JSON string object to it's string version. Concatenate the
 * result to the passed in string reference.
 *
 * @param[in] str the currently formed JSON string
 *
 * @return the JSON string object as a string, concatenated to str.
 */

std::string &
JSONString::ToJSON(std::string &str)
{       
    std::string ret;

    str += std::string("\"") + ConvertUTF8Multibyte() + std::string("\"");
    return str;
}


/**
 * Constructor. Set the appropriate type.
 */

JSONNumber::JSONNumber() 
{
    m_type = JsonType_Number;
}


/**
 * Constructor. Set the appropriate type.
 */

JSONNumber::JSONNumber(long number) 
{
    m_type = JsonType_Number;
    m_value = number;
}


/**
 * Convert a JSON number object to it's string version. Concatenate the
 * result to the passed in string reference.
 *
 * @param[in] str the currently formed JSON string
 *
 * @return the JSON string object as a string, concatenated to str.
 */

std::string &
JSONNumber::ToJSON(std::string &str)
{       
    char buf[128];

    snprintf(buf, sizeof buf - 1, "%ld", Get());
    std::string ret(buf); 
    str += ret;
    return str;
}


/**
 * Constructor. Set the appropriate type.
 */

JSONDouble::JSONDouble() 
{
    m_type = JsonType_Double;
}


/**
 * Constructor. Set the appropriate type.
 */

JSONDouble::JSONDouble(double val) 
{
    m_type = JsonType_Double;
    m_value = val;
}


/**
 * Convert a JSON double to it's string version. Concatenate the
 * result to the passed in string reference.
 *
 * @param[in] str the currently formed JSON string
 *
 * @return the JSON string object as a string, concatenated to str.
 */

std::string &
JSONDouble::ToJSON(std::string &str)
{       
    char buf[128];

    snprintf(buf, sizeof buf - 1, "%f", Get());
    std::string ret(buf); 
    str += ret;
    return str;
}


/**
 * Constructor. Set the appropriate type.
 */

JSONBoolean::JSONBoolean() 
{
    m_type = JsonType_Bool;
}


/**
 * Constructor. Set the appropriate type.
 */

JSONBoolean::JSONBoolean(bool val) 
{
    m_type = JsonType_Bool;
    m_value = val;
}


/**
 * Convert a JSON boolean to it's string version. Concatenate the
 * result to the passed in string reference.
 *
 * @param[in] str the currently formed JSON string
 *
 * @return the JSON string object as a string, concatenated to str.
 */

std::string &
JSONBoolean::ToJSON(std::string &str)
{       
    std::string ret;

    ret = (Get() == true ? "true" : "false");
    str += ret;
    return str;
}


/**
 * Constructor. Set the appropriate type.
 */

JSONTuple::JSONTuple() 
{
    m_type = JsonType_Tuple;
}


/**
 * Convert a JSON tuple to it's string version. Concatenate the
 * result to the passed in string reference.
 *
 * @param[in] str the currently formed JSON string
 *
 * @return the JSON string object as a string, concatenated to str.
 */

std::string &
JSONTuple::ToJSON(std::string &str)
{       
    std::string ret = "\"";
    ret += GetKey();
    ret += "\": ";
    GetValue()->ToJSON(ret);
    str += ret;
    return str;
}


/**
 * Constructor. Set the appropriate type.
 */

JSONNull::JSONNull() 
{
    m_type = JsonType_Null;
}


/**
 * Convert a JSON null to it's string version. Concatenate the
 * result to the passed in string reference.
 *
 * @param[in] str the currently formed JSON string
 *
 * @return the JSON string object as a string, concatenated to str.
 */

std::string &
JSONNull::ToJSON(std::string &str)
{       
    str += "null";
    return str;
}


/**
 * Get the element of an object or array at specified offset.
 *
 * @param[in] offset the offset in range [0, size - 1]
 *
 * @return on success, JSONValue pointer, otherwise NULL. 
 */

JSONValue *
JSONValue::Get(int offset)
{
    std::list<JSONValue *>::iterator iter;
    JSONValue *p = NULL;
    int i;

    for (i = 0,iter = m_elements.begin(); 
         iter != m_elements.end(); ++iter,i++) {
        if (i == offset) {
            p = (*iter);
            break; 
        }
    }
    return p;
}


/**
 * Store the JSONValue at the specified offset in the object or array.
 *
 * @param[in] offset the position to store value at.
 * @param[in] the value to store.
 *
 * @return on success true, otherwise false.
 */

bool
JSONValue::Set(int offset, JSONValue *val)
{
    bool ret = true;

    if (offset < 0 || offset >= (int) m_elements.size()) {
        ret = false;
        goto out;
    }
    Delete(offset);
    Insert(offset, val);
out:
    return ret;
}


/**
 * Insert the value at the specified offset in the object or array.
 *
 * @param[in] offset the offset of the item after insertion.
 * @param[in] val the value to insert.
 *
 * @return true on success, false on failure
 */

bool
JSONValue::Insert(int offset, JSONValue *val)
{
    bool ret = true;
    std::list<JSONValue *>::iterator iter;
    int i;

    if (offset < 0 || offset >= (int) m_elements.size()) {
        ret = false;
        goto out;
    }
    if (offset == 0) {
        Prepend(val);
    } else if (offset == (int) m_elements.size() - 1) {
        Append(val);
    } else {
        for (i = 0,iter = m_elements.begin(); 
            iter != m_elements.end(); ++iter,i++) {
            if (i == offset) {
                break; 
            }
        }
    }
    m_elements.insert(iter, val);
out:
    return ret;
}


/** 
 * Delete item at specified offset from the object or array. The item will
 * be destroyed on removal. 
 *
 * @param[in] offset the offset of the item to be removed. 
 *
 * @return true on success, false on failure.
 */

bool
JSONValue::Delete(int offset)
{
    bool ret = true;

    if (offset < 0 || offset >= (int) m_elements.size()) {
        ret = false;
        goto out;
    }

    if (offset == 0) {
        m_elements.pop_front();
    } else if (offset == (int) m_elements.size() - 1) {
        m_elements.pop_back();
    } else {
        std::list<JSONValue *>::iterator iter;
        int i;

        for (i = 0,iter = m_elements.begin(); 
            iter != m_elements.end(); ++iter,i++) {
            if (i == offset) {
                m_elements.erase(iter);
                break; 
            }
        }
    }
out:
    return ret;
}


/**
 * Add specified element at offset 0 in the object or array.
 *
 * @param[in] val the value to insert.
 */

void
JSONValue::Prepend(JSONValue *val)
{
    m_elements.push_front(val);
}


/**
 * Add the specified element to the end of the object or array.
 *
 * @param[in] val the value to insert.
 */

void
JSONValue::Append(JSONValue *val)
{
    m_elements.push_back(val);
}


/**
 * Default implementation of ToJSON (derived classes should override).
 *
 * @param[in] str current value of JSON object in JSON format.
 * 
 * @return str + "Not implemented"
 */

std::string &
JSONValue::ToJSON(std::string &str)
{       
    str += "Not Implemented";
    return str;
}


/**
 * Destroy a value. If the value is an object or array, destroy all of 
 * its elements, too.
 */

JSONValue::~JSONValue()
{
    std::list<JSONValue *>::iterator iter;

    for (iter = m_elements.begin(); iter != m_elements.end(); ++iter) {
        delete *iter;
    }
    m_elements.clear();
}


/**
 * Convert an internal JsonNode to a api JSONValue object.
 *
 * @param[in] node the internal node to convert.
 * 
 * @return on success, pointer to a JSONValue object (caller must delete)
 *         on failure, NULL.
 */

JSONValue *
JSONAPI::ToJsonValue(JsonNode *node)
{
    JsonType type = node->GetType();
    JSONValue *val = (JSONValue *) NULL;

    switch(type) {
    case JsonType_Object:
        {
        JSONObject *jsonobj = new JSONObject();
        std::list<JsonNode *> children = node->GetChildren();
        std::list<JsonNode *>::iterator iter;

        for (iter = children.begin(); iter != children.end(); ++iter) {
            jsonobj->Append(ToJsonValue(*iter));
        }
        val = static_cast<JSONValue *>(jsonobj);
        }
        break;
    case JsonType_Array:
        {
        JSONArray *jsonarray = new JSONArray();
        std::list<JsonNode *> children = node->GetChildren();
        std::list<JsonNode *>::iterator iter;

        for (iter = children.begin(); iter != children.end(); ++iter) {
            jsonarray->Append(ToJsonValue(*iter));
        }
        val = static_cast<JSONValue *>(jsonarray);
        }
        break;
    case JsonType_String:
        {
        std::string str;
        JSONString *jsonstr = new JSONString();
        static_cast<JsonValue *>(node)->GetValue(str);
        jsonstr->Set(str);
        val = static_cast<JSONValue *>(jsonstr);
        }
        break;
    case JsonType_Number:
        {
        long lval;
        JSONNumber *jsonnum = new JSONNumber();
        static_cast<JsonValue *>(node)->GetValue(lval);
        jsonnum->Set(lval);
        val = static_cast<JSONValue *>(jsonnum);
        }
        break;
    case JsonType_Double:
        {
        double dval;
        JSONDouble *jsondbl = new JSONDouble();
        static_cast<JsonValue *>(node)->GetValue(dval);
        jsondbl->Set(dval);
        val = static_cast<JSONValue *>(jsondbl);
        }
        break;
    case JsonType_Bool:
        {
        bool bval;
        JSONBoolean *jsonbool = new JSONBoolean();
        static_cast<JsonValue *>(node)->GetValue(bval);
        jsonbool->Set(bval);
        val = static_cast<JSONValue *>(jsonbool);
        }
        break;
    case JsonType_Tuple:
        {
        JsonTuple *tval = static_cast<JsonTuple *>(node);
        JSONTuple *jsontuple = new JSONTuple();
        std::string key;

        key = tval->GetKey();
        jsontuple->SetKey(key);
        jsontuple->SetValue(ToJsonValue(tval->GetKeyValue()));
        val = static_cast<JSONValue *>(jsontuple);
        }
        break;
    case JsonType_Null:
        {
        JSONNull *jsonnull = new JSONNull();
        val = static_cast<JSONValue *>(jsonnull);
        }
        break;
    default:
        break;
    }

    return val;
}
