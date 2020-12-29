#include <stdio.h>
#include <string.h>
#include "markable.h"

/* Wrap cmark-gfm's cmark_markdown_to_html().
 *
 * This function expects argv to contain between one and three values.
 *
 * The first value is the Markdown-formatted string to be converted to HTML.
 *
 * The optional second value is a JanetTuple collection of JanetKeyword values
 * that represent the options to used to parse the string. If the second
 * parameter is not provided, the current value of markable_default_options
 * will be used.
 *
 * The optional third value is a JanetTuple collection of JanetKeyword values
 * that represent the extensions to be loaded.
 *
 * The return value is a wrapped JanetString value.
 */
static Janet cfun_markdown_to_html(int32_t argc, Janet *argv) {
    janet_arity(argc, 1, 3);

    const char *input = janet_getcstring(argv, 0);

    int32_t options;
    if (argc == 2) {
        options = markable_extract_options(janet_getindexed(argv, 1));
    } else {
        options = markable_default_options;
    }

    JanetView extensions;
    if (argc == 3) {
        extensions = janet_getindexed(argv, 2);
    } else {
        extensions.items = NULL;
        extensions.len = 0;
    }

    cmark_mem *arena_mem = cmark_get_arena_mem_allocator();
    cmark_parser *parser = cmark_parser_new_with_mem(options, arena_mem);

    for (size_t i = 0; i < (size_t)extensions.len; i++) {
        const char *name = (const char *)janet_getkeyword(extensions.items, i);
        cmark_syntax_extension *syntax_extension = cmark_find_syntax_extension(name);
        if (NULL == syntax_extension) janet_panicf("invalid extension :%s", name);
        cmark_parser_attach_syntax_extension(parser, syntax_extension);
    }

    cmark_parser_feed(parser, input, strlen(input));
    cmark_node *doc = cmark_parser_finish(parser);

    cmark_mem *default_mem = cmark_get_default_mem_allocator();
    char *output = cmark_render_html_with_mem(doc, options, parser->syntax_extensions, default_mem);

    JanetString html = janet_string((uint8_t *)output, strlen(output));

    default_mem->free(output);
    cmark_arena_reset();

    return janet_wrap_string(html);
}

/* Enumerate the wrapped functions.
 *
 * Enumerate the wrapped functions in a struct appropriate for registration.
 * The third argument will be the a docstring.
 */
static const JanetReg cfuns[] = {
    {"markdown->html", cfun_markdown_to_html,
     "(markable/markdown->html str &opt opts exts)\n\n"
     "Convert `str` from Markdown to HTML. `opts` is a tuple of keywords that "
     "match those that can be provided to the cmark-gfm library. Keywords are "
     "of the form such that CMARK_OPT_SMART becomes :smart. Option names that "
     "are in snake-case should be converted to kebab-case (e.g. "
     "CMARK_OPT_VALIDATE_UTF8 becomes :validate-utf8). `exts` is a tuple of"
     "keywords that match the name of the extensions in the GitHub Flavored"
     "Markdown Spec."
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
