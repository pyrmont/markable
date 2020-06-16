#include <stdio.h>
#include <string.h>
#include <janet.h>
#include "cmark-gfm.h"

static JANET_THREAD_LOCAL int perm_options = CMARK_OPT_DEFAULT;
static JANET_THREAD_LOCAL JanetTable *option_values = NULL;

static Janet cfun_markdown_to_html(int32_t argc, Janet *argv) {
    int temp_options = CMARK_OPT_DEFAULT;

    janet_arity(argc, 1, 2);

    const char *input = janet_getcstring(argv, 0);

    if (argc == 2) {
        JanetView options = janet_getindexed(argv, 1);
        for (int32_t i = 0; i < options.len; i++) {
            const uint8_t *option = janet_getkeyword(options.items, i);
            Janet option_value = janet_table_get(option_values, janet_wrap_string(option));
            if (janet_checktype(option_value, JANET_NUMBER)) {
                temp_options |= janet_unwrap_integer(option_value);
            } else {
                janet_panicf("invalid option value, :%s", option);
            }
        }
    } else {
        temp_options = perm_options;
    }

    char *output = cmark_markdown_to_html(input, strlen(input), temp_options);
    JanetString html = janet_string((uint8_t *)output, strlen(output));
    free(output);

    return janet_wrap_string(html);
}

static void markable_options() {
    option_values = janet_table(0);
    janet_gcroot(janet_wrap_table(option_values));
    janet_table_put(option_values, janet_cstringv("sourcepos"), janet_wrap_integer(CMARK_OPT_SOURCEPOS));
    janet_table_put(option_values, janet_cstringv("hardbreaks"), janet_wrap_integer(CMARK_OPT_HARDBREAKS));
    janet_table_put(option_values, janet_cstringv("unsafe"), janet_wrap_integer(CMARK_OPT_UNSAFE));
    janet_table_put(option_values, janet_cstringv("nobreaks"), janet_wrap_integer(CMARK_OPT_NOBREAKS));
    janet_table_put(option_values, janet_cstringv("validate-uft8"), janet_wrap_integer(CMARK_OPT_VALIDATE_UTF8));
    janet_table_put(option_values, janet_cstringv("smart"), janet_wrap_integer(CMARK_OPT_SMART));
    janet_table_put(option_values, janet_cstringv("github-pre-lang"), janet_wrap_integer(CMARK_OPT_GITHUB_PRE_LANG));
    janet_table_put(option_values, janet_cstringv("liberal-html-tag"), janet_wrap_integer(CMARK_OPT_LIBERAL_HTML_TAG));
    janet_table_put(option_values, janet_cstringv("footnotes"), janet_wrap_integer(CMARK_OPT_FOOTNOTES));
    janet_table_put(option_values, janet_cstringv("strikethrough-double-tilde"), janet_wrap_integer(CMARK_OPT_STRIKETHROUGH_DOUBLE_TILDE));
    janet_table_put(option_values, janet_cstringv("table-prefer-style-attributes"), janet_wrap_integer(CMARK_OPT_TABLE_PREFER_STYLE_ATTRIBUTES));
    janet_table_put(option_values, janet_cstringv("full-info-string"), janet_wrap_integer(CMARK_OPT_FULL_INFO_STRING));
}

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

JANET_MODULE_ENTRY(JanetTable *env) {
    markable_options();
    janet_cfuns(env, "markable", cfuns);
}
