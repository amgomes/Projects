package assign4.writer;

import assign4.stringwriter.UpperCaseConverter;
import junit.framework.TestCase;

import java.io.IOException;

public abstract class UpperCaseConverterWithWriterTest extends TestCase {
    private Writer writer;

    public void setUp() throws IOException {
        writer = createWriter();
    }

    public abstract Writer createWriter() throws IOException;

    @SuppressWarnings("unchecked")
    public void testCombineWriterWriteWithUpperCaseConverter() throws IOException {
        writer.setTransformer(UpperCaseConverter::convert);
        writer.write("ComBINe oPERATioNS");
        assertEquals("COMBINE OPERATIONS", writer.read());
    }

    @SuppressWarnings("unchecked")
    public void testResultBeforeAndAfterUpperCaseWhenOperatorIsSet() throws IOException {
        writer.write("uPpeR ");
        writer.setTransformer(UpperCaseConverter::convert);
        writer.write("cASe");
        assertEquals("uPpeR CASE", writer.read());
    }
}
