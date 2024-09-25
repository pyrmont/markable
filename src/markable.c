#include <cmark-gfm.h>
#include <cmark-gfm-core-extensions.h>
#include <cmark-gfm-extension_api.h>
#include <janet.h>
#include <parser.h>
#include <syntax_extension.h>

/* The 'default' options to apply to parsing.
 *
 * This value is initially set to CMARK_OPT_DEFAULT. It is used to set the
 * options that are passed to cmark-gfm's parser if the user does not expressly
 * provide any options.
 *
 * TODO: Provide a mechanism to update this value.
 */
JANET_THREAD_LOCAL JanetTable *markable_option_values = NULL;

/* The 'default' options to apply to parsing.
 *
 * This value is initially set to CMARK_OPT_DEFAULT. It is used to set the
 * options that are passed to cmark-gfm's parser if the user does not expressly
 * provide any options.
 *
 * TODO: Provide a mechanism to update this value.
 */
JANET_THREAD_LOCAL int32_t markable_default_options = CMARK_OPT_DEFAULT;

/* Sets the mappings from keywords to integer values.
 *
 * cmark-gfm provides the user with the ability to set multiple parser options
 * by providing an integer value to parse-related functions. Multiple options
 * can be composed by setting the value of each option to an integer value that
 * can be combined using bitwise or.
 *
 * To make this more ergonomic within Janet, Markable provides mapping of C
 * string values to cmark-gfm's pre-defined integer values. This is implemented
 * using a JanetTable collection and lazy loaded when
 * markable_extract_options() is called. The collection is marked with
 * janet_gcroot() to avoid it being garbage collected during the life of the
 * program.
 *
 * To make it simple for the user, the name of each option is the kebab-case
 * equivalent of the option's constant name with the 'CMARK_OPT_' omitted. For
 * example, CMARK_OPT_VALIDATE_UTF8 becomes "validate-utf8".
 */
static JanetTable *markable_options() {
    if (NULL != markable_option_values) return markable_option_values;

    markable_option_values = janet_table(0);
    janet_gcroot(janet_wrap_table(markable_option_values));
    janet_table_put(markable_option_values, janet_cstringv("sourcepos"), janet_wrap_integer(CMARK_OPT_SOURCEPOS));
    janet_table_put(markable_option_values, janet_cstringv("hardbreaks"), janet_wrap_integer(CMARK_OPT_HARDBREAKS));
    janet_table_put(markable_option_values, janet_cstringv("unsafe"), janet_wrap_integer(CMARK_OPT_UNSAFE));
    janet_table_put(markable_option_values, janet_cstringv("nobreaks"), janet_wrap_integer(CMARK_OPT_NOBREAKS));
    janet_table_put(markable_option_values, janet_cstringv("validate-utf8"), janet_wrap_integer(CMARK_OPT_VALIDATE_UTF8));
    janet_table_put(markable_option_values, janet_cstringv("smart"), janet_wrap_integer(CMARK_OPT_SMART));
    janet_table_put(markable_option_values, janet_cstringv("github-pre-lang"), janet_wrap_integer(CMARK_OPT_GITHUB_PRE_LANG));
    janet_table_put(markable_option_values, janet_cstringv("liberal-html-tag"), janet_wrap_integer(CMARK_OPT_LIBERAL_HTML_TAG));
    janet_table_put(markable_option_values, janet_cstringv("footnotes"), janet_wrap_integer(CMARK_OPT_FOOTNOTES));
    janet_table_put(markable_option_values, janet_cstringv("strikethrough-double-tilde"), janet_wrap_integer(CMARK_OPT_STRIKETHROUGH_DOUBLE_TILDE));
    janet_table_put(markable_option_values, janet_cstringv("table-prefer-style-attributes"), janet_wrap_integer(CMARK_OPT_TABLE_PREFER_STYLE_ATTRIBUTES));
    janet_table_put(markable_option_values, janet_cstringv("full-info-string"), janet_wrap_integer(CMARK_OPT_FULL_INFO_STRING));
    return markable_option_values;
}

