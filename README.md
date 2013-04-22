jsonapi
=======

A simple, fast C++ API for parsing and creating JSON. 

What is the license of JSONAPI?
-------------------------------

JSONAPI is licensed under a BSD license. Generally, this means you can do with
it what you please, as long as I get credit somewhere. Please see the sources 
for details.

What is JSONAPI?
----------------

JSONAPI is a simple, fast C++ JSON parser and encoder that supports UTF-8.
It consists of a shared lib (libjsonapi), a test program (jsonapitest), 
and headers.

It parses using a parser generated from a grammar via yacc/bison. It creates
an in-memory representation of the JSON, this representation is custom and
exposed through a custom API. 

You can also generate JSON by instantiating objects and constructing an 
in-memory repesentation, and then calling ToJSON(). The construction of
the in-memory representation is simple and hopefully intuitive to use.

How fast is JSONAPI?
--------------------

It parses JSON *fast*. This is most likely due to my use of Yacc and lex.
Could I write a faster parser? Probably not.

It encodes a bit more slowly, but probably due to string copies and I'm 
working on it :-). Still, even with encode, it is not in last place among 
those benchmarked at https://github.com/lijoantony/JsonBenchmarkCpp

In an Ubuntu virtual machine running on a MacBook Pro:

<pre>
$ ./JsonBenchmarkCpp 
#library                 parsing                  writing                  
jsonapi                  262                      666                      
cajun                    1738                     280                      
json_spirit              10784                    998                      
libjson                  331                      285                      
json_parser              90                       
</pre>

What does JSONAPI depend on?
----------------------------

JSONAPI itself depends on nothing at runtime except the C++ standard library. 

While I chose to distribute the code with a build system based on autotools, 
the code itself is not dependent on any given platform. JSONAPI was developed 
with GNU C++ compiler version 4.6.3, but it should be portable to earlier and 
later versions of gcc, and to other compilers like Microsoft's Visual C++.

The most complicated C++ construct I used in the code besides classes are 
std::list and std::string. I built my own custom DOM that should represent
JSON object structures about as efficiently as one can in terms of speed,
and implemented what I hope is a sane API on top of it.

The lexer and parser require you to build the library on a system that has
GNU bison and flex installed. I used versions bison 2.5, and flex 2.5.35. 
I would expect later versions to work as well.

The test harness is based on cppunit, so that is a requirement as well. 

If you spin up an instance of Ubuntu 12.04 LTS as a VM, flex, bison, and
cppunit packages as well as the compiler will match mine. 

Why did you write JSONAPI?
--------------------------

I was working on an arduino-based robot that communicates back to a Linux
system. I wanted to use JSON to encode messages between the two. I looked
at some existing C++ JSON parsers and decided I wanted to write my own. 

How do I build JSONAPI?
-----------------------

- Install flex, bison, and cppunit on your system, if not already there.
- ./configure
- make; sudo make install

On Ubuntu systems, this results in the jsonapi library being installed in
/usr/local/lib/jsonapi, the headers installed in /usr/local/include/jsonapi, 
and the test application installed as /usr/local/bin/jsonapitest

Where can I find examples of JSONAPI?
-------------------------------------

Test-driven development was used in the making of JSONAPI. The header file 
test/jsonapitest.h has plenty of simple examples of JSON encode and decode.
All of these tests should pass, so they can be relied on as examples.

What does a parser hello world program look like?
-------------------------------------------------

The following program takes a command line argument as JSON, parses it
and displays the type of the root element parsed.

<pre>
 #include "jsonapi.h"

 void
 usage(char *name)
 {
    fprintf(stderr, "usage: %s <json>\n", name);
    exit(0);
 }

 int
 main(int argc, char *argv[])
 {
    if (argc != 2) {
        usage(argv[0]);
    }

    std::string input(argv[1]);
    JsonParse *parser = new JsonParse();

    parser->SetInput(input);
    if (parser->Parse() == true) {
        JSONValue *val = JSONAPI::GetValue(parser);

        switch(val->GetType()) {
        case JsonType_Object:
            printf("type is JsonType_Object\n");
            break;
        case JsonType_Array:
            printf("type is JsonType_Array\n");
            break;
        case JsonType_String:
            printf("type is JsonType_String\n");
            break;
        case JsonType_Number:
            printf("type is JsonType_Number\n");
            break;
        case JsonType_Double:
            printf("type is JsonType_Double\n");
            break;
        case JsonType_Bool:
            printf("type is JsonType_Bool\n");
            break;
        case JsonType_Tuple:
            printf("type is JsonType_Tuple\n");
            break;
        case JsonType_Null:
            printf("type is JsonType_Null\n");
            break;
        default:
            printf("type is unknown\n");
            break;
        }

        delete val;
    }
    delete parser;
 }
