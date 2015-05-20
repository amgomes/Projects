package assign4.stringwriter;

import junit.framework.TestCase;

public class DuplicateRemoverWithUpperCaseTest extends TestCase {
    StringWriter stringWriter;
    String input = "ComBINe ComBINe oPERATioNS";
    String expected = "COMBINE OPERATIONS";

    public void setUp() {
        stringWriter = new StringWriter();
    }
    public void testCombineDuplicateRemoverWithUpperCaseConverter() {
        stringWriter.setTransformer(DuplicateRemover::convert, UpperCaseConverter::convert);
        stringWriter.write(input);
        assertEquals(expected, stringWriter.read());
    }

    public void testCombineUpperCaseConverterWithDuplicateRemover() {
        stringWriter.setTransformer(UpperCaseConverter::convert, DuplicateRemover::convert);
        stringWriter.write(input);
        assertEquals(expected, stringWriter.read());
    }
}
