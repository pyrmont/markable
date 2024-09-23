#include <stdio.h>
#include <string.h>
#include "../markable.h"

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
    if (argc >= 2) {
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
        if (NULL == syntax_extension) {
            cmark_arena_reset();
            janet_panicf("invalid extension :%s", name);
        }
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

/* Wrap cmark-gfm's cmark_render_plaintext_with_mem().
 *
 * This function expects argv to contain between one and three values.
 *
 * The first value is the Markdown-formatted string to be converted to
 * plaintext.
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
static Janet cfun_markdown_to_plaintext(int32_t argc, Janet *argv) {
    janet_arity(argc, 1, 3);

    const char *input = janet_getcstring(argv, 0);

    int32_t options;
    if (argc >= 2) {
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
        if (NULL == syntax_extension) {
            cmark_arena_reset();
            janet_panicf("invalid extension :%s", name);
        }
        cmark_parser_attach_syntax_extension(parser, syntax_extension);
    }

    cmark_parser_feed(parser, input, strlen(input));
    cmark_node *doc = cmark_parser_finish(parser);

    cmark_mem *default_mem = cmark_get_default_mem_allocator();
    char *output = cmark_render_plaintext_with_mem(doc, options, parser->syntax_extensions, default_mem);

    JanetString plaintext = janet_string((uint8_t *)output, strlen(output));

    default_mem->free(output);
    cmark_arena_reset();

    return janet_wrap_string(plaintext);
}

/* Enumerate the wrapped functions.
 *
 * Enumerate the wrapped functions in a struct appropriate for registration.
 * The third argument will be the a docstring.
 */
static const JanetReg cfuns[] = {
    {"markdown->html", cfun_markdown_to_html,
     "(markdown->html str &opt opts exts)\n\n"
     "Convert a string from Markdown to HTML\n\n"
     "Markable converts Markdown to HTML using the cmark-gfm library. This is "
     "GitHub's implementation of the cmark Common Mark parser. It supports "
     "the GitHub-Flavored Markdown specification.\n\n"
     "In addition to the string to convert, the user can provide `opts` and "
     "`exts`.\n\n"
     "`opts` is a tuple of keywords. The keywords and the cmark-gfm library "
     "options that they map to is shown below::\n\n"
     "```\n"
     "cmark-gfm                                Markable\n"
     "=======================================================================\n"
     "CMARK_OPT_SOURCEPOS                      :sourcepos\n"
     "CMARK_OPT_HARDBREAKS                     :hardbreaks\n"
     "CMARK_OPT_UNSAFE                         :unsafe\n"
     "CMARK_OPT_VALIDATE_UTF8                  :validate-utf8\n"
     "CMARK_OPT_SMART                          :smart\n"
     "CMARK_OPT_GITHUB_PRE_LANG                :github-pre-lang\n"
     "CMARK_OPT_LIBERAL_HTML_TAG               :liberal-html-tag\n"
     "CMARK_OPT_FOOTNOTES                      :footnotes\n"
     "CMARK_OPT_STRIKETHROUGH_DOUBLE_TILDE     :strikethrough-double-tilde\n"
     "CMARK_OPT_TABLE_PREFER_STYLE_ATTRIBUTES  :table-prefer-style-attributes\n"
     "CMARK_OPT_FULL_INFO_STRING               :full-info-string\n"
     "```\n\n"
     "`exts` is a tuple of keywords that match the name of the extensions "
     "in the GitHub-Flavored Markdown specification. These extensions are "
     "currently:\n\n"
     "- `:autolink`\n"
     "- `:strikethrough`\n"
     "- `:table`\n"
     "- `:tagfilter`\n"
     "- `:tasklist`"
    },
    {"markdown->plaintext", cfun_markdown_to_plaintext,
     "(markdown->plaintext str &opt opts exts)\n\n"
     "Convert a string from Markdown to plaintext\n\n"
     "Markable converts Markdown to plaintext using the cmark-gfm library. "
     "This is GitHub's implementation of the cmark Common Mark parser. It "
     "supports the GitHub-Flavored Markdown specification.\n\n"
     "In addition to the string to convert, the user can provide `opts` and "
     "`exts`.\n\n"
     "`opts` is a tuple of keywords. The keywords and the cmark-gfm library "
     "options that they map to is shown below::\n\n"
     "```\n"
     "cmark-gfm                                Markable\n"
     "=======================================================================\n"
     "CMARK_OPT_SOURCEPOS                      :sourcepos\n"
     "CMARK_OPT_HARDBREAKS                     :hardbreaks\n"
     "CMARK_OPT_UNSAFE                         :unsafe\n"
     "CMARK_OPT_VALIDATE_UTF8                  :validate-utf8\n"
     "CMARK_OPT_SMART                          :smart\n"
     "CMARK_OPT_GITHUB_PRE_LANG                :github-pre-lang\n"
     "CMARK_OPT_LIBERAL_HTML_TAG               :liberal-html-tag\n"
     "CMARK_OPT_FOOTNOTES                      :footnotes\n"
     "CMARK_OPT_STRIKETHROUGH_DOUBLE_TILDE     :strikethrough-double-tilde\n"
     "CMARK_OPT_TABLE_PREFER_STYLE_ATTRIBUTES  :table-prefer-style-attributes\n"
     "CMARK_OPT_FULL_INFO_STRING               :full-info-string\n"
     "```\n\n"
     "`exts` is a tuple of keywords that match the name of the extensions "
     "in the GitHub-Flavored Markdown specification. These extensions are "
     "currently:\n\n"
     "- `:autolink`\n"
     "- `:strikethrough`\n"
     "- `:table`\n"
     "- `:tagfilter`\n"
     "- `:tasklist`"
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