</pre>

Compile as in the following example:

<pre>
 $ g++ -o testit testit.cpp -I/usr/local/include/jsonapi \
        -L/usr/local/lib/jsonapi -ljsonapi
</pre>

Run it as in the following example:

<pre>
 $ ./testit "\"hello world\""
 type is JsonType_String
 $ ./testit 1.3
 type is JsonType_Double
 $ ./testit "{\"foo\": 17}"
 type is JsonType_Object
 $ ./testit "[1, \"fred\", 2, 19.4]" 
 type is JsonType_Array
</pre>

How do I decode JSON using JSONAPI?
-----------------------------------

Generally, to decode, you create an instance of JsonParse, give it a string
containing the JSON to parse, call the Parse() method, and then inspect the
results. 

Here is a example that illustrates parsing a JSON array which contains 
two integers:

<pre>
        std::string str("[17, 18]");

        // allocate the parser

        JsonParse *parser = new JsonParse();

        // set the input

        parser->SetInput(str);

        // parse the string. if there is a failure, false will be returned.

        if (parser->Parse() == true) {

            // get the value of the parse, and then determine the type of
            // object parsed.

            JSONValue *val = JSONAPI::GetValue(parser);
            JSONType type = val->GetType();

            // based on the type of object returned, cast to a derived
            // class, and then use the methods provided by JSONArray to 
            // further inspect the results.

            switch (type) {
                case JsonType_Array:
                    // convert the value to an array

                    JSONArray *array = static_cast<JSONArray *>(val);

                    // get the first element of the array

                    val = array->Get(0);

                    type = val->GetType();

                    if (type == JsonType_Number) {

                        // convert the value object to a JSONNumber object
                        // and then retrieve the actual value

                        int num = static_cast<JSONNumber *>(val)->Get();

                        // the following should print "number is 17"

                        printf("number is %d\n", num);
                    }
                    ...
                    break;
                ...
            }
        }
</pre>
 
How do I encode JSON using JSONAPI?
-----------------------------------

Generally, to encode, you create a data structure that reflects the
content of your JSON, and you then call a function to encode it. The 
encoding function returns a string that is JSON format.

Here is an example of how you might create a two element array of
integers:

<pre>
        // instantiate an array object, and two number objects.

        JSONArray *array = new JSONArray();
        JSONNumber *num1 = new JSONNumber(17);
        JSONNumber *num2 = new JSONNumber(18);

        // append the numbers to the array

        array->Append(num1);
        array->Append(num2);

        // encode the array as JSON

        std::string str;
        str = array->ToJSON(str);
        assert(str == "[17,18]");
</pre>

Does JSONAPI support Unicode?
-----------------------------

Technically, JSONAPI supports UTF-8, which is the most common instance of
"Unicode". It can parse strings that contain UTF-8 as a literal in the
form \\uabcd (this is 4 C characters '\\', 'u', 'a', 'b', 'c', 'd').
JSONAPI can also generate JSON UTF-8 from 2 and 3 byte UTF-8 encodings
in strings. 

For example, you can parse a string containing the text "\\u0024" and convert 
it to UTF-8 as follows:

<pre>
        std::string str = "\"\\u0024\"";

        // set the parser's input string

        parser->SetInput(str);

        // parse and look at the result.

        if (parser->Parse() == true) {
            JSONString *obj = 
                static_cast<JSONString *>(JSONAPI::GetValue(parser));

            // get the literal value of the string. This is not
            // UTF-8, and will be the same value as passed to the
            // parser. Note the escaped '\', i.e., "\\u0024"

            std::string value = obj->Get();
            assert(value == "\"\\u0024\"");

            // Get the UTF-8 byte stream. This will be a null terminated
            // C string that is UTF-8 encoded (as 1, 2, or 3 bytes).
            // Note the missing escape, i.e., "\u0024". In this example
            // the encoding is 1 byte in length.

            char *ret = obj->GetAsUTF8();
            assert(value == "\"$\"");
            assert(value == "\"\u0024\"");
</pre>

See the function ParseTest::testCreateUTF8() in jsonapitest.h for examples
of creating JSON from strings that encode 2 and 3 byte UTF-8.
