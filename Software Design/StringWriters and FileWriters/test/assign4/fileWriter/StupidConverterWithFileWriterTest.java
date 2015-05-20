package assign4.fileWriter;

import assign4.writer.StupidConverterWithWriterTest;
import assign4.writer.Writer;

import java.io.File;
import java.io.IOException;

public class StupidConverterWithFileWriterTest extends StupidConverterWithWriterTest {
    private String filepath = "testFile.txt";

    public void tearDown() {
        new File(filepath).delete();
    }

    @Override
    public Writer createWriter() throws IOException {
        return new FileWriter(filepath);
    }
}

