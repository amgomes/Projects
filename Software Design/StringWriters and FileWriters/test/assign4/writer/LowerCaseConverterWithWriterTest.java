package assign4.writer;

import assign4.stringwriter.LowerCaseConverter;
import junit.framework.TestCase;

import java.io.IOException;

public abstract class LowerCaseConverterWithWriterTest extends TestCase {
    private Writer writer;

    public void setUp() throws IOException {
        writer = createWriter();
    }

    public abstract Writer createWriter() throws IOException;

    @SuppressWarnings("unchecked")
    public void testCombineWriterWriteWithLowerCaseConverter() throws IOException {
        writer.setTransformer(LowerCaseConverter::convert);
        writer.write("ComBINe oPERATioNS");
        assertEquals("combine operations", writer.read());
    }

    @SuppressWarnings("unchecked")
    public void testResultBeforeAndAfterLowerCaseWhenOperatorIsSet() throws IOException {
        writer.write("lOWEr ");
        writer.setTransformer(LowerCaseConverter::convert);
        writer.write("cASe");
        assertEquals("lOWEr case", writer.read());
    }
}
