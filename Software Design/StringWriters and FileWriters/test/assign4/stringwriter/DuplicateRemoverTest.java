package assign4.stringwriter;

import junit.framework.TestCase;

public class DuplicateRemoverTest extends TestCase {
    public void testRemoveDuplicateWhenThereIsOneDuplicate() {
        assertEquals("no duplicate", DuplicateRemover.convert("no no duplicate"));
    }

    public void testRemoveDuplicateWhenThereAreMultipleDuplicate() {
        assertEquals("no duplicate", DuplicateRemover.convert("no no duplicate duplicate"));
    }

    public void testDoNotRemoveDuplicateForDifferenceInCase() {
        assertEquals("yes Yes duplicate", DuplicateRemover.convert("yes Yes duplicate"));
    }

    public void testDuplicateRemoverForMoreThanOneSpaceBetweenWords() {
        assertEquals("no  duplicate", DuplicateRemover.convert("no     no    duplicate"));
    }

    public void testDuplicateRemoverForBlankString() {
        assertEquals("", DuplicateRemover.convert(""));
    }

    public void testDuplicateRemoverStringWithMultipleSpaces() {
        assertEquals("", DuplicateRemover.convert("        "));
    }

    public void testAddedForCoverage() {
        DuplicateRemover dup = new DuplicateRemover();
    }
}
