package assign4.stringwriter;

import junit.framework.TestCase;

public class UpperCaseConverterWithStupidConverterTest extends TestCase {
    StringWriter stringWriter;

    public void setUp() {
        stringWriter = new StringWriter();
    }
    public void testCombineUpperCaseConverterWithStupidConverter() {
        stringWriter.setTransformer(UpperCaseConverter::convert, StupidConverter::convert);
        stringWriter.write("stupid wiTH uppER CaSE");
        assertEquals("STUPID WITH UPPER CASE", stringWriter.read());
    }

    public void testCombineStupidConverterWithUpperCaseConverter() {
        stringWriter.setTransformer(StupidConverter::convert, UpperCaseConverter::convert);
        stringWriter.write("stupid wiTH uppER CaSE");
        assertEquals("S***** WITH UPPER CASE", stringWriter.read());
    }
}
