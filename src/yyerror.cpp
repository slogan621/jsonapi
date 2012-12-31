#include "jsonparse.h"

/**
 * Handle bison error.
 *
 * @param[in] locp parse object
 * @param[in] yyscanner the scanner, which provides context
 * @param[in] s the error message.
 */
 
int
yyerror(JsonParse *locp, void *yyscanner, const char *s)
{
    locp->HandleError(yyscanner, s);
    return 0;
}
