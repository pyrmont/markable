(import testament :prefix "" :exit true)
(import ../build/markable :as markable)


(deftest plaintext
  (is (= "<p>Hello world</p>\n"
         (markable/markdown->html "Hello world"))))


(deftest strong
  (is (= "<p>Hello <strong>world</strong></p>\n"
         (markable/markdown->html "Hello **world**"))))


(deftest footnote
  (def expect (string "<p>Hello<sup class=\"footnote-ref\"><a href=\"#fn-1\" id=\"fnref-1\" data-footnote-ref>1</a></sup></p>\n"
                      "<section class=\"footnotes\" data-footnotes>\n"
                      "<ol>\n"
                      "<li id=\"fn-1\">\n"
                      "<p>world <a href=\"#fnref-1\" class=\"footnote-backref\" data-footnote-backref aria-label=\"Back to content\">\xE2\x86\xA9</a></p>\n"
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
