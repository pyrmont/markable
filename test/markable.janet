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


(deftest tables
  (def markdown "| foo | bar |\n| --- | --- |\n| baz | bim |")
  (def expect (string "<table>\n"
                      "<thead>\n<tr>\n<th>foo</th>\n<th>bar</th>\n</tr>\n</thead>\n"
                      "<tbody>\n<tr>\n<td>baz</td>\n<td>bim</td>\n</tr>\n</tbody>\n"
                      "</table>\n"))
  (is (= expect
         (markable/markdown->html markdown [] [:table]))))


(deftest thread-safety
  (thread/new (fn [parent]
                (thread/send parent (markable/markdown->html "Hello **world**"))))
  (is (= "<p>Hello <strong>world</strong></p>\n" (thread/receive))))


(run-tests!)
