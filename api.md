# Markable API

## markable

[markdown-&gt;html](#markdown-html)

## markdown-&gt;html

**cfunction**  | [source][1]

```janet
(markable/markdown->html str &opt opts exts)
```

Convert `str` from Markdown to HTML. `opts` is a tuple of keywords that match those that can be provided to the cmark-gfm library. Keywords are of the form such that CMARK_OPT_SMART becomes :smart. Option names that are in snake-case should be converted to kebab-case (e.g. CMARK_OPT_VALIDATE_UTF8 becomes :validate-utf8). `exts` is a tuple of keywords that match the name of the extensions in the GitHub-Flavored Markdown Spec.

[1]: src/markable/converter.c#L74

