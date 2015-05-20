package assign4.stringwriter;

import junit.framework.TestCase;

public class DuplicateRemoverWithLowerCaseTest extends TestCase {
    StringWriter stringWriter;
    String input = "ComBINe ComBINe oPERATioNS";
    String expected = "combine operations";

    public void setUp() {
        stringWriter = new StringWriter();
    }

    public void testCombineDuplicateRemoverWithLowerCaseConverter() {
        stringWriter.setTransformer(DuplicateRemover::convert, LowerCaseConverter::convert);
        stringWriter.write(input);
        assertEquals(expected, stringWriter.read());
    }

    public void testCombineLowerCaseConverterWithDuplicateRemover() {
        stringWriter.setTransformer(LowerCaseConverter::convert, DuplicateRemover::convert);
        stringWriter.write(input);
        assertEquals(expected, stringWriter.read());
    }
}
