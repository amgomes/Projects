package assign4.stringwriter;

import junit.framework.TestCase;

public class DuplicateRemoverWithStupidConverterTest extends TestCase{
    StringWriter stringWriter;
    String input = "stupid stupid with duplicate";
    String expected = "s***** with duplicate";

    public void setUp() {
        stringWriter = new StringWriter();
    }
    public void testCombineDuplicateRemoverWithStupidConverter() {
        stringWriter.setTransformer(DuplicateRemover::convert, StupidConverter::convert);
        stringWriter.write(input);
        assertEquals(expected, stringWriter.read());
    }

    public void testCombineStupidConverterWithDuplicateRemover() {
        stringWriter.setTransformer(StupidConverter::convert, DuplicateRemover::convert);
        stringWriter.write(input);
        assertEquals(expected, stringWriter.read());
    }
}