# Markable

[![Build Status](https://github.com/pyrmont/markable/workflows/build/badge.svg)](https://github.com/pyrmont/markable/actions?query=workflow%3Abuild)

Markable provides Janet bindings for GitHub's fork of the CommonMark library,
[cmark-gfm](https://github.com/github/cmark-gfm).

## Requirements

Markable includes commands to compile cmark-gfm from source. As such, your
system needs to meet [the requirements](https://github.com/github/cmark-gfm#installing)
for cmark-gfm.

### Windows support

While cmark-gfm ostensibly supports building on Windows, I haven't been able to
get this to work. If you succeed in doing so, please consider submitting
[a PR](https://github.com/pyrmont/markable/pulls).

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
