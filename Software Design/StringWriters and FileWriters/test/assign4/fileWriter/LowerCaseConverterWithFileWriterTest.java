package assign4.fileWriter;

import assign4.writer.LowerCaseConverterWithWriterTest;
import assign4.writer.Writer;

import java.io.File;
import java.io.IOException;

public class LowerCaseConverterWithFileWriterTest extends LowerCaseConverterWithWriterTest {
    private String filepath = "testFile.txt";

    public void tearDown() {
        new File("testFile.txt").delete();
    }

    @Override
    public Writer createWriter() throws IOException {
        return new FileWriter(filepath);
    }
}
