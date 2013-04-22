#if !defined(__JSONPARSE_H__)
#define __JSONPARSE_H__

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
#include "context.h"
#include <string>

/**
 * Helper class providing interfaces useful to parser, primarily called
 * when parser reduces some production that requires adding something to
 * the DOM.
 */

class JsonParse 
{
public:
    JsonParse();
    ~JsonParse();
    void PushRoot();
    bool AddTuple(void *name, void *val);
    bool AddArray(JsonValue *obj);
    bool AddObject(JsonValue *obj);
    bool AddValue(JsonValue *obj);
    std::string ToJson();
    std::string ToJson(JsonValue *val);
    JsonType GetType();
    JsonNode *GetRoot();
    void SetInput(std::string &input);
    void GetInput(char *buffer, int *sizeOut, int sizeIn);
    Context *GetContext() {return &m_ctx;}
    bool Parse();
    void HandleError(void *scanner, const char *msg);
private:
    int m_offset;
    std::string m_input;
    Context m_ctx;
    JsonNode *m_root;
};

#endif
