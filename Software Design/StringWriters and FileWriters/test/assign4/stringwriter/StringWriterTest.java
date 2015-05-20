package assign4.stringwriter;

import assign4.writer.WriterTest;
import assign4.writer.Writer;

public class StringWriterTest extends WriterTest {
    @Override
    public Writer createWriter() {
        return new StringWriter();
    }
}
