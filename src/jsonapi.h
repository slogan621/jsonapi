#if !defined(__JSON_API_H__)
#define __JSON_API_H__

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


#include "jsonobj.h"
#include "jsonparse.h"

/**
 * public classes for accessing values of JSON objects and arrays.
 */


class JSONValue;

/**
 * Singleton class that allows an application to get a JSONValue after 
 * a parse.
 */ 

class JSONAPI
{
public:
    static JSONAPI *GetInstance();
    static JSONValue *GetRootObject(JsonParse *parser);
    static JSONValue *GetValue(JsonParse *parser) {return GetRootObject(parser);}
private:
    JSONAPI(JSONAPI const&) {};
    JSONAPI& operator=(JSONAPI const&); 
    JSONValue *ToJsonValue(JsonNode *);
    JSONAPI() {};
};


/**
 * Base class for all JSON objects exposed by the API.
 */

class JSONValue
{
public:
    virtual ~JSONValue();
    JsonType GetType() {return m_type;}
    int GetSize() {return m_elements.size();}
    JSONValue *Get(int index);
    bool Set(int index, JSONValue *val);
    void Append(JSONValue *val);
    void Prepend(JSONValue *val);
    bool Delete(int offset);
    bool Insert(int offset, JSONValue *val);
    virtual std::string ToJSON(std::string &str);
protected:
    JsonType m_type;
private:
    std::list<JSONValue *> m_elements; // tuples in the case of JSONObject
}; 


/**
 * Class to represent a tuple (a JSON object consists of one or more tuples)
 */

class JSONTuple : public JSONValue
{
public:
    JSONTuple();
    ~JSONTuple() {delete m_value;}
    void SetKey(std::string &key) {m_key = key;}
    void SetKey(const char *key) {m_key = key;}
    std::string GetKey() {return m_key;}
    void SetValue(JSONValue *value) {m_value = value;}
    JSONValue *GetValue() {return m_value;}
    std::string ToJSON(std::string &str);
private:
    std::string m_key;
    JSONValue *m_value;
};

/**
 * Class to represent a JSON object.
 */

class JSONObject : public JSONValue
{
public:
    JSONObject();
    std::string ToJSON(std::string &str);
};


/**
 * Class to represent a JSON array.
 */

class JSONArray : public JSONValue
{
public:
    JSONArray();
    std::string ToJSON(std::string &str);
};

/**
 * Class to represent a JSON number.
 */

class JSONNumber : public JSONValue
{
public:
    JSONNumber();
    JSONNumber(long val);
    void Set(long val) {m_value = val;}
    long Get() {return m_value;}
    std::string ToJSON(std::string &str);
private:
    long m_value;
};

/**
 * Class to represent a JSON string.
 */

class JSONString : public JSONValue
{
public:
    JSONString() {m_type = JsonType_String;}
    JSONString(std::string &str);
    JSONString(const char *str);
    void Set(std::string &str) {m_value = str;}
    void Set(const char *str) {m_value = str;}
    std::string Get() {return ProcessEscapes(m_value);}
    bool SetAsUTF8(const char *str);
    char *GetAsUTF8();
    std::string ToJSON(std::string &str);
private:
    char *ConvertUTF8Multibyte();
    std::string ProcessEscapes(std::string &s);
    std::string m_value;
};


/** 
 * Class to represent a JSON double.
 */

class JSONDouble : public JSONValue
{
public:
    JSONDouble();
    JSONDouble(double val);
    void Set(double val) {m_value = val;}
    double Get() {return m_value;}
    std::string ToJSON(std::string &str);
private:
    double m_value;
};

/** 
 * Class to represent a JSON boolean.
 */

class JSONBoolean : public JSONValue
{
public:
    JSONBoolean();
    JSONBoolean(bool val);
    void Set(bool val) {m_value = val;}
    bool Get() {return m_value;}
    std::string ToJSON(std::string &str);
private:
    bool m_value;
};

/** 
 * Class to represent a JSON null.
 */

class JSONNull : public JSONValue
{
public:
    JSONNull();
    long Get() {return 0L;}
    std::string ToJSON(std::string &str);
private:
};

#endif
