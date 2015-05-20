package assign4.stringwriter;

import junit.framework.TestCase;

public class LowerCaseConverterWithStupidConverterTest extends TestCase {
    StringWriter stringWriter;
    String input = "stupid wiTH LOWer CaSE";
    String expected = "s***** with lower case";

    public void setUp() {
        stringWriter = new StringWriter();
    }
    public void testCombineLowerCaseConverterWithStupidConverter() {
        stringWriter.setTransformer(LowerCaseConverter::convert, StupidConverter::convert);
        stringWriter.write(input);
        assertEquals(expected, stringWriter.read());
    }

    public void testCombineStupidConverterWithLowerCaseConverter() {
        stringWriter.setTransformer(StupidConverter::convert, LowerCaseConverter::convert);
        stringWriter.write(input);
        assertEquals(expected, stringWriter.read());
    }
}
