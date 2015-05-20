package assign4.stringwriter;

import assign4.writer.StupidConverterWithWriterTest;
import assign4.writer.Writer;

public class StupidConverterWithStringWriterTest extends StupidConverterWithWriterTest {
    @Override
    public Writer createWriter() {
        return new StringWriter();
    }
}

