# Markable

[![Build Status][status-badge]][status-result]

[status-badge]: https://github.com/pyrmont/markable/workflows/build/badge.svg
[status-result]: https://github.com/pyrmont/markable/actions?query=workflow%3Abuild

Markable provides Janet bindings for GitHub's fork of the CommonMark library,
[cmark-gfm][].

[cmark-gfm]: https://github.com/github/cmark-gfm

## Requirements

Markable includes commands to compile cmark-gfm from source. As such, your
system needs to meet the [requirements][cmark-gfm-reqs] for cmark-gfm.

[cmark-gfm-reqs]: https://github.com/github/cmark-gfm#installing

## Installation

Add the dependency to your `project.janet` file:

```
(declare-project
  :dependencies ["https://github.com/pyrmont/markable"])
```

Then run:

```shell
$ jpm deps
```

## Usage

Markable provides the function `(markdown->html str &opt opts exts)`.

It can be used like this:

```
(import markable)

(markable/markdown->html "Hello **world**") # => "<p>Hello <strong>world</strong></p>\n"
```

The `opts` and `exts` arguments are optional.

`opts` can be a tuple containing one or more of the following keywords:

```
:footnotes         Parse footnotes.
:full-info-string  Include the remainder of the info string in code blocks in
                   a separate attribute.
:github-pre-lang   Use GitHub-style tags for code blocks.
:hardbreaks        Render soft breaks (newlines inside paragraphs in the
                   CommonMark source) as hard line breaks in the target format.
                   If this option is specified, hard wrapping is disabled for
                   CommonMark output.
:liberal-html-tag  Be liberal in interpreting inline HTML tags.
:nobreaks          Render soft breaks as spaces. If this option is specified,
                   hard wrapping is disabled for all output formats
:smart             Use smart punctuation. Straight double and single quotes
                   will be rendered as curly quotes, depending on their
                   position, -- will be rendered as an en-dash, --- will be
                   rendered as an em-dash and ... will be rendered as ellipses.
:sourcepos         Include the source position attribute.
:unsafe            Render raw HTML or potentially dangerous URLs. (Raw HTML is
                   replaced by a placeholder comment; potentially dangerous
                   URLs are replaced by empty strings.) Dangerous URLs are
                   those that begin with javascript:, vbscript:, file:, or
                   data: (except for image/png, image/gif, image/jpeg, or
                   image/webp mime types).
:validate-utf8     Validate UTF-8, replacing illegal sequences with U+FFFD.
```

The following options can also be set but depend on extensions being enabled:

```
:strikethrough-double-tilde     Only parse strikethroughs if surrounded by
                                exactly two tildes.
:table-prefer-style-attributes  Use style attributes to align table cells
                                instead of align attributes.
```

`exts` can be a tuple containing one or more of the following keywords:
`:autolink`, `:strikethrough`, `:table`, `:tagfilter` and `:tasklist`. More
details about each extension is available in the [GitHub Flavored Markdown
Spec][spec].

[spec]: https://github.github.com/gfm/

## Bugs

Found a bug? I'd love to know about it. The best way is to report your bug in
the [Issues][] section on GitHub.

[Issues]: https://github.com/pyrmont/markable/issues

## Licence

Markable is licensed under the MIT Licence. See [LICENSE][] for more details.

[LICENSE]: https://github.com/pyrmont/markable/blob/master/LICENSE
