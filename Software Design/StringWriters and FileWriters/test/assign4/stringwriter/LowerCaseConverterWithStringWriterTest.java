package assign4.stringwriter;

import assign4.writer.LowerCaseConverterWithWriterTest;
import assign4.writer.Writer;

public class LowerCaseConverterWithStringWriterTest extends LowerCaseConverterWithWriterTest {
    @Override
    public Writer createWriter() {
        return new StringWriter();
    }
}
