package assign4.writer;

import assign4.stringwriter.StupidConverter;
import junit.framework.TestCase;

import java.io.IOException;

public abstract class StupidConverterWithWriterTest extends TestCase {
    private Writer writer;

    public void setUp() throws IOException {
        writer = createWriter();
    }

    public abstract Writer createWriter() throws IOException;

    @SuppressWarnings("unchecked")
    public void testCombineWriterWriteWithStupidConverter() throws IOException {
        writer.setTransformer(StupidConverter::convert);
        writer.write("stupid test");
        assertEquals("s***** test", writer.read());
    }

    @SuppressWarnings("unchecked")
    public void testResultBeforeAndAfterStupidConverterWhenOperatorIsSet() throws IOException {
        writer.write("stupid ");
        writer.setTransformer(StupidConverter::convert);
        writer.write("stupid test");
        assertEquals("stupid s***** test", writer.read());
    }
}

