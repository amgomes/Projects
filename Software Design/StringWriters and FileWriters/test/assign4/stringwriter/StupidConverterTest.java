package assign4.stringwriter;

import junit.framework.TestCase;

public class StupidConverterTest extends TestCase {
    public void testReplaceOccurrenceOfTheWordStupid() {
        assertEquals("s*****", StupidConverter.convert("stupid"));
    }

    public void testReplaceMultipleOccurrencesOfTheWordStupid() {
        assertEquals("blahs*****s*****blahs*****blah s*****",
                StupidConverter.convert("blahstupidstupidblahstupidblah stupid"));
    }

    public void testNoOccurrencesOfTheWordStupid() {
        assertEquals("regular string", StupidConverter.convert("regular string"));
    }

    public void testReplacingTheWordStupidOnAnEmptyString() {
        assertEquals("", StupidConverter.convert(""));
    }
}