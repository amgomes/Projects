package assign4.stringwriter;

import assign4.writer.DuplicateRemoverWithWriterTest;
import assign4.writer.Writer;

public class DuplicateRemoverWithStringWriterTest extends DuplicateRemoverWithWriterTest {
    @Override
    public Writer createWriter() {
        return new StringWriter();
    }
}
