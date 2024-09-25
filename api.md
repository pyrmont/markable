# Markable API

## markable

[markdown-&gt;html](#markdown-html), [markdown-&gt;plaintext](#markdown-plaintext)

## markdown-&gt;html

**cfunction**  

```janet
(markdown->html str &opt opts exts)
```

Convert a string from Markdown to HTML

Markable converts Markdown to HTML using the cmark-gfm library. This is GitHub's implementation of the cmark Common Mark parser. It supports the GitHub-Flavored Markdown specification.

In addition to the string to convert, the user can provide `opts` and `exts`.

`opts` is a tuple of keywords. The keywords and the cmark-gfm library options that they map to is shown below::

```
cmark-gfm                                Markable
=======================================================================
CMARK_OPT_SOURCEPOS                      :sourcepos
CMARK_OPT_HARDBREAKS                     :hardbreaks
CMARK_OPT_UNSAFE                         :unsafe
CMARK_OPT_VALIDATE_UTF8                  :validate-utf8
CMARK_OPT_SMART                          :smart
CMARK_OPT_GITHUB_PRE_LANG                :github-pre-lang
CMARK_OPT_LIBERAL_HTML_TAG               :liberal-html-tag
CMARK_OPT_FOOTNOTES                      :footnotes
CMARK_OPT_STRIKETHROUGH_DOUBLE_TILDE     :strikethrough-double-tilde
CMARK_OPT_TABLE_PREFER_STYLE_ATTRIBUTES  :table-prefer-style-attributes
CMARK_OPT_FULL_INFO_STRING               :full-info-string
```

`exts` is a tuple of keywords that match the name of the extensions in the GitHub-Flavored Markdown specification. These extensions are currently:

- `:autolink`
- `:strikethrough`
- `:table`
- `:tagfilter`
- `:tasklist`


## markdown-&gt;plaintext

**cfunction**  

```janet
(markdown->plaintext str &opt opts width)
```

Convert a string from Markdown to plaintext

Markable converts Markdown to plaintext using GitHub's cmark-gfm library. It supports the GitHub-Flavored Markdown specification.

In addition to the string to convert, the user can provide `opts` and `width`.

`opts` is a tuple of keywords. The keywords and the cmark-gfm library options that they map to are shown below::

```
cmark-gfm                                Markable
=======================================================================
CMARK_OPT_SOURCEPOS                      :sourcepos
CMARK_OPT_HARDBREAKS                     :hardbreaks
CMARK_OPT_UNSAFE                         :unsafe
CMARK_OPT_VALIDATE_UTF8                  :validate-utf8
CMARK_OPT_SMART                          :smart
CMARK_OPT_GITHUB_PRE_LANG                :github-pre-lang
CMARK_OPT_LIBERAL_HTML_TAG               :liberal-html-tag
CMARK_OPT_FOOTNOTES                      :footnotes
CMARK_OPT_STRIKETHROUGH_DOUBLE_TILDE     :strikethrough-double-tilde
CMARK_OPT_TABLE_PREFER_STYLE_ATTRIBUTES  :table-prefer-style-attributes
CMARK_OPT_FULL_INFO_STRING               :full-info-string
```

`width` is an unsigned integer representing the width at which to wrap. Defaults to 80.


