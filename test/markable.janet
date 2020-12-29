(import testament :prefix "" :exit true)
(import ../build/markable :as markable)


(deftest plaintext
  (is (= "<p>Hello world</p>\n"
         (markable/markdown->html "Hello world"))))


(deftest strong
  (is (= "<p>Hello <strong>world</strong></p>\n"
         (markable/markdown->html "Hello **world**"))))


(deftest footnote
  (def expect (string "<p>Hello<sup class=\"footnote-ref\"><a href=\"#fn1\" id=\"fnref1\">1</a></sup></p>\n"
                      "<section class=\"footnotes\">\n"
                      "<ol>\n"
                      "<li id=\"fn1\">\n"
                      "<p>world <a href=\"#fnref1\" class=\"footnote-backref\">\xE2\x86\xA9</a></p>\n"
                      "</li>\n"
                      "</ol>\n"
                      "</section>\n"))
  (is (= expect
         (markable/markdown->html "Hello[^1]\n\n[^1]: world" [:footnotes]))))


(deftest thread-safety
  (thread/new (fn [parent]
                (thread/send parent (markable/markdown->html "Hello **world**"))))
  (is (= "<p>Hello <strong>world</strong></p>\n" (thread/receive))))


(run-tests!)
