(declare-project
  :name "Markable"
  :description "Janet bindings for GitHub's fork of cmark"
  :author "Michael Camilleri"
  :license "MIT"
  :url "https://github.com/pyrmont/markable"
  :repo "git+https://github.com/pyrmont/markable"
  :dependencies ["https://github.com/pyrmont/testament"])


(def cflags
  (case (os/which)
    :macos
    ["-Icmark-gfm/build/src" "-Icmark-gfm/src"]

    ["-Icmark-gfm/build/src" "-Icmark-gfm/src" "-std=c99" "-Wall" "-Wextra" "-O3"]))


(rule "cmark-gfm/build/src/cmark-gfm_export.h" []
      (os/mkdir "cmark-gfm/build")
      (os/cd "cmark-gfm/build")
      (assert (zero? (os/execute ["cmake" ".."] :p)))
      (assert (zero? (os/execute ["make"] :p)))
      (os/cd "../.."))


(rule "build" ["cmark-gfm/build/src/cmark-gfm_export.h"])


(rule "clean-libcmark-gfm" []
      (when (os/stat "cmark-gfm/build")
        (rm "cmark-gfm/build")
        (print "Deleted library build directory.")))


(add-dep "clean" "clean-libcmark-gfm")


(declare-native
  :name "markable"
  :cflags cflags
  :lflags ["-Lcmark-gfm/build/src" "-l:libcmark-gfm.a"]
  :source ["src/markable.c"])
