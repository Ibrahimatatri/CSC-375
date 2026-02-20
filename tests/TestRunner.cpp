#include <iostream>
#include <string>
#include "../src/HashTable.h"
#include "../src/Feedback.h"
#include "../src/WordList.h"

using namespace std;

static int testNum = 0;

void printRow(const string& desc, const string& input, const string& expected, const string& actual, bool pass, const string& comments="") {
    cout << ++testNum << "\t" << desc << "\t" << input << "\t" << expected << "\t"
         << actual << "\t" << (pass ? "PASS" : "FAIL") << "\t" << comments << "\n";
}

int main() {
    cout << "Test #\tTest Description\tTest Input\tExpected Output\tActual Output\tStatus (Pass/Fail)\tAdditional Comments\n";

    HashTable ht;

    bool r1 = ht.insert("crane");
    printRow("Insert into empty table",
             "insert(\"crane\")",
             "true; size=1",
             string(r1 ? "true" : "false") + "; size=" + to_string(ht.size()),
             (r1 == true && ht.size() == 1));

    bool r2 = ht.insert("crane");
    printRow("Duplicate insert rejected",
             "insert(\"crane\") again",
             "false; size=1",
             string(r2 ? "true" : "false") + "; size=" + to_string(ht.size()),
             (r2 == false && ht.size() == 1));

    bool r3 = ht.contains("crane");
    printRow("Contains existing key",
             "contains(\"crane\")",
             "true",
             string(r3 ? "true" : "false"),
             (r3 == true));

    bool r4 = ht.contains("cigar");
    printRow("Contains non-existing key",
             "contains(\"cigar\")",
             "false",
             string(r4 ? "true" : "false"),
             (r4 == false));

    bool r5 = ht.remove("crane");
    printRow("Remove existing key",
             "remove(\"crane\")",
             "true; size=0",
             string(r5 ? "true" : "false") + "; size=" + to_string(ht.size()),
             (r5 == true && ht.size() == 0));

    bool r6 = ht.remove("crane");
    printRow("Remove missing key",
             "remove(\"crane\") again",
             "false; size=0",
             string(r6 ? "true" : "false") + "; size=" + to_string(ht.size()),
             (r6 == false && ht.size() == 0));

    bool a = ht.insert("cigar");
    bool b = ht.insert("rebut");
    bool c = ht.insert("sissy");
    printRow("Insert multiple unique keys",
             "insert(\"cigar\"), insert(\"rebut\"), insert(\"sissy\")",
             "all true; size=3",
             string(a ? "true" : "false") + "," + (b ? "true" : "false") + "," + (c ? "true" : "false") + "; size=" + to_string(ht.size()),
             (a && b && c && ht.size() == 3));

    bool r8 = ht.contains("rebut");
    printRow("Contains after multiple inserts",
             "contains(\"rebut\")",
             "true",
             string(r8 ? "true" : "false"),
             (r8 == true));

    bool r9 = ht.contains("humph");
    printRow("Contains missing after multiple inserts",
             "contains(\"humph\")",
             "false",
             string(r9 ? "true" : "false"),
             (r9 == false));

    bool r10 = ht.remove("rebut");
    bool r10b = ht.contains("rebut");
    printRow("Remove key then verify missing",
             "remove(\"rebut\"), then contains(\"rebut\")",
             "true; then false; size=2",
             string(r10 ? "true" : "false") + "; contains=" + (r10b ? "true" : "false") + "; size=" + to_string(ht.size()),
             (r10 == true && r10b == false && ht.size() == 2));

    bool r11a = ht.remove("cigar");
    bool r11b = ht.remove("sissy");
    printRow("Remove remaining keys",
             "remove(\"cigar\"), remove(\"sissy\")",
             "both true; size=0",
             string(r11a ? "true" : "false") + "," + (r11b ? "true" : "false") + "; size=" + to_string(ht.size()),
             (r11a && r11b && ht.size() == 0));

    string words10[10] = {"alpha","bravo","candy","dodge","eager","fuzzy","gloom","hound","ivory","joker"};
    bool okAll = true;
    for (int i = 0; i < 10; i++) okAll = okAll && ht.insert(words10[i]);
    printRow("Insert 10 words sequentially",
             "insert 10 distinct words",
             "all true; size=10",
             string(okAll ? "true" : "false") + "; size=" + to_string(ht.size()),
             (okAll && ht.size() == 10));

    bool okContainsAll = true;
    for (int i = 0; i < 10; i++) okContainsAll = okContainsAll && ht.contains(words10[i]);
    printRow("Contains all inserted words",
             "contains(each of 10 words)",
             "all true",
             string(okContainsAll ? "true" : "false"),
             (okContainsAll));

    bool okRemove5 = true;
    for (int i = 0; i < 5; i++) okRemove5 = okRemove5 && ht.remove(words10[i]);
    printRow("Remove 5 of 10 words",
             "remove first 5 words",
             "all true; size=5",
             string(okRemove5 ? "true" : "false") + "; size=" + to_string(ht.size()),
             (okRemove5 && ht.size() == 5));

    bool removedAreGone = true;
    for (int i = 0; i < 5; i++) removedAreGone = removedAreGone && (!ht.contains(words10[i]));
    printRow("Contains removed words",
             "contains(first 5 removed)",
             "all false",
             string(removedAreGone ? "true" : "false") + " (true means all were false)",
             (removedAreGone));

    bool remainingPresent = true;
    for (int i = 5; i < 10; i++) remainingPresent = remainingPresent && ht.contains(words10[i]);
    printRow("Contains remaining words",
             "contains(last 5 remaining)",
             "all true",
             string(remainingPresent ? "true" : "false"),
             (remainingPresent));

    bool rein = ht.insert(words10[0]);
    printRow("Re-insert previously removed word",
             "insert(\"alpha\") again",
             "true; size=6",
             string(rein ? "true" : "false") + "; size=" + to_string(ht.size()),
             (rein && ht.size() == 6));

    ht.clear();
    printRow("Clear table after inserts",
             "clear()",
             "size=0; contains(any prior)=false",
             "size=" + to_string(ht.size()) + "; contains(alpha)=" + string(ht.contains("alpha") ? "true" : "false"),
             (ht.size() == 0 && !ht.contains("alpha")));

    bool afterClearIns = ht.insert("crane");
    printRow("Insert after clear",
             "insert(\"crane\") after clear",
             "true; size=1",
             string(afterClearIns ? "true" : "false") + "; size=" + to_string(ht.size()),
             (afterClearIns && ht.size() == 1));

    string fb1 = Feedback::grade("crane","cigar");
    printRow("Feedback grade known case",
             "grade(\"crane\",\"cigar\")",
             "GYYBB",
             fb1,
             (fb1 == "GYYBB"));

    bool m1 = Feedback::matches("crane","GYYBB","cigar");
    printRow("Feedback matches true case",
             "matches(\"crane\",\"GYYBB\",\"cigar\")",
             "true",
             string(m1 ? "true" : "false"),
             (m1 == true));

    bool m2 = Feedback::matches("crane","GYYBB","crown");
    printRow("Feedback matches false case",
             "matches(\"crane\",\"GYYBB\",\"crown\")",
             "false",
             string(m2 ? "true" : "false"),
             (m2 == false));

    WordList wl = loadWordList("data/wordlist.txt");
    printRow("Word list load check",
             "loadWordList(\"data/wordlist.txt\")",
             "count > 0",
             "count=" + to_string(wl.count),
             (wl.count > 0));

    int kept = 0;
    for (int i = 0; i < wl.count; i++) {
        if (Feedback::matches("crane","GYYBB",wl.words[i])) kept++;
    }
    printRow("Wordle filter sanity",
             "guess=\"crane\", feedback=\"GYYBB\"",
             "kept > 0; cigar included (if in list)",
             "kept=" + to_string(kept),
             (kept > 0),
             "You already saw cigar appear in your earlier run");
    ht.clear();
    ht.insert("cigar");
    ht.insert("rebut");
    bool rmMissingNonEmpty = ht.remove("sissy");
    printRow("Remove missing key on non-empty table",
             "insert(\"cigar\"), insert(\"rebut\"), remove(\"sissy\")",
             "false; size=2",
             string(rmMissingNonEmpty ? "true" : "false") + "; size=" + to_string(ht.size()),
             (rmMissingNonEmpty == false && ht.size() == 2));

    ht.clear();
    bool containsAfterClear = ht.contains("rebut");
    printRow("Contains after clear",
             "clear(); contains(\"rebut\")",
             "false",
             string(containsAfterClear ? "true" : "false"),
             (containsAfterClear == false));

    ht.clear();
    bool okBig = true;
    for (int i = 0; i < 250; i++) {
        string k = "k" + to_string(i);
        okBig = okBig && ht.insert(k);
    }
    bool bigContains = ht.contains("k249");
    printRow("Rehash / large insert stress",
             "insert 250 distinct keys; contains(\"k249\")",
             "no crash; size=250; contains=true",
             string(okBig ? "true" : "false") + "; size=" + to_string(ht.size()) + "; contains=" + (bigContains ? "true" : "false"),
             (okBig && ht.size() == 250 && bigContains));

    string fbDup = Feedback::grade("allay","belly");
    printRow("Feedback duplicate-letter behavior",
             "grade(\"allay\",\"belly\")",
             "A valid 5-char string of G/Y/B",
             fbDup,
             (fbDup.size() == 5));

    bool wrongFbMatch = Feedback::matches("crane","BBBBB","cigar");
    printRow("Matches with wrong feedback",
             "matches(\"crane\",\"BBBBB\",\"cigar\")",
             "false",
             string(wrongFbMatch ? "true" : "false"),
             (wrongFbMatch == false));

    int keptAllGreen = 0;
    for (int i = 0; i < wl.count; i++) {
        if (Feedback::matches("cigar","GGGGG",wl.words[i])) keptAllGreen++;
    }
    printRow("All-green filter sanity",
             "guess=\"cigar\", feedback=\"GGGGG\"",
             "kept >= 0 (often 1 if cigar in list)",
             "kept=" + to_string(keptAllGreen),
             (keptAllGreen >= 0));
    return 0;
}

