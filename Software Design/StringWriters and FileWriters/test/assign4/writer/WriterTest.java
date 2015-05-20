package assign4.writer;

import junit.framework.TestCase;

import java.io.IOException;

public abstract class WriterTest extends TestCase {
    Writer writer;

    public void testCanary() {
        assertTrue(true);
    }

    public void setUp() throws IOException {
        writer = createWriter();
    }

    public abstract Writer createWriter() throws IOException;

    public void testWriterReturnsStringForValidString() throws IOException {
        writer.write("abc");
        assertEquals("abc", writer.read());
    }

    public void testWriterReturnsEmptyStringForEmptyInputString() throws IOException {
        writer.write("");
        assertEquals("", writer.read());
    }

    public void testWriterAppendsToExistingContents() throws IOException {
        writer.write("abc");
        writer.write("def");
        assertEquals("abcdef", writer.read());
    }

    public void testWriterAppendsWithNewlineCharacter() throws IOException {
        writer.write("abc");
        writer.write("\\ndef");
        assertEquals("abc\\ndef", writer.read());
    }
}
