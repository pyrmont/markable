#include "markable.h"

JANET_THREAD_LOCAL JanetTable *markable_option_values = NULL;
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
    janet_table_put(markable_option_values, janet_cstringv("validate-uft8"), janet_wrap_integer(CMARK_OPT_VALIDATE_UTF8));
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

JANET_MODULE_ENTRY(JanetTable *env) {
    markable_register_converter(env);
    cmark_gfm_core_extensions_ensure_registered();
}
