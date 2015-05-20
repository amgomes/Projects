package assign4.writer;

import assign4.stringwriter.DuplicateRemover;
import junit.framework.TestCase;

import java.io.IOException;

public abstract class DuplicateRemoverWithWriterTest extends TestCase {
    private Writer writer;

    public void setUp() {
        writer = createWriter();
    }

    public abstract Writer createWriter();

    @SuppressWarnings("unchecked")
    public void testCombineWriterWriteWithDuplicateRemover() throws IOException {
        writer.setTransformer(DuplicateRemover::convert);
        writer.write("remove remove duplicate");
        assertEquals("remove duplicate", writer.read());
    }

    @SuppressWarnings("unchecked")
    public void testResultBeforeAndAfterLowerCaseWhenOperatorIsSet() throws IOException {
        writer.write("there there ");
        writer.setTransformer(DuplicateRemover::convert);
        writer.write("are are duplicates");
        assertEquals("there there are duplicates", writer.read());
    }
}
