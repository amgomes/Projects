package assign4.stringwriter;

import junit.framework.TestCase;

public class LowerCaseConverterWithUpperCaseTest extends TestCase {
    StringWriter stringWriter;

    public void setUp() {
        stringWriter = new StringWriter();
    }
    public void testCombineLowerCaseConverterWithUpperCaseConverter() {
        stringWriter.setTransformer(LowerCaseConverter::convert, UpperCaseConverter::convert);
        stringWriter.write("ComBINe oPERATioNS");
        assertEquals("COMBINE OPERATIONS", stringWriter.read());
    }

    public void testCombineUpperCaseConverterWithLowerCaseConverter() {
        stringWriter.setTransformer(UpperCaseConverter::convert, LowerCaseConverter::convert);
        stringWriter.write("ComBINe oPERATioNS");
        assertEquals("combine operations", stringWriter.read());
    }
}
