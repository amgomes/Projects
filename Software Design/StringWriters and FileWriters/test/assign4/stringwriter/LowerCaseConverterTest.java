package assign4.stringwriter;

import junit.framework.TestCase;

public class LowerCaseConverterTest extends TestCase {
    String expected = "lower case";

    public void testConvertAllUpperCaseStringToLowerCase() {
        assertEquals(expected, LowerCaseConverter.convert("LOWER CASE"));
    }

    public void testConvertSomeUpperCaseStringToLowerCase() {
        assertEquals(expected, LowerCaseConverter.convert("lOweR caSe"));
    }

    public void testConvertAllLowerCaseStringToLowerCase() {
        assertEquals(expected, LowerCaseConverter.convert("lower case"));
    }

    public void testConvertEmptyStringToLowerCase() {
        assertEquals("", LowerCaseConverter.convert(""));
    }
}