/* Extract valid keywords.
 *
 * This is a helper function for use by Markable's wrapping functions.  The
 * underlying parsing-related functions provided by cmark-gfm will accept an
 * integer representing a composite of options. These options are defined as
 * constants in cmark-gfm.h. To make these more ergonomic for use from Janet,
 * Markable provides a mapping from C string values to the integer values
 * for each option (the mapping is provided via a JanetTable collection that is
 * initialised when the module is loaded).
 *
 * This function takes a JanetView value (this is a value representing an
 * indexed collection that can be iterated over in a for-loop). The values in
 * the JanetView collection should all be JanetKeyword values whose C string
 * representation is contained in the JanetTable collection. Invalid options
 * are not handled gracefully. Instead, the function calls janet_panicf and
 * exits.
 *
 * The return value is the composite result of applying bitwise or on each
 * option's value.
 */
int32_t markable_extract_options(JanetView options) {
    int32_t result = CMARK_OPT_DEFAULT;
    JanetTable *option_values = markable_options();
    for (int32_t i = 0; i < options.len; i++) {
        const uint8_t *option = janet_getkeyword(options.items, i);
        Janet option_value = janet_table_get(option_values, janet_wrap_string(option));
        if (janet_checktype(option_value, JANET_NUMBER)) {
            result |= janet_unwrap_integer(option_value);
        } else {
            janet_panicf("invalid option value, :%s", option);
        }
    }
    return result;
}

/* Wrap cmark-gfm's cmark_render_html_with_mem().
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
JANET_FN(cfun_markdown_to_html,
         "(markable/markdown->html str &opt opts exts)",
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
         "- `:tasklist`") {
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

    JanetString html = janet_cstring(output);

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
 * The optional third value is an unsigned integer representing the width at
 * which to wrap the resulting output.
 *
 * The return value is a wrapped JanetString value.
 */
JANET_FN(cfun_markdown_to_plaintext,
         "(markable/markdown->plaintext str &opt opts width)",
         "Convert a string from Markdown to plaintext\n\n"
         "Markable converts Markdown to plaintext using GitHub's cmark-gfm library. "
         "It supports the GitHub-Flavored Markdown specification.\n\n"
         "In addition to the string to convert, the user can provide `opts` and "
         "`width`.\n\n"
         "`opts` is a tuple of keywords. The keywords and the cmark-gfm library "
         "options that they map to are shown below::\n\n"
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
         "`width` is an unsigned integer representing the width at which to wrap. "
         "Defaults to 80.") {
    janet_arity(argc, 1, 3);

    const char *input = janet_getcstring(argv, 0);

    int32_t options;
    if (argc >= 2) {
        options = markable_extract_options(janet_getindexed(argv, 1));
    } else {
        options = markable_default_options;
    }

    uint32_t width;
    if (argc == 3) {
        width = janet_getuinteger(argv, 2);
    } else {
        width = 80;
    }

    cmark_mem *arena_mem = cmark_get_arena_mem_allocator();
    cmark_parser *parser = cmark_parser_new_with_mem(options, arena_mem);

    cmark_parser_feed(parser, input, strlen(input));
    cmark_node *doc = cmark_parser_finish(parser);

    cmark_mem *default_mem = cmark_get_default_mem_allocator();
    char *output = cmark_render_plaintext_with_mem(doc, options, (int)width, default_mem);

    JanetString plaintext = janet_cstring(output);

    default_mem->free(output);
    cmark_arena_reset();

    return janet_wrap_string(plaintext);
}

JANET_MODULE_ENTRY(JanetTable *env) {
    JanetRegExt cfuns[] = {
        JANET_REG("markdown->html", cfun_markdown_to_html),
        JANET_REG("markdown->plaintext", cfun_markdown_to_plaintext),
        JANET_REG_END
    };
    janet_cfuns_ext(env, "markable", cfuns);
    cmark_gfm_core_extensions_ensure_registered();
}
