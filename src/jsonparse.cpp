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

#include "jsonparse.h"
#include "context.h"
#include "jsonobj.h"
#include <memory.h>

#if 0
int yyerror(JsonParse *parser, void *yyscanner, const char *s);
#include "json.h" // XXX for YYSTYPE
int yylex(YYSTYPE *yylval_param, void *yyscanner);
#include "json.cpp"
#include "lex.cpp"
#endif

#if 0
/**
 * Display error message found during a parse.
 *
 * @param[in] scanner lexical analyzer
 * @param[in] msg the error message to display
 * 
 * @note sometimes the return from yyget_lineno is nonsense. 
 */

void
JsonParse::HandleError(void *scanner, const char *msg)
{
#ifdef JSON_PARSE_VERBOSE
    if (scanner && msg) {
        fprintf(stderr, "line %d: %s\n", yyget_lineno(scanner) + 1, msg); 
    } else if (msg) {
        fprintf(stderr, "%s\n", msg); 
    }
#endif
}

#endif


/**
 * Constructor.
 */

JsonParse::JsonParse() :
    m_offset(0),
    m_root(NULL)
{
}


/**
 * Destructor.
 */

JsonParse::~JsonParse()
{
    delete m_root;
}


/**
 * Get type of the object that was parsed by looking at the root element.
 *
 * @return a JsonType enumeration value.
 */

JsonType
JsonParse::GetType()
{
    JsonType type = JsonType_Root;

    if (m_root->GetNumChildren() == 1) {
        std::list<JsonNode *>::iterator iter;

        iter = m_root->GetChildren().begin();
        type = (*iter)->GetType();
    }
    return type;
}


/** 
 * Get the root element from the parse.
 *
 * @return the root element of the parse.
 */

JsonNode *
JsonParse::GetRoot()
{
    JsonNode *ret = (JsonNode *)NULL;
    if (m_root->GetNumChildren() == 1) {
        std::list<JsonNode *>::iterator iter;

        ret = *(m_root->GetChildren().begin());
    }
    return ret;
}

#if 0
/**
 * Parse the JSON string, constructing an object tree which can be used
 * by the API.
 *
 * @return true on success, false on failure.
 */

bool
JsonParse::Parse()
{
    JsonNode *p;
    YY_BUFFER_STATE state;
    yyscan_t scanner;

    // clear out any previous context, e.g., a subsequent parse.

    while ((p = m_ctx.Pop()) != (JsonNode *) NULL) {
        delete p;
    }

    m_root = NULL;

    PushRoot();

    if (yylex_init(&scanner)) {
        fprintf(stderr, "%s: failed to init scanner\n", __FUNCTION__);
        return false;
    }

    state = yy_scan_string(m_input.c_str(), scanner);

    int ret = yyparse(this, scanner);
    yy_delete_buffer(state, scanner);
    yylex_destroy(scanner);

    // XXX check for errors from parse.

    return ret == 0 ? true : false;
}

#endif
/**
 * Set the input path for a parse.
 *
 * @param[in] input path of input file.
 */

void
JsonParse::SetInput(std::string &input)
{
    m_input = input;
    m_offset = 0;
}


/**
 * Read a specified number of bytes from the input
 *
 * @param[in] buffer buffer to hold result, allocated by caller.
 * @param[out] sizeOut the actual number of bytes read.
 * @param[in] sizeIn the number of bytes to read.
 */

void
JsonParse::GetInput(char *buffer, int *sizeOut, int sizeIn)
{
    int rem = m_input.length() - m_offset;

    if (sizeIn >= rem) {
        memcpy(buffer, m_input.c_str() + m_offset, rem);
        *sizeOut = rem;
        m_offset += rem;
    } else {
        memcpy(buffer, m_input.c_str() + m_offset, sizeIn);
        *sizeOut = sizeIn;
        m_offset += sizeIn;
    }
}


/**
 * Convert the object specified by root to a JSON string. 
 * 
 * @param[in] root the object to convert.
 * 
 * @return the value and its children, if any, as a JSON string.
 *
 * @note this function is not a part of the public API.
 */

std::string
JsonParse::ToJson(JsonValue *root)
{
    std::string str;

    return root->ToJson(str);
}


/**
 * Convert the object specified by root to a JSON string. 
 * 
 * @param[in] root the object to convert.
 * 
 * @return the value and its children, if any, as a JSON string.
 *
 * @note this function is not a part of the public API.
 */

std::string
JsonParse::ToJson()
{
    JsonNode *current;
    std::string str;

    if (m_ctx.Current()) {
        str = ToJson(static_cast<JsonValue *>(m_ctx.Current()));
    }
    return str;
}


/** 
 * Push a root object onto the context stack.
 */

void
JsonParse::PushRoot()
{
    if (m_root) {
        delete m_root;
    }
    m_root = new JsonNode();
    m_root->SetType(JsonType_Root);
    m_ctx.Push(m_root);
}


/**
 * Create a JsonTuple and add it as a child to the node on top of the
 * context stack.
 *
 * @param[in] name key for the tuple.
 * @param[in] value value for the tuple/
 *
 * @return true on success, false otherwise.
 */

bool
JsonParse::AddTuple(void *name, void *val)
{
    bool ret = false;

    JsonNode *current;
    JsonTuple *tuple;

    current = m_ctx.Current();
    if (current) {
        tuple = new JsonTuple(
            static_cast<JsonValue *>(name),
            static_cast<JsonValue *>(val));
        current->AddChild(tuple);
        ret = true;
    } else {
        delete static_cast<JsonValue *>(name);
        delete static_cast<JsonValue *>(val);
    } 
out:
    return ret;
}


/**
 * Push JSON object onto context stack.
 *
 * @param[in] obj the object to push.
 *
 * @return true on success, false on failure.
 */

bool 
JsonParse::AddObject(JsonValue *obj)
{
    obj->SetType(JsonType_Object); 
    if (m_ctx.Current()->GetType() == JsonType_Root ||
        m_ctx.Current()->GetType() == JsonType_Array) {
        m_ctx.Current()->AddChild(obj); 
    }
    m_ctx.Push(obj);
    return true;
}


/**
 * Push JSON value onto context stack.
 *
 * @param[in] obj the value to push.
 *
 * @return true on success, false on failure.
 */

bool 
JsonParse::AddValue(JsonValue *obj)
{
    bool ret = false;

    JsonNode *current;
    JsonType type;

    current = m_ctx.Current();
    if (current) {
        type = static_cast<JsonValue *>(current)->GetType();
    }
    if (current && type != JsonType_Object && type != JsonType_Tuple) {
        current->AddChild(obj);
        ret = true;
    } 
out:
    return ret;
}
