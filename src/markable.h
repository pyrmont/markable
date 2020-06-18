#ifndef MARKABLE_H
#define MARKABLE_H

#include <janet.h>

/* Mapping of keywords to option values.
 *
 * The cmark-gfm library defines a number of integer values that represent
 * options used in parsing Markdown. Markable makes these easier to use from
 * Janet by allowing the options to be enabled with keywords. To simplify the
 * matching of these keywords to the specific values, Markable associates the
 * keywords with the values in a JanetTable object.
 */
extern JANET_THREAD_LOCAL JanetTable *markable_option_values;

/* Register functions for the converter submodule. */
void register_converter(JanetTable *env);

#endif /* end of include guard */
