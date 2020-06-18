#include "cmark-gfm.h"
#include "markable.h"

JANET_THREAD_LOCAL JanetTable *markable_option_values = NULL;

static void markable_options() {
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
}

JANET_MODULE_ENTRY(JanetTable *env) {
    markable_options();
    register_converter(env);
}
