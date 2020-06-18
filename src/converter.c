#include <stdio.h>
#include <string.h>
#include "cmark-gfm.h"
#include "markable.h"

static JANET_THREAD_LOCAL int perm_options = CMARK_OPT_DEFAULT;

static Janet cfun_markdown_to_html(int32_t argc, Janet *argv) {
    int temp_options = CMARK_OPT_DEFAULT;

    janet_arity(argc, 1, 2);

    const char *input = janet_getcstring(argv, 0);

    if (argc == 2) {
        JanetView options = janet_getindexed(argv, 1);
        for (int32_t i = 0; i < options.len; i++) {
            const uint8_t *option = janet_getkeyword(options.items, i);
            Janet option_value = janet_table_get(markable_option_values, janet_wrap_string(option));
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

void register_converter(JanetTable *env) {
    janet_cfuns(env, "markable", cfuns);
}
