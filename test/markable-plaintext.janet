(import ../deps/testament :prefix "" :exit true)
(import ../_build/release/markable :as markable)


(deftest heading
  (is (= "Heading\n"
         (markable/markdown->plaintext "# Heading\n"))))


(deftest emphasis
  (is (= "emphasis\n"
         (markable/markdown->plaintext "*emphasis*\n"))))


(deftest block-quote
  (is (= "block-quote\n"
         (markable/markdown->plaintext "> block-quote\n"))))


(deftest image
  (is (= "image\n"
         (markable/markdown->plaintext "![image](image.png)\n"))))


(deftest inline-link
  (is (= "inline-link\n"
         (markable/markdown->plaintext "[inline-link](doc.html)\n"))))


(deftest reference-link
  (is (= "reference-link\n"
         (markable/markdown->plaintext (string "[reference-link][id]\n"
                                               "\n"
                                               "[id]: doc.html\n")))))


(deftest code-span
  (is (= "code-span\n"
         (markable/markdown->plaintext "`code-span`\n"))))


(deftest code-block
  (is (= "code-block\n"
         (markable/markdown->plaintext (string "```\n"
                                               "code-block\n"
                                               "```\n")))))

(deftest thematic-break
  (is (= "Hi\n\nBye\n"
         (markable/markdown->plaintext (string "Hi\n"
                                               "---\n"
                                               "Bye\n")))))


(deftest thread-safety
  (def chan-1 (ev/thread-chan 1))
  (def chan-2 (ev/thread-chan 1))
  (ev/do-thread (ev/give chan-1 (markable/markdown->plaintext "**hey**\n")))
  (ev/do-thread (ev/give chan-2 (markable/markdown->plaintext "# Look!\n")))
  (is (= "hey\n" (ev/take chan-1)))
  (is (= "Look!\n" (ev/take chan-2))))


(run-tests!)
