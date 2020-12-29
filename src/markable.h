#ifndef MARKABLE_H
#define MARKABLE_H

#include <cmark-gfm.h>
#include <node.h>
#include <cmark-gfm-extension_api.h>
#include <syntax_extension.h>
#include <parser.h>
#include <cmark-gfm-core-extensions.h>
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

/* The 'default' options to apply to parsing.
 *
 * This value is initially set to CMARK_OPT_DEFAULT. It is used to set the
 * options that are passed to cmark-gfm's parser if the user does not expressly
 * provide any options.
 *
 * TODO: Provide a mechanism to update this value.
 */
extern JANET_THREAD_LOCAL int32_t markable_default_options;

/* Extract options from tuple of keywords.
 *
 * In multiple functions, Markable allows a user to pass a JanetTuple listing
 * the options that should be applied during parsing. This function takes a
 * JanetView of the tuple and extracts the options, performing bitwise
 * operations to produce a final composite value.
 */
int markable_extract_options(JanetView options);

/* Register functions for the converter submodule. */
void markable_register_converter(JanetTable *env);

/* Register functions for the parser submodule. */
void markable_register_parser(JanetTable *env);

#endif /* end of include guard */
