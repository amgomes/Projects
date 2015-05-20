package assign4.fileWriter;

import assign4.writer.UpperCaseConverterWithWriterTest;
import assign4.writer.Writer;

import java.io.File;
import java.io.IOException;

public class UpperCaseConverterWithFileWriterTest extends UpperCaseConverterWithWriterTest {
    private String filepath = "testFile.txt";

    public void tearDown() {
        new File("testFile.txt").delete();
    }

    @Override
    public Writer createWriter() throws IOException {
        return new FileWriter(filepath);
    }
}
