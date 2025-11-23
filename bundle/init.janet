(import ./spork/build-rules :as br)
(import ./spork/cc :as cc)
(import ./spork/declare-cc :as declare)

(def- seps {:windows "\\" :mingw "\\" :cygwin "\\"})
(def- s (get seps (os/which) "/"))

(def- windows? (index-of (os/which) [:cygwin :windows]))

(defn cflags [c windows?]
  (array/concat @[]
                (get c :common)
                (get c (if windows? :windows :posix))))

(defn build [manifest &]
  (def configs (get-in manifest [:info :native]))
  (def rules @{})
  (with-dyns [cc/*rules* rules
              declare/*build-root* "_build"]
    (declare/declare-project :name "fake project")
    (os/mkdir "_build")
    (os/mkdir "_build/release")
    (os/mkdir "_build/release/static")
    (each config configs
      (declare/declare-native
        :name (get config :name)
        :source (get config :files)
        :cflags (cflags (get config :cflags) windows?)))
    (br/build-run rules "build")))

(defn install [manifest &]
  # man pages
  (def manpages (get-in manifest [:info :manpage] []))
  (os/mkdir (string (dyn :syspath) s "man"))
  (os/mkdir (string (dyn :syspath) s "man" s "man1"))
  (each mp manpages
    (bundle/add-file manifest mp))
  # native modules
  (def natives (get-in manifest [:info :native]))
  (each nat natives
    (def prefix (get nat :prefix))
    (if prefix (bundle/add-directory manifest prefix))
    (each ext [".a" ".meta.janet" ".so"]
      (def ext1 (if (and windows? (= ".so" ext))
                  ".dll"
                  ext))
      (def filename (string (get nat :name) ext1))
      (def src (string "_build" s "release" s filename))
      (def dest (string (if prefix (string prefix s)) filename))
      (bundle/add-file manifest src dest)))
  # source modules
  (def prefix (get-in manifest [:info :source :prefix]))
  (def srcs (get-in manifest [:info :source :files] []))
  (if prefix (bundle/add-directory manifest prefix))
  (each src srcs
    (bundle/add manifest src (string prefix s src)))
  # executables
  (def bins (get-in manifest [:info :executable] []))
  (each bin bins
    (bundle/add-bin manifest bin)))
