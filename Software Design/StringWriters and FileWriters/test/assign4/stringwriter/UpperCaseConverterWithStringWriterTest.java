package assign4.stringwriter;

import assign4.writer.UpperCaseConverterWithWriterTest;
import assign4.writer.Writer;

public class UpperCaseConverterWithStringWriterTest extends UpperCaseConverterWithWriterTest {
    @Override
    public Writer createWriter() {
        return new StringWriter();
    }
}
