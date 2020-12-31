(declare-project
  :name "Markable"
  :description "Janet bindings for GitHub's fork of cmark"
  :author "Michael Camilleri"
  :license "MIT"
  :url "https://github.com/pyrmont/markable"
  :repo "git+https://github.com/pyrmont/markable"
  :dependencies ["https://github.com/pyrmont/testament"])


(def is-win (= :windows (os/which)))


(def cflags
  (array/concat
    @["-Icmark-gfm/src" "-Icmark-gfm/extensions"]
    (if is-win
      ["-Iinclude/win"]
      ["-Iinclude/unix" "-Wno-unused-parameter"])))


(declare-native
  :name "markable"
  :cflags [;default-cflags ;cflags]
  :lflags [;default-lflags]
  :headers @["src/markable.h"]
  :source @["cmark-gfm/src/blocks.c"
            "cmark-gfm/src/buffer.c"
            "cmark-gfm/src/cmark.c"
            "cmark-gfm/src/cmark_ctype.c"
            "cmark-gfm/src/commonmark.c"
            "cmark-gfm/src/footnotes.c"
            "cmark-gfm/src/houdini_href_e.c"
            "cmark-gfm/src/houdini_html_e.c"
            "cmark-gfm/src/houdini_html_u.c"
            "cmark-gfm/src/html.c"
            "cmark-gfm/src/inlines.c"
            "cmark-gfm/src/iterator.c"
            "cmark-gfm/src/latex.c"
            "cmark-gfm/src/linked_list.c"
            "cmark-gfm/src/man.c"
            "cmark-gfm/src/map.c"
            "cmark-gfm/src/node.c"
            "cmark-gfm/src/plaintext.c"
            "cmark-gfm/src/plugin.c"
            "cmark-gfm/src/references.c"
            "cmark-gfm/src/registry.c"
            "cmark-gfm/src/render.c"
            "cmark-gfm/src/scanners.c"
            "cmark-gfm/src/syntax_extension.c"
            "cmark-gfm/src/utf8.c"
            "cmark-gfm/src/xml.c"

            # cmark Extensions
            "cmark-gfm/extensions/autolink.c"
            "cmark-gfm/extensions/core-extensions.c"
            "cmark-gfm/extensions/ext_scanners.c"
            "cmark-gfm/extensions/strikethrough.c"
            "cmark-gfm/extensions/table.c"
            "cmark-gfm/extensions/tagfilter.c"
            "cmark-gfm/extensions/tasklist.c"

            # Janet wrapper
            "src/arena.c"
            "src/converter.c"
            "src/markable.c"])
