#if !defined(__JSONAPITEST_H__)
#define __JSONAPITEST_H__

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

/**
 * Unit tests, examples for JSON api. Requires cppunit.
 */

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "jsonparse.h"
#include "jsonapi.h"

#include <string.h>

class ParseTest : public CppUnit::TestFixture {
public:
    CPPUNIT_TEST_SUITE( ParseTest );
    CPPUNIT_TEST( testBadInput1 );
    CPPUNIT_TEST( testBadInput2 );
    CPPUNIT_TEST( testBadInput3 );
    CPPUNIT_TEST( testBadInput4 );
    CPPUNIT_TEST( testBadInput5 );
    CPPUNIT_TEST( testNumber );
    CPPUNIT_TEST( testNull );
    CPPUNIT_TEST( testDouble );
    CPPUNIT_TEST( testBoolean );
    CPPUNIT_TEST( testString );
    CPPUNIT_TEST( testOneElementArray );
    CPPUNIT_TEST( testTwoElementArray );
    CPPUNIT_TEST( testThreeElementArray );
    CPPUNIT_TEST( testTwoElementHeteroArray );
    CPPUNIT_TEST( testThreeElementHeteroArray );
    CPPUNIT_TEST( testTwoElementNestedArray );
    CPPUNIT_TEST( testThreeElementNestedArray );
    CPPUNIT_TEST( testOneElementObject );
    CPPUNIT_TEST( testTwoElementObject );
    CPPUNIT_TEST( testThreeElementObject );
    CPPUNIT_TEST( testTwoElementNestedObject );
    CPPUNIT_TEST( testThreeElementNestedObject );
    CPPUNIT_TEST( testObjectTupleArray );
    CPPUNIT_TEST( testCreateNumber );
    CPPUNIT_TEST( testCreateNull );
    CPPUNIT_TEST( testCreateDouble );
    CPPUNIT_TEST( testCreateBoolean );
    CPPUNIT_TEST( testCreateString );
    CPPUNIT_TEST( testCreateOneElementArray );
    CPPUNIT_TEST( testCreateTwoElementArray );
    CPPUNIT_TEST( testCreateThreeElementArray );
    CPPUNIT_TEST( testCreateTwoElementHeteroArray );
    CPPUNIT_TEST( testCreateThreeElementHeteroArray );
    CPPUNIT_TEST( testCreateTwoElementNestedArray );
    CPPUNIT_TEST( testCreateThreeElementNestedArray );
    CPPUNIT_TEST( testCreateOneElementObject );
    CPPUNIT_TEST( testCreateTwoElementObject );
    CPPUNIT_TEST( testCreateThreeElementObject );
    CPPUNIT_TEST( testCreateTwoElementNestedObject );
    CPPUNIT_TEST( testCreateThreeElementNestedObject );
    CPPUNIT_TEST( testCreateUTF8 );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
        // not used
    }

    void tearDown()
    {
        // not used
    }

    void testBadInput1()
    {
        std::string str("{17");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        parser = new JsonParse();
       
        str = "17}"; 
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;
    }

    void testBadInput2()
    {
        std::string str("Now is the time for all good men to come to the aid of their country");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;
    }

    void testBadInput3()
    {
        std::string str("{\"Name\": \"First\": 1, \"Last\": 2}, \"Age\": 3, \"City\": 4}");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        parser = new JsonParse();
        str = "{\"Name\": {\"First\" 1, \"Last\": 2}, \"Age\": 3, \"City\": 4}";    
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        parser = new JsonParse();
        str = "{\"Name\": {\"First\": 1, \"Last\": }, \"Age\": 3, \"City\": 4}";    
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        parser = new JsonParse();
        str = "{\"Name\": {\"First\": 1, \"Last\": 2}, : 3, \"City\": 4}";    
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;
    }

    void testBadInput4()
    {
        std::string str("--17");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        str = "++17";    
        parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        str = "1++7";    
        parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        str = "e10";    
        parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        str = "-e10";    
        parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        str = ".e10";    
        parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;
    }

    void testBadInput5()
    {
        std::string str("\"");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        str = "%%%%%%";    
        parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        str = "\n";    
        parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        str = " ";    
        parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        str = "                  ";    
        parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        str = "cccccccccccccc";    
        parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        str = "'";    
        parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        str = "''";    
        parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;

        str = "'?*'";    
        parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == false);
        delete parser;
    }

    void testNumber()
    {
        std::string str("17");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONNumber *obj = static_cast<JSONNumber *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Number);
        long value;
        value = obj->Get();
        CPPUNIT_ASSERT(value == 17);  
        delete obj;

        str = "-17";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONNumber *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Number);
        value = obj->Get();
        CPPUNIT_ASSERT(value == -17);  
        delete obj;

        str = "+17";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONNumber *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Number);
        value = obj->Get();
        CPPUNIT_ASSERT(value == 17);  
        delete obj;

        delete parser;
    }

    void testNull()
    {
        std::string str("null");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONNull *obj = static_cast<JSONNull *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Null);
        long value;
        value = obj->Get();
        CPPUNIT_ASSERT(value == 0);  
        delete obj;
        delete parser;
    }

    void testDouble()
    {
        std::string str("17.0");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONDouble *obj = static_cast<JSONDouble *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Double);
        double value;
        value = obj->Get();
        CPPUNIT_ASSERT(value == 17.0);  
        delete obj;

        str = "-17.0";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONDouble *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Double);
        value = obj->Get();
        CPPUNIT_ASSERT(value == -17.0);  
        delete obj;

        str = "+17.0";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONDouble *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Double);
        value = obj->Get();
        CPPUNIT_ASSERT(value == 17.0);  
        delete obj;

        str = "1.2e5";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONDouble *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Double);
        value = obj->Get();
        CPPUNIT_ASSERT(value == 120000.0);  
        delete obj;

        str = "1.2e-5";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONDouble *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Double);
        value = obj->Get();
        CPPUNIT_ASSERT(value == 0.000012);  
        delete obj;

        delete parser;
    }

    void testBoolean()
    {
        std::string str("false");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONBoolean *obj = static_cast<JSONBoolean *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Bool);
        bool value;
        value = obj->Get();
        CPPUNIT_ASSERT(value == false);  
        delete obj;

        str = "true";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONBoolean *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Bool);
        value = obj->Get();
        CPPUNIT_ASSERT(value == true);  
        delete obj;

        delete parser;
    }

    void testString()
    {
        std::string str("\"Hello World\"");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONString *obj = static_cast<JSONString *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_String);
        std::string value;
        value = obj->Get();
        CPPUNIT_ASSERT(value == str);  
        delete obj;

        str = "\"\"";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONString *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_String);
        value = obj->Get();
        CPPUNIT_ASSERT(value == str);  
        delete obj;

        str = "\"Hello\\nWorld\"";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONString *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_String);
        value = obj->Get();
        CPPUNIT_ASSERT(value == "\"Hello\\nWorld\"");  
        delete obj;

        str = "\"\n\"";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONString *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_String);
        value = obj->Get();
        CPPUNIT_ASSERT(value == "\"\\n\"");  
        delete obj;

        str = "\"\\u0024\"";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONString *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_String);
        value = obj->Get();
        CPPUNIT_ASSERT(value == "\"\\u0024\"");  
        char *ret = obj->GetAsUTF8();
        CPPUNIT_ASSERT(ret);
        value = ret;
        CPPUNIT_ASSERT(value == "\"$\"");  
        CPPUNIT_ASSERT(value == "\"\u0024\"");  
        delete obj;

        str = "\"\\u00A2\"";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONString *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_String);
        value = obj->Get();
        CPPUNIT_ASSERT(value == "\"\\u00A2\"");  
        ret = obj->GetAsUTF8();
        CPPUNIT_ASSERT(ret);
        value = ret;
        CPPUNIT_ASSERT(value == "\"\u00a2\"");  
        delete obj;

        str = "\"\\u20AC\"";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONString *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_String);
        value = obj->Get();
        CPPUNIT_ASSERT(value == "\"\\u20AC\"");  
        ret = obj->GetAsUTF8();
        CPPUNIT_ASSERT(ret);
        value = ret;
        CPPUNIT_ASSERT(value == "\"\u20ac\"");  
        delete obj;

        str = "\"\\u003Cp\\u003E\"";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONString *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_String);
        value = obj->Get();
        CPPUNIT_ASSERT(value == "\"\\u003Cp\\u003E\"");  
        value = obj->GetAsUTF8();
        CPPUNIT_ASSERT(value == "\"\u003cp\u003e\"");  
        delete obj;

        str = "\"\r\n\b\"";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONString *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_String);
        value = obj->Get();
        CPPUNIT_ASSERT(value == "\"\\r\\n\\b\"");  
        delete obj;

        str = "\"Hello\nWorld\"";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONString *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_String);
        value = obj->Get();
        CPPUNIT_ASSERT(value == "\"Hello\\nWorld\"");  
        delete obj;

        str = "\"\\\"\"";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONString *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_String);
        value = obj->Get();
        CPPUNIT_ASSERT(value == "\"\\\"\"");  
        delete obj;
    }

    void testOneElementArray()
    {
        std::string str("[17]");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONArray *obj = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        JSONValue *value;
        value = obj->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 17);  
        delete obj;

        str = "[\"Hello\"]";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        value = obj->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"Hello\""); 
        delete obj;
        delete parser;
    }

    void testTwoElementArray()
    {
        std::string str("[17, 18]");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONArray *obj = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        JSONValue *value;
        value = obj->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 17);  
        value = obj->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 18);  
        delete obj;

        str = "[\"Hello\", \"World\"]";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        value = obj->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"Hello\""); 
        value = obj->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"World\""); 
        delete obj;
        delete parser;
    }

    void testThreeElementArray()
    {
        std::string str("[17, 18, 19]");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONArray *obj = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        JSONValue *value;
        value = obj->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 17);  
        value = obj->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 18);  
        value = obj->Get(2);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 19);  
        delete obj;

        str = "[\"Hello\", \"JSON\", \"World\"]";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        value = obj->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"Hello\""); 
        value = obj->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"JSON\""); 
        value = obj->Get(2);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"World\""); 
        delete obj;
        delete parser;
    }

    void testTwoElementHeteroArray()
    {
        std::string str("[17, \"Seventeen\"]");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONArray *obj = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        JSONValue *value;
        value = obj->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 17);  
        value = obj->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"Seventeen\"");
        delete obj;

        str = "[\"Hello\", true]";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        value = obj->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"Hello\""); 
        value = obj->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONBoolean *>(value)->Get() == true); 
        delete obj;
        delete parser;
    }

    void testThreeElementHeteroArray()
    {
        std::string str("[17, \"Seventeen\", true]");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONArray *obj = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        JSONValue *value;
        value = obj->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 17);  
        value = obj->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"Seventeen\"");
        value = obj->Get(2);
        CPPUNIT_ASSERT(static_cast<JSONBoolean *>(value)->Get() == true);
        delete obj;

        str = "[\"Hello\", false, 1.5e3]";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        value = obj->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"Hello\""); 
        value = obj->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONBoolean *>(value)->Get() == false); 
        value = obj->Get(2);
        CPPUNIT_ASSERT(static_cast<JSONDouble *>(value)->Get() == 1500.0); 
        delete obj;

        str = "[\"Hello\", true, null]";
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        obj = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        value = obj->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"Hello\""); 
        value = obj->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONBoolean *>(value)->Get() == true); 
        value = obj->Get(2);
        CPPUNIT_ASSERT(static_cast<JSONNull *>(value)->Get() == 0); 
        delete obj;

        delete parser;
    }

    void testTwoElementNestedArray()
    {
        JsonParse *parser = new JsonParse();
        std::string str("[17, [34, 68]]");    
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONArray *array = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(array);
        JsonType type =array->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        JSONValue *value;
        value = array->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 17);  

        JSONArray *array2 = static_cast<JSONArray *>(array->Get(1));
        CPPUNIT_ASSERT(array2);
        type = array2->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        value = array2->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 34);  
        value = array2->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 68);  

        delete array;

        str = "[[68, 17], 34]";    
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        array = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(array);
        type =array->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        array2 = static_cast<JSONArray *>(array->Get(0));
        CPPUNIT_ASSERT(array2);
        type = array2->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        value = array->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 34);  

        value = array2->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 68);  
        value = array2->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 17);  

        delete array;
        delete parser;
    }

    void testThreeElementNestedArray()
    {
        JsonParse *parser = new JsonParse();
        std::string str("[17, [34, 68, [\"hello\", \"world\"]]]");    
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONArray *array = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(array);
        JsonType type =array->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        JSONValue *value;
        value = array->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 17);  

        JSONArray *array2 = static_cast<JSONArray *>(array->Get(1));
        CPPUNIT_ASSERT(array2);
        type = array2->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        value = array2->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 34);  
        value = array2->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 68);  

        JSONArray *array3 = static_cast<JSONArray *>(array2->Get(2));
        CPPUNIT_ASSERT(array3);
        type = array3->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        value = array3->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"hello\"");  
        value = array3->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"world\"");  

        delete array;

        str = "[[68, 17], 34, [\"a\", \"b\"]]";    
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        array = static_cast<JSONArray *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(array);
        type =array->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        array2 = static_cast<JSONArray *>(array->Get(0));
        CPPUNIT_ASSERT(array2);
        type = array2->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        value = array2->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 68);  
        value = array2->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 17);  

        value = array->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value)->Get() == 34);  

        array3 = static_cast<JSONArray *>(array->Get(2));
        CPPUNIT_ASSERT(array3);
        type = array3->GetType();
        CPPUNIT_ASSERT(type == JsonType_Array);
        value = array3->Get(0);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"a\"");  
        value = array3->Get(1);
        CPPUNIT_ASSERT(static_cast<JSONString *>(value)->Get() == "\"b\"");  

        delete array;
        delete parser;
    }

    void testOneElementObject()
    {
        std::string str("{\"Name\": 2}");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONObject *obj = static_cast<JSONObject *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Object);
        JSONTuple *value;
        value = static_cast<JSONTuple *>(obj->Get(0));
        type = value->GetType();
        CPPUNIT_ASSERT(type == JsonType_Tuple);
        std::string key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Name\"");
        JSONValue *value2 = value->GetValue();
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value2)->Get() == 2);  
        delete obj;
        delete parser;
    }

    void testTwoElementObject()
    {
        std::string str("{\"Name\": 2, \"Age\": 3}");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONObject *obj = static_cast<JSONObject *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Object);
        JSONTuple *value;
        value = static_cast<JSONTuple *>(obj->Get(0));
        type = value->GetType();
        CPPUNIT_ASSERT(type == JsonType_Tuple);
        std::string key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Name\"");
        JSONValue *value2 = value->GetValue();
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value2)->Get() == 2);

        value = static_cast<JSONTuple *>(obj->Get(1));
        type = value->GetType();
        CPPUNIT_ASSERT(type == JsonType_Tuple);
        key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Age\"");
        value2 = value->GetValue();
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value2)->Get() == 3);

        delete obj;
        delete parser;
    }

    void testThreeElementObject()
    {
        std::string str("{\"Name\": 2, \"Age\": 3, \"City\": 4}");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONObject *obj = static_cast<JSONObject *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Object);
        JSONTuple *value;
        value = static_cast<JSONTuple *>(obj->Get(0));
        type = value->GetType();
        CPPUNIT_ASSERT(type == JsonType_Tuple);
        std::string key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Name\"");
        JSONValue *value2 = value->GetValue();
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value2)->Get() == 2);

        value = static_cast<JSONTuple *>(obj->Get(1));
        type = value->GetType();
        CPPUNIT_ASSERT(type == JsonType_Tuple);
        key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Age\"");
        value2 = value->GetValue();
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value2)->Get() == 3);

        value = static_cast<JSONTuple *>(obj->Get(2));
        type = value->GetType();
        CPPUNIT_ASSERT(type == JsonType_Tuple);
        key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"City\"");
        value2 = value->GetValue();
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value2)->Get() == 4);

        delete obj;
        delete parser;
    }

    void testTwoElementNestedObject()
    {
        std::string str("{\"Name\": {\"First\": 1, \"Last\": 2}, \"Age\": 3, \"City\": 4}");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONObject *obj = static_cast<JSONObject *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Object);
        JSONTuple *value;
        value = static_cast<JSONTuple *>(obj->Get(0));
        type = value->GetType();
        CPPUNIT_ASSERT(type == JsonType_Tuple);
        std::string key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Name\"");
        JSONObject *obj2 = static_cast<JSONObject *>(value->GetValue());
        type = obj2->GetType();
        CPPUNIT_ASSERT(type == JsonType_Object);
        value = static_cast<JSONTuple *>(obj2->Get(0));
        key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"First\"");
        JSONValue *value2;
        value2 = value->GetValue();
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value2)->Get() == 1);
        value = static_cast<JSONTuple *>(obj2->Get(1));
        key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Last\"");
        value2 = value->GetValue();
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value2)->Get() == 2);

        value = static_cast<JSONTuple *>(obj->Get(1));
        type = value->GetType();
        CPPUNIT_ASSERT(type == JsonType_Tuple);
        key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Age\"");
        value2 = value->GetValue();
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value2)->Get() == 3);

        value = static_cast<JSONTuple *>(obj->Get(2));
        type = value->GetType();
        CPPUNIT_ASSERT(type == JsonType_Tuple);
        key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"City\"");
        value2 = value->GetValue();
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value2)->Get() == 4);

        delete obj;
        delete parser;
    }

    void testThreeElementNestedObject()
    {
        std::string str("{\"Field1\": {\"Field2\": {\"Field3\": 17}}}");    
        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
        JSONObject *obj = static_cast<JSONObject *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Object);
        JSONTuple *value;
        value = static_cast<JSONTuple *>(obj->Get(0));
        type = value->GetType();
        CPPUNIT_ASSERT(type == JsonType_Tuple);
        std::string key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Field1\"");
        JSONObject *obj2 = static_cast<JSONObject *>(value->GetValue());
        type = obj2->GetType();
        CPPUNIT_ASSERT(type == JsonType_Object);
        value = static_cast<JSONTuple *>(obj2->Get(0));
        key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Field2\"");
        JSONObject *obj3 = static_cast<JSONObject *>(value->GetValue());
        type = obj3->GetType();
        CPPUNIT_ASSERT(type == JsonType_Object);
        value = static_cast<JSONTuple *>(obj3->Get(0));
        key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Field3\"");
        JSONValue *value2;
        value2 = value->GetValue();
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value2)->Get() == 17);

        delete obj;
        delete parser;
    }

    void testObjectTupleArray()
    {
        std::string str = " \
            {\"web-app\": \
                { \
                   \"servlet\": [   \
                        { \
                            \"servlet-name\": \"cofaxCDS1\" \
                        }, \
                        { \
                            \"servlet-name\": \"cofaxCDS2\" \
                        } \
                    ] \
                } \
            }";

        JsonParse *parser = new JsonParse();
        
        parser->SetInput(str);
        CPPUNIT_ASSERT(parser->Parse() == true);
#if 0
        JSONObject *obj = static_cast<JSONObject *>(JSONAPI::GetValue(parser));
        CPPUNIT_ASSERT(obj);
        JsonType type = obj->GetType();
        CPPUNIT_ASSERT(type == JsonType_Object);
        JSONTuple *value;
        value = static_cast<JSONTuple *>(obj->Get(0));
        type = value->GetType();
        CPPUNIT_ASSERT(type == JsonType_Tuple);
        std::string key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Field1\"");
        JSONObject *obj2 = static_cast<JSONObject *>(value->GetValue());
        type = obj2->GetType();
        CPPUNIT_ASSERT(type == JsonType_Object);
        value = static_cast<JSONTuple *>(obj2->Get(0));
        key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Field2\"");
        JSONObject *obj3 = static_cast<JSONObject *>(value->GetValue());
        type = obj3->GetType();
        CPPUNIT_ASSERT(type == JsonType_Object);
        value = static_cast<JSONTuple *>(obj3->Get(0));
        key = value->GetKey();
        CPPUNIT_ASSERT(key == "\"Field3\"");
        JSONValue *value2;
        value2 = value->GetValue();
        CPPUNIT_ASSERT(static_cast<JSONNumber *>(value2)->Get() == 17);

        delete obj;
