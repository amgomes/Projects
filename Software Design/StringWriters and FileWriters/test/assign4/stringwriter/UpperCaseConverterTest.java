package assign4.stringwriter;

import junit.framework.TestCase;

public class UpperCaseConverterTest extends TestCase {
    String expected = "UPPER CASE";

    public void testConvertAllLowerCaseStringToUpperCase() {
        assertEquals(expected, UpperCaseConverter.convert("upper case"));
    }

    public void testConvertSomeLowerCaseStringToUpperCase() {
        assertEquals(expected, UpperCaseConverter.convert("upPEr cAse"));
    }

    public void testConvertAllUpperCaseStringToUpperCase() {
        assertEquals(expected, UpperCaseConverter.convert("UPPER CASE"));
    }

    public void testConvertEmptyStringToLowerCase() {
        assertEquals("", UpperCaseConverter.convert(""));
    }
}
