#include <stdio.h>
#include <string.h>
#include <cmark-gfm.h>
#include "markable.h"

/* Wrap cmark-gfm's cmark_markdown_to_html().
 *
 * This function expects argv to contain between one and two values. The first
 * value is the Markdown-formatted string to be converted to HTML. The second
 * optional value  is a JanetTuple collection of JanetKeyword values that
 * represent the options to used to parse the string. If the second parameter
 * is not provided, the current value of markable_default_options will be used.
 *
 * The return value is a wrapped JanetString value.
 */
static Janet cfun_markdown_to_html(int32_t argc, Janet *argv) {
    janet_arity(argc, 1, 2);

    const char *input = janet_getcstring(argv, 0);

    int32_t options;
    if (argc == 2) {
        options = markable_extract_options(janet_getindexed(argv, 1));
    } else {
        options = markable_default_options;
    }

    char *output = cmark_markdown_to_html(input, strlen(input), options);
    JanetString html = janet_string((uint8_t *)output, strlen(output));
    free(output);

    return janet_wrap_string(html);
}

/* Enumerate the wrapped functions.
 *
 * Enumerate the wrapped functions in a struct appropriate for registration.
 * The third argument will be the a docstring.
 */
static const JanetReg cfuns[] = {
    {"markdown->html", cfun_markdown_to_html,
     "(markable/markdown->html str &opt opts)\n\n"
     "Convert `str` from Markdown to HTML. `opts` is a tuple of keywords that "
     "match those that can be provided to the cmark-gfm library. Keywords are "
     "of the form such that CMARK_OPT_SMART becomes :smart. Option names that "
     "are in snake-case should be converted to kebab-case (e.g. "
     "CMARK_OPT_VALIDATE_UTF8 becomes :validate-utf8)."
    },
    {NULL, NULL, NULL}
};

/* Register the wrapped functions.
 *
 * Markable modularises its wrapped functions. This function is called from
 * markable.c.
 */
void markable_register_converter(JanetTable *env) {
    janet_cfuns(env, "markable", cfuns);
}