#endif
        delete parser;
    }

    void testCreateNumber()
    {
        JSONNumber *obj = new JSONNumber(17);
        std::string str;

        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(str == "17");
        delete obj;

        obj = new JSONNumber(-17);
        str = "";
        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(str == "-17");
        delete obj;

        obj = new JSONNumber(+17);
        str = "";
        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(str == "17");
        delete obj;
    }

    void testCreateNull()
    {
        JSONNull *obj = new JSONNull();
        std::string str;

        str = obj->ToJSON(str);

        CPPUNIT_ASSERT(str == "null");
        delete obj;
    }

    void testCreateDouble()
    {
        JSONDouble *obj = new JSONDouble(3.14);
        std::string str;

        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(strstr(str.c_str(), "3.14") != NULL);
        delete obj;

        obj = new JSONDouble(-3.14);
        str = "";
        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(strstr(str.c_str(), "-3.14") != NULL);
        delete obj;

        obj = new JSONDouble(+3.14);
        str = "";
        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(strstr(str.c_str(), "3.14") != NULL);
        delete obj;

        obj = new JSONDouble(1.2e5);
        str = "";
        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(strstr(str.c_str(), "120000") != NULL);
        delete obj;

        obj = new JSONDouble(1.2e-5);
        str = "";
        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(strstr(str.c_str(), "0.000012") != NULL);
        delete obj;
    }

    void testCreateBoolean()
    {
        JSONBoolean *obj = new JSONBoolean(true);
        std::string str;

        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(str == "true");
        delete obj;

        obj = new JSONBoolean(false);
        str = "";
        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(str == "false");
        delete obj;
    }

    void testCreateString()
    {
        JSONString *obj = new JSONString("Hello World");
        std::string str;

        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"Hello World\"");
        delete obj;

        obj = new JSONString("");
        str = "";
        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"\"");
        delete obj;

        obj = new JSONString("Hello\\nWorld");
        str = "";
        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"Hello\\nWorld\"");
        delete obj;

        obj = new JSONString("Hello\nWorld");
        str = "";
        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"Hello\\nWorld\"");
        delete obj;

        obj = new JSONString("\"");
        str = "";
        str = obj->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"\"\"");
        delete obj;
    }

    void testCreateOneElementArray()
    {
        JSONArray *array = new JSONArray();
        JSONNumber *num = new JSONNumber(17);
        array->Append(num);
        std::string str;

        str = array->ToJSON(str);
        CPPUNIT_ASSERT(str == "[17]");
        delete array;
    }

    void testCreateTwoElementArray()
    {
        JSONArray *array = new JSONArray();
        JSONNumber *num1 = new JSONNumber(17);
        JSONNumber *num2 = new JSONNumber(18);
        array->Append(num1);
        array->Append(num2);

        std::string str;
        str = array->ToJSON(str);
        CPPUNIT_ASSERT(str == "[17,18]");

        delete array;
    }

    void testCreateThreeElementArray()
    {
        JSONArray *array = new JSONArray();
        JSONNumber *num1 = new JSONNumber(17);
        JSONNumber *num2 = new JSONNumber(18);
        JSONNumber *num3 = new JSONNumber(19);
        array->Append(num1);
        array->Append(num2);
        array->Append(num3);

        std::string str;
        str = array->ToJSON(str);
        CPPUNIT_ASSERT(str == "[17,18,19]");

        delete array;
    }

    void testCreateTwoElementHeteroArray()
    {
        JSONArray *array = new JSONArray();
        JSONNumber *num = new JSONNumber(17);
        JSONString *jstr = new JSONString("Seventeen");
        array->Append(num);
        array->Append(jstr);

        std::string str;
        str = array->ToJSON(str);
        CPPUNIT_ASSERT(str == "[17,\"Seventeen\"]");

        delete array;
    }

    void testCreateThreeElementHeteroArray()
    {
        JSONArray *array = new JSONArray();
        JSONNumber *num = new JSONNumber(17);
        JSONString *jstr = new JSONString("Seventeen");
        JSONNull *null = new JSONNull();
        array->Append(num);
        array->Append(jstr);
        array->Append(null);

        std::string str;
        str = array->ToJSON(str);
        CPPUNIT_ASSERT(str == "[17,\"Seventeen\",null]");

        delete array;
    }

    void testCreateTwoElementNestedArray()
    {
        JSONArray *array1 = new JSONArray();
        JSONArray *array2 = new JSONArray();
        JSONNumber *num = new JSONNumber(17);
        JSONString *jstr = new JSONString("Seventeen");
        JSONNull *null = new JSONNull();
        array1->Append(num);
        array1->Append(array2);
        array2->Append(jstr);
        array2->Append(null);

        std::string str;
        str = array1->ToJSON(str);
        CPPUNIT_ASSERT(str == "[17,[\"Seventeen\",null]]");

        delete array1;
    }

    void testCreateThreeElementNestedArray()
    {
        JSONArray *array1 = new JSONArray();
        JSONArray *array2 = new JSONArray();
        JSONArray *array3 = new JSONArray();
        JSONNumber *num1 = new JSONNumber(17);
        JSONNumber *num2 = new JSONNumber(34);
        JSONNumber *num3 = new JSONNumber(68);
        JSONNumber *num4 = new JSONNumber(5);
        JSONString *jstr = new JSONString("Seventeen");
        JSONNull *null = new JSONNull();
        array1->Append(num1);
        array1->Append(array2);
        array2->Append(array3);
        array2->Append(jstr);
        array2->Append(null);
        array3->Append(num2);
        array3->Append(num3);
        array1->Append(num4);
    

        std::string str;
        str = array1->ToJSON(str);
        CPPUNIT_ASSERT(str == "[17,[[34,68],\"Seventeen\",null],5]");

        delete array1;
    }

    void testCreateOneElementObject()
    {
        JSONObject *object = new JSONObject();
        JSONTuple *tuple = new JSONTuple();
        JSONString *jstr = new JSONString("Fred");
       
        tuple->SetKey("Name");
        tuple->SetValue(jstr);
        
        object->Append(tuple);
        
        std::string str;

        str = object->ToJSON(str);
        CPPUNIT_ASSERT(str == "{\"Name\": \"Fred\"}");
        delete object;
    }

    void testCreateTwoElementObject()
    {
        JSONObject *object = new JSONObject();
        JSONTuple *tuple1 = new JSONTuple();
        JSONTuple *tuple2 = new JSONTuple();
        JSONString *jstr = new JSONString("Fred");
        JSONNumber *num = new JSONNumber(50);
       
        tuple1->SetKey("Name");
        tuple1->SetValue(jstr);
        
        tuple2->SetKey("Age");
        tuple2->SetValue(num);
        
        object->Append(tuple1);
        object->Append(tuple2);
        
        std::string str;

        str = object->ToJSON(str);
        CPPUNIT_ASSERT(str == "{\"Name\": \"Fred\",\"Age\": 50}");
        delete object;
    }

    void testCreateThreeElementObject()
    {
        JSONObject *object = new JSONObject();
        JSONTuple *tuple1 = new JSONTuple();
        JSONTuple *tuple2 = new JSONTuple();
        JSONTuple *tuple3 = new JSONTuple();
        JSONString *jstr = new JSONString("Fred");
        JSONNumber *num = new JSONNumber(50);
        JSONBoolean *bval = new JSONBoolean(true);
       
        tuple1->SetKey("Name");
        tuple1->SetValue(jstr);
        
        tuple2->SetKey("Age");
        tuple2->SetValue(num);
        
        tuple3->SetKey("Boolean");
        tuple3->SetValue(bval);
        
        object->Append(tuple1);
        object->Append(tuple2);
        object->Append(tuple3);
        
        std::string str;

        str = object->ToJSON(str);
        CPPUNIT_ASSERT(str == "{\"Name\": \"Fred\",\"Age\": 50,\"Boolean\": true}");
        delete object;
    }

    void testCreateTwoElementNestedObject()
    {
        JSONObject *object1 = new JSONObject();
        JSONObject *object2 = new JSONObject();
        JSONTuple *tuple1 = new JSONTuple();
        JSONTuple *tuple2 = new JSONTuple();
        JSONTuple *tuple3 = new JSONTuple();
        JSONTuple *tuple4 = new JSONTuple();
        JSONString *jstr = new JSONString("Fred");
        JSONNumber *num1 = new JSONNumber(50);
        JSONNumber *num2 = new JSONNumber(210);
       
        tuple1->SetKey("Name");
        tuple1->SetValue(jstr);
        
        tuple2->SetKey("Vitals");
        tuple2->SetValue(object2);

        tuple3->SetKey("Age");
        tuple3->SetValue(num1);
        tuple4->SetKey("Weight");
        tuple4->SetValue(num2);
        
        object1->Append(tuple1);
        object1->Append(tuple2);
        object2->Append(tuple3);
        object2->Append(tuple4);
        
        std::string str;

        str = object1->ToJSON(str);
        CPPUNIT_ASSERT(str == "{\"Name\": \"Fred\",\"Vitals\": {\"Age\": 50,\"Weight\": 210}}");
        delete object1;
    }

    void testCreateThreeElementNestedObject()
    {
        JSONObject *object1 = new JSONObject();
        JSONObject *object2 = new JSONObject();
        JSONObject *object3 = new JSONObject();
        JSONTuple *tuple1 = new JSONTuple();
        JSONTuple *tuple2 = new JSONTuple();
        JSONTuple *tuple3 = new JSONTuple();
        JSONTuple *tuple4 = new JSONTuple();
        JSONTuple *tuple5 = new JSONTuple();
        JSONTuple *tuple6 = new JSONTuple();
        JSONString *jstr = new JSONString("Fred");
        JSONNumber *num1 = new JSONNumber(50);
        JSONNumber *num2 = new JSONNumber(210);
        JSONNumber *num3 = new JSONNumber(72);
       
        tuple1->SetKey("Name");
        tuple1->SetValue(jstr);
        
        tuple2->SetKey("Vitals");
        tuple2->SetValue(object2);

        tuple3->SetKey("Age");
        tuple3->SetValue(num1);
        tuple4->SetKey("Weight and Height");
        tuple4->SetValue(object3);

        tuple5->SetKey("Weight");
        tuple5->SetValue(num2);
        tuple6->SetKey("Height");
        tuple6->SetValue(num3);
        
        object1->Append(tuple1);
        object1->Append(tuple2);
        object2->Append(tuple3);
        object2->Append(tuple4);
        object3->Append(tuple5);
        object3->Append(tuple6);
        
        std::string str;

        str = object1->ToJSON(str);
        CPPUNIT_ASSERT(str == "{\"Name\": \"Fred\",\"Vitals\": {\"Age\": 50,\"Weight and Height\": {\"Weight\": 210,\"Height\": 72}}}");
        delete object1;
    }

    void testCreateUTF8()
    {
        JSONString *jstr;
        std::string str;

        jstr = new JSONString("\\u0024");
        str = jstr->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"\\u0024\"");
        delete jstr;

        jstr = new JSONString("\u0024");
        str = "";
        str = jstr->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"\u0024\"");
        delete jstr;

        jstr = new JSONString("\\u00A2");
        str = "";
        str = jstr->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"\\u00A2\"");
        delete jstr;

        jstr = new JSONString("\u00A2");
        str = "";
        str = jstr->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"\\u00A2\"");
        delete jstr;

        jstr = new JSONString("\\u20AC");
        str = "";
        str = jstr->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"\\u20AC\"");
        delete jstr;

        jstr = new JSONString("\u20AC");
        str = "";
        str = jstr->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"\\u20AC\"");
        delete jstr;

        jstr = new JSONString("\\u003Cp\\u003E");
        str = "";
        str = jstr->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"\\u003Cp\\u003E\"");
        delete jstr;

        jstr = new JSONString("\u003Cp\u003E");
        str = "";
        str = jstr->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"<p>\"");
        delete jstr;

        jstr = new JSONString("\u20ACp\u20AC");
        str = "";
        str = jstr->ToJSON(str);
        CPPUNIT_ASSERT(str == "\"\\u20ACp\\u20AC\"");
        delete jstr;
    }
private:
};

#endif 
