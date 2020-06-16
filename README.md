# Markable

[![Build Status](https://github.com/pyrmont/markable/workflows/build/badge.svg)](https://github.com/pyrmont/markable/actions?query=workflow%3Abuild)

Markable provides Janet bindings for GitHub's fork of the CommonMark library,
[cmark-gfm](https://github.com/github/cmark-gfm).

## Installation

Add the dependency to your `project.janet` file:

```
(declare-project
  :dependencies ["https://github.com/pyrmont/markable"])
```

## Usage

Markable can be used like this:

```
(import markable)

(markable/markdown->html "Hello **world**") # => "<p>Hello <strong>world</strong></p>\n"
```

## Bugs

Found a bug? I'd love to know about it. The best way is to report your bug in
the [Issues][] section on GitHub.

[Issues]: https://github.com/pyrmont/markable/issues

## Licence

Markable is licensed under the MIT Licence. See [LICENSE][] for more details.

[LICENSE]: https://github.com/pyrmont/markable/blob/master/LICENSE
